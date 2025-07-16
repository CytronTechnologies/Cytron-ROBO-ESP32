#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 16  // GPIO where DHT11 is connected
#define DHTTYPE DHT11

//Replace with your WiFi SSID and password
const char* ssid = "CytronVeryFastWifi";
const char* password = "CytronVerySecureWifi";
const char* thingSpeakAPI = "https://api.thingspeak.com/update?api_key=YOUR_API_KEY";  //Replace with your API Key

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}


void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();


  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");


  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(thingSpeakAPI) + "&field1=" + String(temperature) + "&field2=" + String(humidity);

    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("Data sent. HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending data: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }


  delay(15000);  // Send data every 15 seconds
}
