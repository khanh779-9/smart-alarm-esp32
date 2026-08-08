#include "EventLog.h"

void EventLog::add(unsigned long timestampMs) {
  _events[_head] = { timestampMs };
  _head = (_head + 1) % MAX_EVENT_LOG;
  if (_count < MAX_EVENT_LOG) _count++;
}

int EventLog::count() const {
  return _count;
}

const MotionEvent& EventLog::get(int i) const {
  int index = (_head - 1 - i + MAX_EVENT_LOG * 2) % MAX_EVENT_LOG;
  return _events[index];
}
