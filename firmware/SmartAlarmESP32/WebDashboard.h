#pragma once
#include <WebServer.h>
#include "EventLog.h"
#include "AlarmController.h"

class WebDashboard {
public:
  void begin(uint16_t port, EventLog* eventLog, AlarmController* alarm);
  void handleClient(); // gọi trong loop()

private:
  WebServer* _server = nullptr;
  EventLog* _eventLog = nullptr;
  AlarmController* _alarm = nullptr;

  void handleRoot();
  void handleStatus();
  void handleEvents();
  void handleArm();
};
