#ifndef __DEFINE__H__
#define __DEFINE__H__

#include <Arduino.h>
#include <FirebaseESP32.h>
#include <WiFi.h>

#include "ESPAsyncWiFiManager.h"
#include "EepromClass.h"
#include "TimeDateClass.h"
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

// define baurate
#define SERIAL_BAUD 115200

// define state
#define STATE_ON 0
#define STATE_OFF 1
#define TOUCH_ON 1
#define TOUCH_OFF 0

// define GPIO
#define D1_PIN GPIO_NUM_25
#define D2_PIN GPIO_NUM_26
#define D3_PIN GPIO_NUM_27
#define PIN_TOUCH_1 GPIO_NUM_15      // D15
#define PIN_TOUCH_2 GPIO_NUM_18       // D2
#define PIN_TOUCH_3 GPIO_NUM_19      // D4

// define wifi
#define WIFI_SSID "GDB"
#define WIFI_PASSWORD "123456789bao"

// define firebase config
#define LENG_ID 16

/* 1. URL */
#define DATABASE_URL "https://iot-home-4d236-default-rtdb.firebaseio.com/"
/* 2. Define the API Key */
#define API_KEY "AIzaSyCF92ixJjodLzGGpPIWT2bLMb6ccympISY"
/* 4. Define the user Email and password */
#define USER_EMAIL "dqst09@gmail.com"
#define USER_PASSWORD "Stormdang"

#define PARENT_PATH "/iot-home/devices"
#define STATE1_PATH "/D1"
#define STATE2_PATH "/D2"
#define STATE3_PATH "/D3"
#define LIST_DEVICE "/list-device"
#define STATE_PATH "/state"
#define TIMER_ON_PATH "/time-on"
#define TIMER_OFF_PATH "/timer-off"
#define TOKEN_PATH "/token"


struct deviceCurrent {
    bool state;
    String name;
    String timeOn;
    String timeOff;
};

// function
bool getBit(int data, int index);
void output_(int PIN);
void input_(int PIN);

void print_(String text);
void println_(String text);
void print_(int text);
void println_(int text);
bool getPin_(int PIN);
void setPin_(int PIN, bool value);

// Firebase
bool setStateToFirebase1(bool state, String token);
bool setStateToFirebase2(bool state, String token);
bool setStateToFirebase3(bool state, String token);

bool getStateFromFirebase1(String token);
bool getStateFromFirebase2(String token);
bool getStateFromFirebase3(String token);
void getStateFromFirebase(String token);

String getTokenList();
String getNameSwitch(String token);
String selectDevice(String tokenList, int num);
void streamTimeoutCallback(bool timeout);
void streamCallbackSevice(StreamData data);
void streamDevice(String pathStream);
void getTimer(String token);
void beginFirebase(U8X8_SSD1306_128X64_NONAME_SW_I2C &u8x8);
String selectDevice(U8X8_SSD1306_128X64_NONAME_SW_I2C &u8x8, String tokenList);

#endif /** __DEFINE__H__ **/
