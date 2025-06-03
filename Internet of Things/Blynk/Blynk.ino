#define BLYNK_PRINT Serial

//Replace with your Blynk Template ID and Template Name
#define BLYNK_TEMPLATE_ID "TMPL6ncjOOsFY"
#define BLYNK_TEMPLATE_NAME "Robo ESP32 Temperature Monitoring System"
#define BLYNK_AUTH_TOKEN "yxTjYly3mMr-bNhd1Vbn4_9DcrndtQbZ"

//Replace with your WiFi SSID and password
#define WIFI_SSID "CytronVeryFastWifi"
#define WIFI_PASS "CytronVerySecurePassword"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

#define DHTPIN 32
#define DHTTYPE DHT11  //Using DHT11 sensor module
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void sendSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();


  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  |  Humidity: ");
  Serial.print(humidity);
  Serial.println("%");


  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
}


void setup() {
  Serial.begin(115200);
  delay(500);


  Serial.println("\n Initializing DHT Sensor...");
  dht.begin();


  Serial.print("Connecting to WiFi ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);


  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { 
    delay(500);
    Serial.print(".");
    attempts++;
  }


  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n Failed to connect to WiFi! Check credentials.");
    return;
  }


  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
