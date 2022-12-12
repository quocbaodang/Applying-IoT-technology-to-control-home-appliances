#ifndef __FIREBASE_SERVICE__CPP__
#define __FIREBASE_SERVICE__CPP__

#include "Define.h"
#include "main.h"

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

extern deviceCurrent device1;
extern deviceCurrent device2;
extern deviceCurrent device3;
extern String nameSwitch;
extern String token;

FirebaseAuth auth;
FirebaseConfig config;
FirebaseData dataStream;
FirebaseData dataFirebase;

void beginFirebase(U8X8_SSD1306_128X64_NONAME_SW_I2C &u8x8) {
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

  String tokenList = getTokenList();
  token = selectDevice(u8x8, tokenList);
  delay(1000);
  getStateFromFirebase(token);
  // getTimer(token);
  streamDevice((String)PARENT_PATH);
}

void streamDevice(String pathStream) {
  int t = 0;
  println_(F("Begin streamDevide "));
  // Stream
  while (!Firebase.beginStream(dataStream, pathStream)) {
    // Log(F("Firebase"), F("Stream error ! ! ! =>"));
    print_(pathStream);
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
  // String token = "34865D3B0308";
  //Get state
  if (path == "/" + token + (String)STATE1_PATH + "/state") {
    device1.state = data.boolData();
    setPin_(D1_PIN, device1.state);
    println_("Path: " + path);
    println_("State: " + (String)device1.state);
  }
  if (path == "/" + token + (String)STATE2_PATH + "/state") {
    device2.state = data.boolData();
    setPin_(D2_PIN, device2.state);
    println_("Path: " + path);
    println_("State: " + (String)device2.state);
  }
  if (path == "/" + token + (String)STATE3_PATH + "/state") {
    device3.state = data.boolData();
    setPin_(D3_PIN, device3.state);
    println_("Path: " + path);
    println_("State: " + (String)device3.state);
  }
  // get name
  if (path == "/" + token + (String)STATE1_PATH + "/name") {
    device1.name = data.stringData();
    println_("Name of device 1: " + device1.name);
  }
  if (path == "/" + token + (String)STATE2_PATH + "/name") {
    device2.name = data.stringData();
    println_("Name of device 2: " + device2.name);
  }
  if (path == "/" + token + (String)STATE3_PATH + "/name") {
    device3.name = data.stringData();
    println_("Name of device 3: " + device3.name);
  }
  // get name switch wifi
  if (path == "/" + token + "/name") {
    nameSwitch = data.stringData();
    println_("Name of switch wifi: " + nameSwitch);
  }
}

// Get state form Firebase
bool getStateFromFirebase1(String token) {
  println_("Get state D1 from firebase...");
  String pathParent = "/iot-home/devices/" + token + "/D1" + "/state";
  Firebase.getBool(dataFirebase, pathParent);
  vTaskDelay(100);
  return dataFirebase.boolData();
}

bool getStateFromFirebase2(String token) {
  println_("Get state D2 from firebase...");
  String pathParent = "/iot-home/devices/" + token + "/D2" + "/state";
  Firebase.getBool(dataFirebase, pathParent);
  vTaskDelay(100);
  return dataFirebase.boolData();
}

bool getStateFromFirebase3(String token) {
  println_("Get state D3 from firebase...");
  String pathParent = "/iot-home/devices/" + token + "/D3" + "/state";
  Firebase.getBool(dataFirebase, pathParent);
  vTaskDelay(100);
  return dataFirebase.boolData();
}

void getStateFromFirebase(String token) {
  device1.state = getStateFromFirebase1(token);
  setPin_(D1_PIN, device1.state);
  device2.state = getStateFromFirebase2(token);
  setPin_(D2_PIN, device2.state);
  device3.state = getStateFromFirebase3(token);
  setPin_(D3_PIN, device3.state);
}

// Set state to firebase
bool setStateToFirebase1(bool state, String token) {
  println_("Set state for D1 to firebase...");
  String pathParent = (String)PARENT_PATH + "/" + token;
  if (Firebase.setBool(dataFirebase, pathParent + (String)STATE1_PATH + "/state", state)) {
    println_("Set state for D1 successful!");
    return true;
  } else {
    println_("Set state for D1 failed");
    return false;
  }
}

bool setStateToFirebase2(bool state, String token) {
  println_("Set state for D2 to firebase...");
  String pathParent = (String)PARENT_PATH + "/" + token;
  if (Firebase.setBool(dataFirebase, pathParent + (String)STATE2_PATH + "/state", state)) {
    println_("Set state for D2 successful!");
    return true;
  } else {
    println_("Set state for D2 failed");
    return false;
  }
}

bool setStateToFirebase3(bool state, String token) {
  println_("Set state for D3 to firebase...");
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

void getTimer(String token) {
    println_("Get time ON or OFF from firebase...");
    String pathParent = "/iot-home/devices/" + token;
    //D1
    Firebase.getString(dataFirebase, pathParent + "/D1/time-on");
    vTaskDelay(20);
    device1.timeOn = dataFirebase.stringData();
    Firebase.getString(dataFirebase, pathParent + "/D1/time-off");
    vTaskDelay(20);
    device1.timeOff = dataFirebase.stringData();
    //D2
    Firebase.getString(dataFirebase, pathParent + "/D2/time-on");
    vTaskDelay(20);
    device2.timeOn = dataFirebase.stringData();
    Firebase.getString(dataFirebase, pathParent + "/D2/time-off");
    vTaskDelay(20);
    device2.timeOff = dataFirebase.stringData();
    //D3
    Firebase.getString(dataFirebase, pathParent + "/D3/time-on");
    vTaskDelay(20);
    device3.timeOn = dataFirebase.stringData();
    Firebase.getString(dataFirebase, pathParent + "/D3/time-off");
    vTaskDelay(20);
    device3.timeOff = dataFirebase.stringData();
  }

String getTokenList() {
  println_("Get token list from firebase...");
  String path = (String)PARENT_PATH + "/list_device";
  Firebase.getString(dataFirebase, path);
  vTaskDelay(100);
  println_("Token list: " + dataFirebase.stringData());
  return dataFirebase.stringData();
}

String selectDevice(U8X8_SSD1306_128X64_NONAME_SW_I2C &u8x8, String tokenList) {
  println_("List: " + tokenList);
  u8x8.clear();
  u8x8.drawString(1, 1, "SELECT DEVICE:");
  int numberDevice = 0;
  int len = tokenList.length();
  println_("Lenght: " + (String)len);
  for (int j = 0; j < len; j++) {
    if (tokenList[j] == ';') numberDevice++;
  }
  println_("SL device: " + (String)numberDevice);
  String listName = "0";
  for (int j = 0; j < numberDevice; j++) {
    String t = tokenList.substring(j * 13, j * 13 + 12);
    String name = getNameSwitch(t);
    listName = listName + name + String(j+1);
  }
  int i = 0;
  while (i < numberDevice) {
    token = tokenList.substring(i * 13, i * 13 + 12);
    String name = listName.substring(listName.indexOf(String(i)) + 1, listName.indexOf(String(i + 1)));
    u8x8.drawString(2, 3, token.c_str());
    u8x8.drawString(2, 5, name.c_str());
    
    if (getPin_(PIN_TOUCH_3) == TOUCH_ON) {
      while (getPin_(PIN_TOUCH_3) == TOUCH_ON);
      println_("touch 3");
      break;
    }
    if (getPin_(PIN_TOUCH_1) == TOUCH_ON) {
      while (getPin_(PIN_TOUCH_1) == TOUCH_ON);
      if (i < numberDevice - 1) {
        println_("touch 1");
        i++;
        u8x8.clearLine(3);
        u8x8.clearLine(5);
      }
    }
    if (getPin_(PIN_TOUCH_2) == TOUCH_ON) {
      while (getPin_(PIN_TOUCH_2) == TOUCH_ON);
      if (i > 0) {
        println_("touch 2");
        i--;
        u8x8.clearLine(3);
        u8x8.clearLine(5);
      }
    }
  }
  u8x8.clearLine(1);
  u8x8.drawString(2, 1, "SELECTED!");
  println_("Selected!");
  println_("Token selected: " + token);
  return token;
}

String getNameSwitch(String token) {
  println_("Get name of switch wifi...");
  String path = (String)PARENT_PATH + "/" + token + "/name";
  Firebase.getString(dataFirebase, path);
  vTaskDelay(100);
  println_("Name of switch: " + dataFirebase.stringData());
  return dataFirebase.stringData();
}

#endif  // __FIREBASE_SERVICE__CPP__
