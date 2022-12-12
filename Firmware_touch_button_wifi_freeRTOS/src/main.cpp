#include "main.h"

#include <AsyncTCP.h>

EepromClass eeprom;
AsyncWebServer server(80);
DNSServer dns;
AsyncWiFiManager wifiManager(&server, &dns);
long debouncing_time = 150;
volatile unsigned long last_micros;

static U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(
    /* clock=*/SCL, /* data=*/SDA,
    /* reset=*/U8X8_PIN_NONE);  // OLEDs without Reset of the Display

deviceCurrent device1;
deviceCurrent device2;
deviceCurrent device3;
String nameSwitch = "";
String token = "";

SemaphoreHandle_t xBinarySemaphore1;
SemaphoreHandle_t xBinarySemaphore2;
SemaphoreHandle_t xBinarySemaphore3;
TaskHandle_t interrupt1 = NULL;
TaskHandle_t interrupt2 = NULL;
TaskHandle_t interrupt3 = NULL;
TaskHandle_t checkWifi = NULL;
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
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_torussansbold8_r);
  u8x8.drawString(5, 1, "IOT-HOME");
  u8x8.drawString(1, 3, "CONNECTING WIFI");
  u8x8.drawString(1, 5, "...............");
  delay(200);
  checkConnect();
  u8x8.clearLine(3);
  u8x8.clearLine(5);
  u8x8.drawString(2, 3, "CONNECTED!");
  delay(200);
  beginFirebase(u8x8);

  xBinarySemaphore1 = xSemaphoreCreateBinary();
  xBinarySemaphore2 = xSemaphoreCreateBinary();
  xBinarySemaphore3 = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(runCallBack1, "interrupt1", 10000, NULL, 2,
                          &interrupt1, 0);
  xTaskCreatePinnedToCore(runCallBack2, "interrupt2", 10000, NULL, 3,
                          &interrupt2, 0);
  xTaskCreatePinnedToCore(runCallBack3, "interrupt3", 10000, NULL, 4,
                          &interrupt3, 0);
  xTaskCreatePinnedToCore(checkInternet, "checkInternet", 10000, NULL, 5,
                          &checkWifi, 0);
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
      device1.state = !device1.state;
      println_((String)device1.state);
      bool state = setStateToFirebase1(device1.state, token);
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
      device2.state = !device2.state;
      println_((String)device2.state);
      bool state = setStateToFirebase2(device2.state, token);
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
      device3.state = !device3.state;
      println_((String)device3.state);
      bool state = setStateToFirebase3(device3.state, token);
      vTaskDelay(50);
      if (state) t = 0;
      else t++;
      if (t > 5) ESP.restart();
    }
  }
}
void checkInternet(void* parameter) {
  int t = 0;
  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      println_("disconnect");
      t++;
      if (t >= 3) ESP.restart();
    }
    println_("check internet");
    vTaskDelay(4000);
  }
}
