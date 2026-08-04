#include "MotionSensor.h"
#include "AppConfig.h"

void MotionSensor::begin(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, INPUT);
}

bool MotionSensor::detectTriggered() {
  bool isHigh = digitalRead(_pin) == HIGH;
  bool triggered = false;

  unsigned long now = millis();
  // Chỉ tính là "phát hiện mới" khi tín hiệu vừa chuyển LOW->HIGH và đã qua thời gian debounce
  if (isHigh && !_wasHigh && (now - _lastTriggerMs > MOTION_DEBOUNCE_MS)) {
    triggered = true;
    _lastTriggerMs = now;
  }

  _wasHigh = isHigh;
  return triggered;
}
