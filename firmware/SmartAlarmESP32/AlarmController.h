#pragma once
#include <Arduino.h>

class AlarmController {
public:
  void begin(uint8_t buzzerPin, uint8_t ledPin);
  void setArmed(bool armed);
  bool isArmed() const;
  void triggerAlarm();   // bắt đầu kêu còi (non-blocking)
  void update();          // gọi trong loop() để tự tắt còi sau ALARM_SOUND_DURATION_MS

private:
  uint8_t _buzzerPin = 0;
  uint8_t _ledPin = 0;
  bool _armed = false;
  bool _alarming = false;
  unsigned long _alarmStartMs = 0;
};
