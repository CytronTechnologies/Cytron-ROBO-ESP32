#include "EspMQTTClient.h"
#include <DHT.h>

#define DHTPIN 16      // DHT11 data pin
#define DHTTYPE DHT11  // DHT sensor type

DHT dht(DHTPIN, DHTTYPE);

EspMQTTClient client(
  "CytronVeryFastWiFi",         // WiFi SSID
  "CytronVerySecuredPassword",  // WiFi password
  "broker.mqttdashboard.com",   // MQTT Broker server
  "",                           // MQTT username (optional)
  "",                           // MQTT password (optional)
  "espclientID",                // Unique client ID
  1883                          // MQTT port
);

void setup() {
  Serial.begin(115200);
  dht.begin();                       // Start DHT sensor
  client.enableDebuggingMessages();  // Serial debug for MQTT
}

void onConnectionEstablished() {
  Serial.println("MQTT Connection established.");
}

unsigned long lastPublishTime = 0;

void loop() {
  client.loop();

  if (millis() - lastPublishTime > 2000) {
    lastPublishTime = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp)) {
      client.publish("esp32/temperature", String(temp));
      Serial.println("Temp: " + String(temp) + "°C");
    } else {
      Serial.println("Failed to read temperature");
    }

    if (!isnan(hum)) {
      client.publish("esp32/humidity", String(hum));
      Serial.println("Humidity: " + String(hum) + "%");
    } else {
      Serial.println("Failed to read humidity");
    }
  }
}
