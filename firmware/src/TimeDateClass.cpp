#ifndef __TIME_DATE__CPP__
#define __TIME_DATE__CPP__

#include "TimeDateClass.h"

TimeDateClass::TimeDateClass() { timeClient.begin(); }

TimeDateClass::~TimeDateClass() { timeClient.begin(); }

void TimeDateClass::begin() {}

String TimeDateClass::getDate() {
  timeClient.update();
  return timeClient.getFormattedDate();
}

int TimeDateClass::getDay() {
  timeClient.update();
  return timeClient.getDay();
}

String TimeDateClass::getTimeCurrent() {
  timeClient.update();
  return timeClient.getFormattedTime();
}

#endif /* __TIME_DATE__CPP__ */
