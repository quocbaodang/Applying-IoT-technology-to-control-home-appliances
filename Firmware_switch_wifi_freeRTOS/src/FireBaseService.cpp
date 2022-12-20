#ifndef __FIREBASE_SERVICE__CPP__
#define __FIREBASE_SERVICE__CPP__

#include "Define.h"
#include "main.h"

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
extern deviceStateCurrent stateDevice;
extern timerDevice D1;
extern timerDevice D2;
extern timerDevice D3;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData dataStream;
FirebaseData dataFirebase;

void beginFirebase() {
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback =
      tokenStatusCallback;  // see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Firebase.begin(DATABASE_URL, "euA3w7XT1NstVQR8uWCTH6fg4qDCjTogTS1RP7H1");
  Firebase.begin(&config, &auth);

  //  Firebase.begin(DATABASE_URL, "<database secret>");

  Firebase.reconnectWiFi(true);

  println_(F("Connect firebase complete"));

  delay(1000);
  if (!checkNewDevice()) {
    createNewDevice();
  }
  delay(500);
  getStateFromFirebase();
  getTimer();
  streamDevide((String)PARENT_PATH);
}

void streamDevide(String pathStream) {
  int t = 0;
  println_(F("Begin streamDevide "));
  String token = Local.getToken();
  // Stream
  while (!Firebase.beginStream(dataStream, pathStream + "/" + token)) {
    // Log(F("Firebase"), F("Stream error ! ! ! =>"));
    print_(pathStream + token);
    print_(" : ");
    println_(dataStream.errorReason());
    t++;
    if (t > 5) {
      ESP.restart();
    }
    delay(1500);
  }
  println_((String)dataStream.dataPath());
  Firebase.setStreamCallback(dataStream, streamCallbackSevice,
                             streamTimeoutCallback);
  delay(500);
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) Serial.println("stream timeout, resuming...\n");
}

void streamCallbackSevice(StreamData data) {
  String path = data.dataPath();
  // Get state
  if (path == (String)STATE1_PATH + "/state") {
    bool st = data.boolData();
    stateDevice.stateD1 = st;
    setPin_(D1_PIN, st);
    Local.write((char)st, KEY_STATE_D1);
    println_("State: " + (String)st);
  }
  if (path == (String)STATE2_PATH + "/state") {
    bool st = data.boolData();
    stateDevice.stateD2 = st;
    setPin_(D2_PIN, st);
    Local.write((char)st, KEY_STATE_D2);
    println_("State: " + (String)st);
  }
  if (path == (String)STATE3_PATH + "/state") {
    bool st = data.boolData();
    stateDevice.stateD3 = st;
    setPin_(D3_PIN, st);
    Local.write((char)st, KEY_STATE_D3);
    println_("State: " + (String)st);
  }
  // Get name device
  if (path == (String)STATE1_PATH + "/name") {
    String name = data.stringData();
    Local.write(name, KEY_START_D1, KEY_END_D1);
    println_("D1 Name: " + name);
  }
  if (path == (String)STATE2_PATH + "/name") {
    String name = data.stringData();
    Local.write(name, KEY_START_D2, KEY_END_D2);
    println_("D2 Name: " + name);
  }
  if (path == (String)STATE3_PATH + "/name") {
    String name = data.stringData();
    Local.write(name, KEY_START_D3, KEY_END_D3);
    println_("D3 Name: " + name);
  }
  // get timer
  if (path == (String)STATE1_PATH + "/time_on") {
    D1.timeOn = data.stringData();
    println_("D1 Time on: " + D1.timeOn);
  }
  if (path == (String)STATE1_PATH + "/time_off") {
    D1.timeOff = data.stringData();
    println_("D1 Time off: " + D1.timeOff);
  }
  if (path == (String)STATE2_PATH + "/time_on") {
    D2.timeOn = data.stringData();
    println_("D2 Time on: " + D2.timeOn);
  }
  if (path == (String)STATE2_PATH + "/time_off") {
    D2.timeOff = data.stringData();
    println_("D2 Time off: " + D2.timeOff);
  }
  if (path == (String)STATE3_PATH + "/time_on") {
    D3.timeOn = data.stringData();
    println_("D3 Time on: " + D3.timeOn);
  }
  if (path == (String)STATE3_PATH + "/time_off") {
    D3.timeOff = data.stringData();
    println_("D3 Time off: " + D3.timeOff);
  }
  //get Name Switch
  if (path == "/name") {
    String nameSwitch = data.stringData();
    println_("Name of switch wifi: " + nameSwitch);
    Local.setSwitchName(nameSwitch);
  }
}

// Get state form Firebase
bool getStateFromFirebase1() {
  println_("Get state D1 from firebase...");
  String token = Local.getToken();
  String pathParent = "/iot-home/devices/" + token + "/D1" + "/state";
  Firebase.getBool(dataFirebase, pathParent);
  vTaskDelay(100);
  return dataFirebase.boolData();
}

bool getStateFromFirebase2() {
  println_("Get state D2 from firebase...");
  String token = Local.getToken();
  String pathParent = "/iot-home/devices/" + token + "/D2" + "/state";
  Firebase.getBool(dataFirebase, pathParent);
  vTaskDelay(100);
  return dataFirebase.boolData();
}

bool getStateFromFirebase3() {
  println_("Get state D3 from firebase...");
  String token = Local.getToken();
  String pathParent = "/iot-home/devices/" + token + "/D3" + "/state";
  Firebase.getBool(dataFirebase, pathParent);
  vTaskDelay(100);
  return dataFirebase.boolData();
}

void getStateFromFirebase() {
  stateDevice.stateD1 = getStateFromFirebase1();
  setPin_(D1_PIN, stateDevice.stateD1);
  stateDevice.stateD2 = getStateFromFirebase2();
  setPin_(D2_PIN, stateDevice.stateD2);
  stateDevice.stateD3 = getStateFromFirebase3();
  setPin_(D3_PIN, stateDevice.stateD3);
}

// Set state to firebase
bool setStateToFirebase1(bool state) {
  println_("Set state for D1 to firebase...");
  String token = Local.getToken();
  String pathParent = (String)PARENT_PATH + "/" + token;
  if (Firebase.setBool(dataFirebase,
                       pathParent + (String)STATE1_PATH + "/state", state)) {
    println_("Set state for D1 successful!");
    return true;
  } else {
    println_("Set state for D1 failed");
    return false;
  }
}

bool setStateToFirebase2(bool state) {
  println_("Set state for D2 to firebase...");
  String token = Local.getToken();
  String pathParent = (String)PARENT_PATH + "/" + token;
  if (Firebase.setBool(dataFirebase,
                       pathParent + (String)STATE2_PATH + "/state", state)) {
    println_("Set state for D2 successful!");
    return true;
  } else {
    println_("Set state for D2 failed");
    return false;
  }
}

bool setStateToFirebase3(bool state) {
  println_("Set state for D3 to firebase...");
  String token = Local.getToken();
  String pathParent = (String)PARENT_PATH + "/" + token;
  if (Firebase.setBool(dataFirebase,
                       pathParent + (String)STATE3_PATH + "/state", state)) {
    println_("Set state for D3 successful!");
    return true;
  } else {
    println_("Set state for D3 failed");
    return false;
  }
}

void createNewDevice() {
  FirebaseJson deviceJson;
  String token = Local.getToken();
  String name = "name";
  String pathParent = (String)PARENT_PATH + "/" + token;
  deviceJson.set(F("/D1/state"), false);
  deviceJson.set(F("/D1/time_on"), "");
  deviceJson.set(F("D1/time_off"), "");
  deviceJson.set(F("/D1/name"), "D1");
  deviceJson.set(F("/D2/state"), false);
  deviceJson.set(F("/D2/time_on"), "");
  deviceJson.set(F("D2/time_off"), "");
  deviceJson.set(F("/D2/name"), "D2");
  deviceJson.set(F("/D3/state"), false);
  deviceJson.set(F("/D3/time_on"), "");
  deviceJson.set(F("D3/time_off"), "");
  deviceJson.set(F("/D3/name"), "D3");
  deviceJson.set(F("/token"), token);
  deviceJson.set(F("/name"), name);
  vTaskDelay(200);

  if (Firebase.set(dataFirebase, pathParent, deviceJson)) {
    vTaskDelay(200);
    if (Firebase.getString(dataFirebase,
                           (String)PARENT_PATH + (String)LIST_DEVICE)) {
      String listDevice = dataFirebase.stringData() + token + ";";
      Firebase.setString(dataFirebase,
                         (String)PARENT_PATH + (String)LIST_DEVICE, listDevice);
    }
    println_("Create device successful!");
  } else {
    println_("Create the new device failed. Create again...");
    ESP.restart();
  }
}

bool checkNewDevice() {
  String token = Local.getToken();
  String pathParent = (String)PARENT_PATH + "/" + token;

  if (Firebase.getString(dataFirebase, pathParent + (String)TOKEN_PATH)) {
    println_("TOKEN: " + dataFirebase.stringData());
    println_(token);
    if (dataFirebase.stringData() == token) {
      println_("Device is activated!");
      vTaskDelay(200);
      return true;
    } else {
      return false;
    }
  } else {
    println_("FAILED");
    println_("REASON: " + dataFirebase.errorReason());
    println_("------------------------------------\n");
    return false;
  }
  return false;
}

void getTimer() {
  println_("Get time ON or OFF from firebase...");
  String token = Local.getToken();
  String pathParent = "/iot-home/devices/" + token;
  // D1
  Firebase.getString(dataFirebase, pathParent + "/D1/time_on");
  vTaskDelay(20);
  D1.timeOn = dataFirebase.stringData();
  Firebase.getString(dataFirebase, pathParent + "/D1/time_off");
  vTaskDelay(20);
  D1.timeOff = dataFirebase.stringData();
  // D2
  Firebase.getString(dataFirebase, pathParent + "/D2/time_on");
  vTaskDelay(20);
  D2.timeOn = dataFirebase.stringData();
  Firebase.getString(dataFirebase, pathParent + "/D2/time_off");
  vTaskDelay(20);
  D2.timeOff = dataFirebase.stringData();
  // D3
  Firebase.getString(dataFirebase, pathParent + "/D3/time_on");
  vTaskDelay(20);
  D2.timeOn = dataFirebase.stringData();
  Firebase.getString(dataFirebase, pathParent + "/D3/time_off");
  vTaskDelay(20);
  D2.timeOff = dataFirebase.stringData();
}

#endif  // __FIREBASE_SERVICE__CPP__
