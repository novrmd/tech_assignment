#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHT_PIN 4  // GPIO pin connected to the DHT sensor
#define DHT_TYPE DHT11  // Specify the type of DHT sensor

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "nun kochi";  // Your WiFi SSID
const char* password = "torojoyo";  // Your WiFi password
const char* serverUrl = "http://192.168.43.20:5000/data";  // URL of the server to send data

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  // Read data from the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Check if the readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Prepare the JSON payload
  String jsonPayload = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";

  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient httpClient;
    httpClient.begin(serverUrl);
    httpClient.addHeader("Content-Type", "application/json");

    // Send the HTTP POST request
    int httpResponseCode = httpClient.POST(jsonPayload);
    
    // Print the response from the server
    if (httpResponseCode > 0) {
      String response = httpClient.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }
    
    // End the HTTP request
    httpClient.end();
  } else {
    Serial.println("WiFi connection error");
  }

  // Wait 10 seconds before sending the next request
  delay(10000);
}
