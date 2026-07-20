# SmartAlarmESP32 - Báo động chuyển động, tự host web dashboard


## Linh kiện & đấu nối

| Linh kiện                   | Chân ESP32                                          | Giá tham khảo   |
| ---------------------------- | ---------------------------------------------------- | ----------------- |
| PIR HC-SR501                 | VCC→5V, GND→GND, OUT→GPIO13 (`PIR_PIN`)         | ~15.000–20.000đ |
| Buzzer 5V (active)           | VCC→GPIO14 (`BUZZER_PIN`), GND→GND               | ~5.000–8.000đ   |
| LED báo động (tuỳ chọn) | dùng LED onboard GPIO2, hoặc LED rời + trở 220Ω | ~1.000đ          |

HC-SR501 có 2 biến trở nhỏ: 1 chỉnh độ nhạy, 1 chỉnh thời gian giữ tín hiệu HIGH — để mặc định
là dùng được, không cần chỉnh.

## Cài đặt

1. Cài thư viện **ArduinoJson** qua Library Manager (Arduino IDE).
2. Mở `firmware/SmartAlarmESP32/SmartAlarmESP32.ino`.
3. Copy `secrets.example.h` → `secrets.h`, điền `WIFI_SSID`/`WIFI_PASSWORD`.
4. Chọn board **ESP32 Dev Module**, đúng cổng COM, Upload.
5. Mở Serial Monitor (baud 115200) — sẽ thấy dòng `Dashboard đang chạy tại: http://<IP>`.
6. Mở địa chỉ IP đó bằng trình duyệt (điện thoại/laptop cùng mạng WiFi) → thấy dashboard.

## Cách hoạt động

- Mặc định khởi động ở trạng thái **BẬT báo động**.
- Khi PIR phát hiện chuyển động: còi + đèn kêu/sáng 2 giây (không chặn vòng lặp — web server
  vẫn phản hồi bình thường trong lúc đó), sự kiện được lưu vào bộ nhớ (tối đa 20 sự kiện gần
  nhất, lưu dạng ring buffer trong RAM).
- Dashboard tự động `fetch()` API mỗi 2 giây để cập nhật trạng thái + danh sách sự kiện, có nút
  bật/tắt báo động gửi lệnh qua `POST /api/arm`.

## API (tự ESP32 phục vụ)

| Method | Endpoint        | Mô tả                                         |
| ------ | --------------- | ----------------------------------------------- |
| GET    | `/`           | Trang dashboard HTML                            |
| GET    | `/api/status` | `{"armed":true,"lastMotionSecAgo":12}`        |
| GET    | `/api/events` | `[{"secAgo":12},{"secAgo":340}, ...]`         |
| POST   | `/api/arm`    | Body`{"armed":true}` — bật/tắt báo động |
