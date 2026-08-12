#include "AlarmController.h"
#include "AppConfig.h"

void AlarmController::begin(uint8_t buzzerPin, uint8_t ledPin) {
  _buzzerPin = buzzerPin;
  _ledPin = ledPin;
  pinMode(_buzzerPin, OUTPUT);
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_buzzerPin, LOW);
  digitalWrite(_ledPin, LOW);
}

void AlarmController::setArmed(bool armed) {
  _armed = armed;
  if (!armed) {
    _alarming = false;
    digitalWrite(_buzzerPin, LOW);
    digitalWrite(_ledPin, LOW);
  }
}

bool AlarmController::isArmed() const {
  return _armed;
}

void AlarmController::triggerAlarm() {
  if (!_armed) return; // đã tắt báo động thì phát hiện chuyển động cũng không kêu còi
  _alarming = true;
  _alarmStartMs = millis();
  digitalWrite(_buzzerPin, HIGH);
  digitalWrite(_ledPin, HIGH);
}

void AlarmController::update() {
  // Tự tắt còi sau ALARM_SOUND_DURATION_MS mà KHÔNG dùng delay() - loop() vẫn chạy mượt để phục vụ web server
  if (_alarming && millis() - _alarmStartMs >= ALARM_SOUND_DURATION_MS) {
    _alarming = false;
    digitalWrite(_buzzerPin, LOW);
    digitalWrite(_ledPin, LOW);
  }
}
