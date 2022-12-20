#include "Define.h"

const bool ___debug = true;

void output_(int PIN) {
  pinMode(PIN, OUTPUT);
}

void input_(int PIN) { pinMode(PIN, INPUT_PULLDOWN); }

void print_(String text) {
  if (___debug) {
    Serial.print(text);
  }
}

void println_(String text) {
  if (___debug) {
    Serial.println(text);
  }
}

void println_(int text) {
  if (___debug) {
    Serial.println(text);
  }
}

bool getPin_(int PIN) {
  int state = digitalRead(PIN);
  if (state == 1) return true;
  if (state == 0) return false;
}

void setPin_(int PIN, bool value) {
  int state;
  if (value == true) state = 1;
  if (value == false) state = 0; 
  digitalWrite(PIN, value); 
}
