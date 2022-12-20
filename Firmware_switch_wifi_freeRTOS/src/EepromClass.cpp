#include "EepromClass.h"

EepromClass ::EepromClass() {}
EepromClass ::~EepromClass() {}

void EepromClass::init() {
  EEPROM.begin(EEPROM_SIZE);
  delay(5);
}
// void EepromClass::end() {
//   delay(5);
//   EEPROM.end();
// }

//  WRITE
void EepromClass::write(String data, int beginA, int endA) {
  // init();
  clear(beginA, endA);

  for (int i = beginA; i <= endA && (i - beginA) < data.length(); i++) {
    EEPROM.write(i, data[i - beginA]);
    EEPROM.commit();
    delay(TIME_DELAY);
  }
  // commit();
  // end();
}

//  WRITE one
void EepromClass::write(char data, int index) {
  EEPROM.write(index, data);
  EEPROM.commit();
  delay(TIME_DELAY);
}

//   READ
String EepromClass::read(int beginA, int endA) {
  // init();
  String data;
  char c;
  for (int i = beginA; i <= endA; ++i) {
    c = (char)EEPROM.read(i);
    if (c != 255) {
      data += c;
    }
    delay(TIME_DELAY);
  }
  // end();
  return data;
}

//   READ one
char EepromClass::read(int index) {
  char data = (char)EEPROM.read(index);
  delay(TIME_DELAY);
  return data;
}

// clear
void EepromClass::clear(int beginA, int endA) {
  for (int i = beginA; i <= endA; ++i) {
    EEPROM.write(i, 0);
    delay(TIME_DELAY);
  }
}

void EepromClass::resetEEPROM() { clear(0, EEPROM_SIZE); }

// void EepromClass::commit() {
//   if (EEPROM.commit()) {
//     // println_(F("[write] Ghi thành công "));
//   } else {
//     // println_(F("[write] Ghi thất bại "));
//   }
// }

String EepromClass::getToken() {
  String token = WiFi.macAddress();
  for (int i = 5; i >= 1; i--) {
    // remove ':'
    token.remove(3 * i - 1, 1);
  }
  return token;
}

String EepromClass::getSwitchName() {
  String SwitchName = read(KEY_START_NAME, KEY_END_NAME);
  println_("Switch Name: " + SwitchName);
  return SwitchName;
}

void EepromClass::setSwitchName(String switchName) {
  write(switchName, KEY_START_NAME, KEY_END_NAME);
}