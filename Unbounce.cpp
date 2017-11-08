/*
  Unbounce.cpp - Library for software debouncing.
  Created by adrianshepard, November 5, 2017.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Unbounce.h"

Unbounce::Unbounce(int pin, int type, void (*callback)(), int thresholdReadingMillis, int thresholdCooldownMillis)
{
    init(pin, type, callback, thresholdReadingMillis, thresholdCooldownMillis);
}

Unbounce::Unbounce(int pin, int type, void (*callback)())
{
    init(pin, type, callback, 1, 5);
}

void Unbounce::init(int pin, int type, void (*callback)(), int thresholdReadingMillis, int thresholdCooldownMillis)
{
  _data = (type & B1); // leftmost bit: type
  _data <<= 2;
  _data |= (thresholdReadingMillis & B11); // bits 2 and 3: consecutive reading level
  _data <<= 3;
  _data |= (thresholdCooldownMillis & B111); // bits 4, 5 and 6: cooldown level
  _data <<= 2;
  _data |= (type & B1); // bit 7: cooldown flag, bit 8: current pin state
  _callback = callback;
  _pinBitMask = digitalPinToBitMask(pin & B11111111);
  _portRegister = portInputRegister(digitalPinToPort(pin & B11111111));
}

void Unbounce::handle()
{
  if (((*_portRegister & _pinBitMask) != 0) == (_data & B1)){ // check if current pin state matches saved pin state
    if ((_data & B10) == 0){ // check if consecutive reading (not cooldown) flag is active
        _timestamp = 0;
    }
    return;
  }

  if (((_data & B10) >> 1) == 1){ // check if cooldown (not consecutive reading) flag is active
    if (millis() < _timestamp){ // return if cooldown target has not been met
      return;
    }else{ // unset timestamp and cooldown flag if cooldown target has been met
      _timestamp = 0;
      _data &= B11111101;
    }
  }

  if (_timestamp == 0){ // set consecutive reading target
    _timestamp = millis() + (16 >> ((((_data & B1100) >> 2) ^ B11) << 1));
  }

  if (millis() < _timestamp){ // return if the consecutive reading target has not been met
    return;
  }

  _data = _data ^ (B1); // toggle current pin state
  _data |= B10; // set cooldown flag
  _timestamp = millis() + ((255 >> (((_data & B1110000) >> 4) ^ B111)) & B11111110); // set cooldown target
  if ((_data & B1) == (_data >> 7)){ // callback if the current pin state matches the unbounce type
    _callback();
  }
}

