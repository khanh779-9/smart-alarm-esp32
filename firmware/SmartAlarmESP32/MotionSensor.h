#pragma once
#include <Arduino.h>

class MotionSensor {
public:
  void begin(uint8_t pin);
  // Trả về true đúng 1 lần khi vừa phát hiện chuyển động mới (đã debounce)
  bool detectTriggered();

private:
  uint8_t _pin = 0;
  unsigned long _lastTriggerMs = 0;
  bool _wasHigh = false;
};
