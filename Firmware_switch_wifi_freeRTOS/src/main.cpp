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
TaskHandle_t checkTime = NULL;
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
  initPin();
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
  xTaskCreatePinnedToCore(checkTimer, "checkTime", 10000, NULL, 1, &checkTime,
                          5);
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
      stateDevice.stateD1 = !stateDevice.stateD1;
      println_((String)stateDevice.stateD1);
      bool state = setStateToFirebase1(stateDevice.stateD1);
      vTaskDelay(50);
      if (state) t = 0;
      else t++;
      if (t > 5) ESP.restart();
    }
  }
}
void runCallBack2(void* parameter) {
  int t = 0;
  while (true) {
    if (xSemaphoreTake(xBinarySemaphore2, portMAX_DELAY) == pdPASS) {
      stateDevice.stateD2 = !stateDevice.stateD2;
      println_((String)stateDevice.stateD2);
      bool state = setStateToFirebase2(stateDevice.stateD2);
      vTaskDelay(50);
      if (state) t = 0;
      else t++;
      if (t > 5) ESP.restart();
    }
  }
}
void runCallBack3(void* parameter) {
  int t = 0;
  while (true) {
    if (xSemaphoreTake(xBinarySemaphore3, portMAX_DELAY) == pdPASS) {
      stateDevice.stateD3 = !stateDevice.stateD3;
      println_((String)stateDevice.stateD3);
      bool state = setStateToFirebase3(stateDevice.stateD3);
      vTaskDelay(50);
      if (state) t = 0;
      else t++;
      if (t > 5) ESP.restart();
    }
  }
}
void checkTimer(void* parameter) {
  TimeDateClass timeDate;
  int t = 0;
  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      println_("disconnect");
      t++;
      if (t >= 3) ESP.restart();
    } else {
      String current = timeDate.getDate();
      // D1
      if (current == D1.timeOn) {
        if (stateDevice.stateD1 == false) {
          stateDevice.stateD1 = true;
          setStateToFirebase1(stateDevice.stateD1);
          vTaskDelay(6000);
        }
      }
      if (current == D1.timeOff) {
        if (stateDevice.stateD1 == true) {
          stateDevice.stateD1 = false;
          setStateToFirebase1(stateDevice.stateD1);
          vTaskDelay(6000);
        }
      }
      // D2
      if (current == D2.timeOn) {
        if (stateDevice.stateD2 == false) {
          stateDevice.stateD2 = true;
          setStateToFirebase2(stateDevice.stateD2);
          vTaskDelay(6000);
        }
      }
      if (current == D2.timeOff) {
        if (stateDevice.stateD2 == true) {
          stateDevice.stateD2 = false;
          setStateToFirebase2(stateDevice.stateD2);
          vTaskDelay(6000);
        }
      }
      // D3
      if (current == D3.timeOn) {
        if (stateDevice.stateD3 == false) {
          stateDevice.stateD3 = true;
          setStateToFirebase3(stateDevice.stateD3);
          vTaskDelay(6000);
        }
      }
      if (current == D3.timeOff) {
        if (stateDevice.stateD3 == true) {
          stateDevice.stateD3 = false;
          setStateToFirebase3(stateDevice.stateD3);
          vTaskDelay(6000);
        }
      }
    }
  }
}