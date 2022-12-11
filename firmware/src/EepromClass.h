#ifndef __EEPROM_CLASS_H__
#define __EEPROM_CLASS_H__

#include <EEPROM.h>
#include "Define.h"

// define register
#define KEY_START_PASSWORD 100
#define KEY_END_PASSWORD 130
#define KEY_REGISTERED 450
#define KEY_REGISTERED1 451
#define KEY_REGISTERED2 452
#define KEY_REGISTERED3 453
#define KEY_REGISTERED4 454
#define KEY_REGISTERED5 455

class EepromClass {
 public:
  EepromClass();
  ~EepromClass();

  void init();

  void end();

  //  WRITE
  void write(String data, int beginA, int endA);
  //  WRITE one
  void write(char data, int index);

  //   READ
  String read(int beginA, int endA);

  //   READ one
  uint8_t read(int index);

  // clear
  void clear(int beginA, int endA);
  void resetEEPROM();

  void commit();

  String getToken();
  String getPassword();
  void setPassword(String password_);

 private:
  unsigned char TIME_DELAY = 2;
  unsigned char EEPROM_SIZE = 64;
};

static EepromClass Local;

#endif /** __EEPROM_CLASS_H__ **/
