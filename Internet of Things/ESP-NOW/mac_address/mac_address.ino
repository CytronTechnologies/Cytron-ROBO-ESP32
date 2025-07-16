#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // Set to station mode
  Serial.println("ESP32 MAC Address:");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Nothing needed here
}
