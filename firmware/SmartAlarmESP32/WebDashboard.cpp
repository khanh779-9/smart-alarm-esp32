#include "WebDashboard.h"
#include <ArduinoJson.h>

// Trang dashboard nhúng sẵn trong flash (PROGMEM) - không cần thẻ nhớ SPIFFS/LittleFS
static const char DASHBOARD_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart Alarm ESP32</title>
<style>
  body { font-family: -apple-system, system-ui, sans-serif; background:#0d1117; color:#edf1f5; margin:0; padding:24px; }
  .card { background:#141a22; border:1px solid #262e38; border-radius:16px; padding:20px; margin-bottom:16px; max-width:420px; }
  h1 { font-size:18px; margin:0 0 16px; letter-spacing:0.02em; }
  .state { font-size:26px; font-weight:700; font-family: 'JetBrains Mono', monospace; }
  .armed { color:#f4a261; }
  .disarmed { color:#7d8a99; }
  button { width:100%; padding:12px; border-radius:10px; border:none; font-size:15px; font-weight:600; margin-top:14px; cursor:pointer; }
  .btn-arm { background:#f4a261; color:#0d1117; }
  .btn-disarm { background:#262e38; color:#edf1f5; }
  ul { list-style:none; padding:0; margin:0; font-size:13px; color:#7d8a99; }
  li { padding:6px 0; border-bottom:1px solid #262e38; }
  .sub { font-size:12px; color:#7d8a99; margin-top:6px; }
</style>
</head>
<body>
  <div class="card">
    <h1>SMART MOTION ALARM · ESP32</h1>
    <div class="state" id="state">--</div>
    <div class="sub" id="lastMotion"></div>
    <button id="toggleBtn" onclick="toggleArm()">--</button>
  </div>
  <div class="card">
    <h1>Sự kiện gần đây</h1>
    <ul id="events"><li>Đang tải...</li></ul>
  </div>

<script>
let armed = false;

async function refresh() {
  const res = await fetch('/api/status');
  const data = await res.json();
  armed = data.armed;

  const stateEl = document.getElementById('state');
  stateEl.textContent = armed ? 'ĐANG BẬT BÁO ĐỘNG' : 'ĐANG TẮT';
  stateEl.className = 'state ' + (armed ? 'armed' : 'disarmed');

  document.getElementById('lastMotion').textContent = data.lastMotionSecAgo >= 0
    ? `Chuyển động gần nhất: ${data.lastMotionSecAgo}s trước`
    : 'Chưa phát hiện chuyển động nào';

  const btn = document.getElementById('toggleBtn');
  btn.textContent = armed ? 'TẮT báo động' : 'BẬT báo động';
  btn.className = armed ? 'btn-disarm' : 'btn-arm';

  const evRes = await fetch('/api/events');
  const events = await evRes.json();
  const list = document.getElementById('events');
  list.innerHTML = events.length
    ? events.map(e => `<li>Phát hiện chuyển động - ${e.secAgo}s trước</li>`).join('')
    : '<li>Chưa có sự kiện nào</li>';
}

async function toggleArm() {
  await fetch('/api/arm', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ armed: !armed })
  });
  refresh();
}

refresh();
setInterval(refresh, 2000);
</script>
</body>
</html>
)HTML";

void WebDashboard::begin(uint16_t port, EventLog* eventLog, AlarmController* alarm) {
  _eventLog = eventLog;
  _alarm = alarm;
  _server = new WebServer(port);

  // WebServer.on() nhận std::function nên dùng lambda bắt "this" trực tiếp được, không cần static-instance trick
  _server->on("/", HTTP_GET, [this]() { handleRoot(); });
  _server->on("/api/status", HTTP_GET, [this]() { handleStatus(); });
  _server->on("/api/events", HTTP_GET, [this]() { handleEvents(); });
  _server->on("/api/arm", HTTP_POST, [this]() { handleArm(); });

  _server->begin();
}

void WebDashboard::handleClient() {
  if (_server) _server->handleClient();
}

void WebDashboard::handleRoot() {
  _server->send_P(200, "text/html", DASHBOARD_HTML);
}

void WebDashboard::handleStatus() {
  StaticJsonDocument<128> doc;
  doc["armed"] = _alarm->isArmed();

  if (_eventLog->count() > 0) {
    unsigned long lastMs = _eventLog->get(0).timestampMs;
    doc["lastMotionSecAgo"] = (millis() - lastMs) / 1000;
  } else {
    doc["lastMotionSecAgo"] = -1;
  }

  String out;
  serializeJson(doc, out);
  _server->send(200, "application/json", out);
}

void WebDashboard::handleEvents() {
  StaticJsonDocument<512> doc;
  JsonArray arr = doc.to<JsonArray>();

  int n = _eventLog->count();
  for (int i = 0; i < n; i++) {
    JsonObject obj = arr.createNestedObject();
    obj["secAgo"] = (millis() - _eventLog->get(i).timestampMs) / 1000;
  }

  String out;
  serializeJson(doc, out);
  _server->send(200, "application/json", out);
}

void WebDashboard::handleArm() {
  if (!_server->hasArg("plain")) {
    _server->send(400, "application/json", "{\"error\":\"thieu body\"}");
    return;
  }

  StaticJsonDocument<64> doc;
  DeserializationError err = deserializeJson(doc, _server->arg("plain"));
  if (err) {
    _server->send(400, "application/json", "{\"error\":\"json khong hop le\"}");
    return;
  }

  bool armed = doc["armed"] | false;
  _alarm->setArmed(armed);
  _server->send(200, "application/json", "{\"ok\":true}");
}
