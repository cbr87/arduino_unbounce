/*
  Unbounce.h - Library for software debouncing.
  Created by adrianshepard, November 5, 2017.
  Released into the public domain.
*/
#ifndef Unbounce_h
#define Unbounce_h

#include "Arduino.h"

class Unbounce
{
  public:
    Unbounce (int pin, int type, void (*callback)());
    Unbounce (int pin, int type, void (*callback)(), int thresholdReadingMillis, int thresholdCooldownMillis);
    void handle();
  private:
    void init(int pin, int type, void (*callback)(), int thresholdReadingMillis, int thresholdCooldownMillis);
    uint8_t _data;
    uint8_t _pinBitMask;
    volatile uint8_t* _portRegister;
    void (*_callback)();
    unsigned long _timestamp;
};

#endif
