#pragma once

// ==== Chân GPIO ====
#define PIR_PIN                   13
#define BUZZER_PIN                14
#define LED_PIN                    2   // hầu hết board ESP32 DevKit có LED onboard ở GPIO2

// ==== Thời gian ====
#define MOTION_DEBOUNCE_MS       3000   // sau khi phát hiện, bỏ qua tín hiệu PIR trong X ms (tránh trigger liên tục)
#define ALARM_SOUND_DURATION_MS  2000   // còi + đèn kêu/sáng bao lâu mỗi lần báo động
#define MAX_EVENT_LOG               20  // số sự kiện gần nhất lưu trong RAM
#define WIFI_CONNECT_TIMEOUT_MS 15000

// ==== Web server ====
#define WEB_SERVER_PORT             80
