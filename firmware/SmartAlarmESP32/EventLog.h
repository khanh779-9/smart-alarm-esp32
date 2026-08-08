#pragma once
#include <Arduino.h>
#include "AppConfig.h"

struct MotionEvent {
  unsigned long timestampMs; // giá trị millis() lúc phát hiện
};

// Ring buffer đơn giản lưu MAX_EVENT_LOG sự kiện gần nhất trong RAM (không cần thẻ nhớ/DB ngoài)
class EventLog {
public:
  void add(unsigned long timestampMs);
  int count() const;
  const MotionEvent& get(int i) const; // i = 0 là sự kiện mới nhất

private:
  MotionEvent _events[MAX_EVENT_LOG];
  int _count = 0;
  int _head = 0;
};
