#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

typedef struct struct_message {
  int gasValue;
} struct_message;

struct_message message;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Received Gas Value: ");
  Serial.println(message.gasValue);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Value:");
  lcd.setCursor(0, 1);
  lcd.print(message.gasValue);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0); 
  lcd.setCursor(0, 0);
  lcd.print("Waiting for data");
}

void loop() {

}
