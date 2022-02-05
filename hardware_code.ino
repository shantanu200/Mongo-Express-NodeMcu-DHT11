#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define DHTPIN D1     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
const char *ssid = "*********"; 
const char *password = "*******"; 
const char *server_url = "http://192.168.0.106:5000/sensordata/";
StaticJsonDocument <256> jsonBuffer;

WiFiClient client;
HTTPClient http;
void setup() {
   delay(3000);
   Serial.begin(9600);
   dht.begin();
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
   Serial.println("WiFi connected");
   delay(1000);
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();         
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("Temperature = ");
  Serial.print(t); 
  Serial.println("C  ");
 JsonObject values = jsonBuffer.createNestedObject();

  values["humidity"] = h;
  values["temperature"] = t;
  char json_str[100];

   http.begin(client, server_url);
    http.addHeader("Content-Type", "application/json");
    serializeJsonPretty(values,json_str);
    Serial.println(json_str);
    int httpCode = http.POST(json_str);
    if(httpCode > 0){
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.print("Response: ");Serial.println(payload);
        }
    }else{
         Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
    }
    http.end();
    delay(5000);
}
