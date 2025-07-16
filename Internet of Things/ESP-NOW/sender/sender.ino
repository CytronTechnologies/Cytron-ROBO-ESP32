#include <WiFi.h>
#include <esp_now.h>

#define MQ2_PIN 36  // Analog input pin connected to MQ2

// MAC address of receiver ESP32 (replace with actual MAC)
uint8_t receiverAddress[] = { 0x14, 0x33, 0x5C, 0x02, 0xC0, 0xFC };

typedef struct struct_message {
  int gasValue;
} struct_message;

struct_message message;

void OnSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());  

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnSent);
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  int gasReading = analogRead(MQ2_PIN);
  message.gasValue = gasReading;
  esp_now_send(receiverAddress, (uint8_t *)&message, sizeof(message));
  delay(1000);  // Send every 1s
}
