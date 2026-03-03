const int lm35Pin = 32;
const int resistPin = 34;
const String idOfEmbedded = "abcd";

const String wifiSSID = "Campus-GES";
const String wifiPassword = "R3s3@u-G3S";

#include "mqtt.h"
#include "wifiSide.h"
#include "temp.h"

void setup() {
  Serial.begin(9600);

  // Configuration ADC ESP32
  analogReadResolution(12);                 // 0–4095
  analogSetPinAttenuation(lm35Pin, ADC_11db); // ~0–3.3V
  analogSetPinAttenuation(resistPin, ADC_11db);
  initWifi(wifiSSID, wifiPassword);
  initMqttClient();
}

void loop() {
  keepAliveMqtt();
  float temperatureC = readTemperature(lm35Pin);
  //float temperatureC = 20;

  int resistValue = analogRead(resistPin);
  /**Serial.print("Bande resistante :");
  Serial.println(resistValue);**/

  sendMqttMessage("temperature/"+ idOfEmbedded, "{\"temp\":\"" + String(temperatureC, 1) + "\"}");
  sendMqttMessage("sizeBust/"+ idOfEmbedded, "{\"sizeBust\":\"" + String(resistValue) + "\"}");

  delay(1000);
}