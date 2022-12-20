#include "main.h"

#include <AsyncTCP.h>

EepromClass eeprom;
AsyncWebServer server(80);
DNSServer dns;
AsyncWiFiManager wifiManager(&server, &dns);
long debouncing_time = 150;
volatile unsigned long last_micros;

deviceStateCurrent stateDevice;
timerDevice D1;
timerDevice D2;
timerDevice D3;

SemaphoreHandle_t xBinarySemaphore1;
SemaphoreHandle_t xBinarySemaphore2;
SemaphoreHandle_t xBinarySemaphore3;
TaskHandle_t interrupt1 = NULL;
TaskHandle_t interrupt2 = NULL;
TaskHandle_t interrupt3 = NULL;
TaskHandle_t checkTimer = NULL;
void ISRcallback1();
void ISRcallback2();
void ISRcallback3();

void initPin() {
  // input
  input_(PIN_TOUCH_1);
  input_(PIN_TOUCH_2);
  input_(PIN_TOUCH_3);
  // output
  output_(D1_PIN);
  output_(D2_PIN);
  output_(D3_PIN);
  if (Local.read(KEY_STATE_D1) == '1') setPin_(D1_PIN, true);
  else setPin_(D1_PIN, false);
  if (Local.read(KEY_STATE_D2) == '1') setPin_(D2_PIN, true);
  else setPin_(D2_PIN, false);
  if (Local.read(KEY_STATE_D3) == '1') setPin_(D3_PIN, true);
  else setPin_(D3_PIN, false);
}

void startConnect() {
  println_("AUTO CONNECT");
  if (wifiManager.autoConnect()) {
    println_("WIFI is connected!");
    delay(500);
  }
}

void checkConnect() {
  println_("Connect to wifi: " + (String)WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(5000);
  if (WiFi.status() != WL_CONNECTED) {
    startConnect();
  }
  println_(F("Connected with IP: "));
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  println_("init begin");
  Local.init();
  initPin();
  String name = Local.getSwitchName();
  println_("Name get local: " + name);
  println_("set pin success!");
  checkConnect();
  beginFirebase();
  
  xBinarySemaphore1 = xSemaphoreCreateBinary();
  xBinarySemaphore2 = xSemaphoreCreateBinary();
  xBinarySemaphore3 = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(runCallBack1, "interrupt1", 10000, NULL, 2,
                          &interrupt1, 0);
  xTaskCreatePinnedToCore(runCallBack2, "interrupt2", 10000, NULL, 3,
                          &interrupt2, 0);
  xTaskCreatePinnedToCore(runCallBack3, "interrupt3", 10000, NULL, 4,
                          &interrupt3, 0);
  xTaskCreatePinnedToCore(checkTime, "checkTime", 10000, NULL, 1, &checkTimer,
                          0);

  attachInterrupt(PIN_TOUCH_1, ISRcallback1, FALLING);
  attachInterrupt(PIN_TOUCH_2, ISRcallback2, FALLING);
  attachInterrupt(PIN_TOUCH_3, ISRcallback3, FALLING);
}

void loop() {}

void ISRcallback1() {
  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
    xSemaphoreGiveFromISR(xBinarySemaphore1, NULL);
    last_micros = micros();
  }
}
void ISRcallback2() {
  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
    xSemaphoreGiveFromISR(xBinarySemaphore2, NULL);
    last_micros = micros();
  }
}
void ISRcallback3() {
  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
    xSemaphoreGiveFromISR(xBinarySemaphore3, NULL);
    last_micros = micros();
  }
}

void runCallBack1(void* parameter) {
  int t = 0;
  while (true) {
    if (xSemaphoreTake(xBinarySemaphore1, portMAX_DELAY) == pdPASS) {
      bool state = !stateDevice.stateD1;
      println_((String)state);
      bool st = setStateToFirebase1(state);
      vTaskDelay(50);
      if (st) {
        t = 0;
        stateDevice.stateD1 = state;
      } else {
        t++;
        if (t > 3) ESP.restart();
      }
    }
  }
}
void runCallBack2(void* parameter) {
  int t = 0;
  while (true) {
    if (xSemaphoreTake(xBinarySemaphore2, portMAX_DELAY) == pdPASS) {
      bool state = !stateDevice.stateD2;
      println_((String)state);
      bool st = setStateToFirebase2(state);
      vTaskDelay(50);
      if (st) {
        t = 0;
        stateDevice.stateD2 = state;
      } else {
        t++;
        if (t > 3) ESP.restart();
      }
    }
  }
}
void runCallBack3(void* parameter) {
  int t = 0;
  while (true) {
    if (xSemaphoreTake(xBinarySemaphore3, portMAX_DELAY) == pdPASS) {
      bool state = !stateDevice.stateD3;
      println_((String)state);
      bool st = setStateToFirebase3(state);
      vTaskDelay(50);
      if (st) {
        t = 0;
        stateDevice.stateD3 = state;
      } else {
        t++;
        if (t > 3) ESP.restart();
      }
    }
  }
}
// Check timer
void checkTime(void* parameter) {
  TimeDateClass timeDate;
  int t = 0;
  bool check1 = true;
  bool check2 = true;
  bool check3 = true;
  bool check4 = true;
  bool check5 = true;
  bool check6 = true;
  String current = "";
  while (true) {
    println_("Check timer");
    if (WiFi.status() != WL_CONNECTED) {
      println_("disconnect");
      t++;
      if (t >= 2) ESP.restart();
    } else {
      t = 0;
      String time = timeDate.getDate();
      if (current != time) {
        current = time;
        check1 = true;
        check2 = true;
        check3 = true;
        check4 = true;
        check5 = true;
        check6 = true;
      }
      println_("Current Time: " + current);
      // D1
      if (current == D1.timeOn && check1) {
        if (setStateToFirebase1(true)) {
          vTaskDelay(60);
          check1 = false;
        }
      }
      if (current == D1.timeOff && check2) {
        if (setStateToFirebase1(false)) {
          vTaskDelay(60);
          check2 = false;
        }
      }
      // D2
      if (current == D2.timeOn && check3) {
        if (setStateToFirebase2(true)) {
          vTaskDelay(60);
          check3 = false;
        }
      }
      if (current == D2.timeOff && check4) {
        if (setStateToFirebase2(false)) {
          vTaskDelay(60);
          check4 = false;
        }
      }
      // D3
      if (current == D3.timeOn && check5) {
        if (setStateToFirebase3(true)) {
          vTaskDelay(60);
          check5 = false;
        }
      }
      if (current == D3.timeOff && check6) {
        if (setStateToFirebase3(false)) {
          vTaskDelay(60);
          check6 = false;
        }
      }
    }
    vTaskDelay(6000);
  }
}