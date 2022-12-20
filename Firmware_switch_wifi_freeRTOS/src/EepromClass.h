#ifndef __EEPROM_CLASS_H__
#define __EEPROM_CLASS_H__

#include <EEPROM.h>
#include "Define.h"

// define register
#define KEY_START_NAME 1
#define KEY_END_NAME 30
#define KEY_START_D1 31
#define KEY_END_D1 50
#define KEY_START_D2 51
#define KEY_END_D2 70
#define KEY_START_D3 71
#define KEY_END_D3 90
#define KEY_STATE_D1 100
#define KEY_STATE_D2 110
#define KEY_STATE_D3 120

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
  char read(int index);

  // clear
  void clear(int beginA, int endA);
  void resetEEPROM();

  void commit();

  String getToken();
  String getSwitchName();
  void setSwitchName(String switchName);

 private:
  unsigned char TIME_DELAY = 5;
  unsigned char EEPROM_SIZE = 128;
};

static EepromClass Local;

#endif /** __EEPROM_CLASS_H__ **/
