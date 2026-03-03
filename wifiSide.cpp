#include "wifiSide.h"

extern bool isTryingToConnectToWifi = false;

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
