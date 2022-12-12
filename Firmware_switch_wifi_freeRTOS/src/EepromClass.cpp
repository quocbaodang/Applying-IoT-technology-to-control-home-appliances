#include "EepromClass.h"

EepromClass ::EepromClass() {}
EepromClass ::~EepromClass() {}

void EepromClass::init() {
  EEPROM.begin(EEPROM_SIZE);
  delay(5);
}
void EepromClass::end() {
  delay(5);
  EEPROM.end();
}

//  WRITE
void EepromClass::write(String data, int beginA, int endA) {
  init();
  int var = data.length() + beginA;
  if (var - 1 > endA) {
    // Log(F("[EEPRom]"),F("[write] Over length "));
  }
  clear(beginA, endA);

  for (int i = beginA; i <= endA && (i - beginA) < data.length(); i++) {
    EEPROM.write(i, data[i - beginA]);
    delay(TIME_DELAY);
  }
  commit();
  end();
}

//  WRITE one
void EepromClass::write(char data, int index) {
  EEPROM.write(index, data);
  delay(TIME_DELAY);
}

//   READ
String EepromClass::read(int beginA, int endA) {
  init();
  String data;
  char c;
  for (int i = beginA; i <= endA; ++i) {
    c = (char)EEPROM.read(i);
    if (c != 255) {
      data += c;
    }
    delay(TIME_DELAY);
  }
  end();
  return data;
}

//   READ one
uint8_t EepromClass::read(int index) {
  uint8_t data = (char)EEPROM.read(index);
  delay(TIME_DELAY);
  return data;
}

// clear
void EepromClass::clear(int beginA, int endA) {
  for (int i = beginA; i <= endA; ++i) {
    EEPROM.write(i, 255);
    delay(TIME_DELAY);
  }
}

void EepromClass::resetEEPROM() { clear(0, EEPROM_SIZE); }

void EepromClass::commit() {
  if (EEPROM.commit()) {
    // println_(F("[write] Ghi thành công "));
  } else {
    // println_(F("[write] Ghi thất bại "));
  }
}

String EepromClass::getToken() {
  String token = WiFi.macAddress();
  for (int i = 5; i >= 1; i--) {
    // remove ':'
    token.remove(3 * i - 1, 1);
  }
  return token;
}

String EepromClass::getPassword() {
  return read(KEY_START_PASSWORD, KEY_END_PASSWORD);
}

void EepromClass::setPassword(String password_) {
  write(password_, KEY_START_PASSWORD, KEY_END_PASSWORD);
}