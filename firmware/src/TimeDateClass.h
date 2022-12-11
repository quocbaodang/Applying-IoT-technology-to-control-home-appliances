#ifndef __TIME_DATE__H__
#define __TIME_DATE__H__

#include <Arduino.h>
#include <WiFiUdp.h>

#include "NTPClient.h"

class TimeDateClass {
 private:
    const long utcOffsetInSeconds = 7 * 60 * 60;
    WiFiUDP ntpUDP;
    NTPClient timeClient = NTPClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

 public:
  TimeDateClass();
  ~TimeDateClass();
  void begin();
  String getDate();
  String getTimeCurrent();
  int getDay();
};

#endif /** __TIME_DATE__H__ **/
