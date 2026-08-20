/*
 * SmartAlarmESP32
 * Cảm biến PIR phát hiện chuyển động -> kêu còi + đèn báo động.
 * ESP32 tự host 1 web dashboard (không cần backend/database ngoài) để xem trạng thái
 * và bật/tắt báo động từ xa qua trình duyệt trong cùng mạng WiFi.
 *
 * Thư viện cần cài (Library Manager): ArduinoJson (Benoit Blanchon).
 * WiFi.h và WebServer.h đã có sẵn trong ESP32 board package, không cần cài thêm.
 *
 * Trước khi upload: copy secrets.example.h -> secrets.h và điền WiFi thật.
 */

#include <WiFi.h>
#include "AppConfig.h"
#include "secrets.h"
#include "NetworkManager.h"
#include "MotionSensor.h"
#include "AlarmController.h"
#include "EventLog.h"
#include "WebDashboard.h"

NetworkManager networkManager;
MotionSensor motionSensor;
AlarmController alarmController;
EventLog eventLog;
WebDashboard webDashboard;

void setup() {
  Serial.begin(115200);
  delay(500);

  motionSensor.begin(PIR_PIN);
  alarmController.begin(BUZZER_PIN, LED_PIN);
  alarmController.setArmed(true); // mặc định bật báo động khi khởi động

  networkManager.begin(WIFI_SSID, WIFI_PASSWORD);
  networkManager.ensureConnected();

  webDashboard.begin(WEB_SERVER_PORT, &eventLog, &alarmController);

  Serial.print("Dashboard đang chạy tại: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  networkManager.ensureConnected();
  webDashboard.handleClient(); // xử lý request HTTP, không block
  alarmController.update();    // tự tắt còi/đèn khi hết thời gian báo động

  if (motionSensor.detectTriggered()) {
    Serial.println("Phát hiện chuyển động!");
    eventLog.add(millis());
    alarmController.triggerAlarm();
  }
}
