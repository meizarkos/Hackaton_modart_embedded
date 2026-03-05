#include "wifiSide.h"
#include "sensor.h"
#include "globale.h"
#include <ArduinoJson.h>

extern bool isTryingToConnectToWifi = false;
String url = "http://localhost:3000/api/";

bool isConnectedToWifi(){
  return WiFi.status() == WL_CONNECTED;
}

bool initWifi(String ssid, String password){
  int attempts = 0;
  WiFi.begin(ssid, password);
  isTryingToConnectToWifi = true;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    delay(1000);
    attempts++;
    Serial.println(F("...."));
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("Connecté au WiFi"));
    isTryingToConnectToWifi = false;
    return true;
  } else {
    Serial.println(F("Échec de connexion au WiFi après 30 secondes."));
    isTryingToConnectToWifi = false;
    return false;
  }
}

void getSizeBustInitial(String id) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String apiUrl = url + "sizeBust/manual";
    http.begin(apiUrl);       
    int httpResponseCode = http.GET(); 

    if (httpResponseCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
          Serial.println("JSON parsing failed");
          return;
      }
      sizeInCmInitial = doc["sizeBust"];
      getAnalogValueFirstSize();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Payload: " + payload);
    } else {
      Serial.println("Error on HTTP request: " + String(httpResponseCode));
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}
