const int lm35Pin = 32;
const int resistPin = 34;
const int cardiacPin = 33;
const int accSDA = 27;
const int accSCL = 26;
unsigned long lastMQTTTime = 0;
const unsigned long mqttInterval = 1000;

const String wifiSSID = "Campus-GES";
const String wifiPassword = "R3s3@u-G3S";

#include "mqtt.h"
#include "wifiSide.h"
#include "sensor.h"
#include "globale.h"
#include <Wire.h>


MPU6050 mpu;
TaskHandle_t cardiacTaskHandle = NULL;

void setup() {
  Serial.begin(9600);
  Wire.begin(accSDA, accSCL);
  mpu.initialize();
    if (mpu.testConnection()) {
    Serial.println("MPU6050 connecté !");
  } else {
    Serial.println("Erreur connexion MPU6050 !");
  }

  // Configuration ADC ESP32
  analogReadResolution(12);                 // 0–4095
  analogSetPinAttenuation(lm35Pin, ADC_11db); // ~0–3.3V
  analogSetPinAttenuation(resistPin, ADC_11db);
  analogSetPinAttenuation(cardiacPin, ADC_11db);
  initWifi(wifiSSID, wifiPassword);
  initMqttClient();

  xTaskCreatePinnedToCore(
    cardiacTask,            // Function name
    "CardiacTask",          // Task name
    4096,                   // Stack size (bigger for safety)
    NULL,
    1,
    &cardiacTaskHandle,
    1                       // Core 1 (good choice)
  );
}

void loop() {
  keepAliveMqtt();
  float magnitude = accelerometerMagnitude(mpu);

  float temperatureC = readTemperature(lm35Pin) + 13.0;
  int resistValue = analogRead(resistPin);

  unsigned long now = millis();
  if (now - lastMQTTTime >= mqttInterval) {
    lastMQTTTime = now;

    sendMqttMessage("temperature/"+ idOfEmbedded, "{\"temp\":\"" + String(temperatureC, 1) + "\"}");
    sendMqttMessage("sizeBust/"+ idOfEmbedded, "{\"sizeBust\":\"" + String(resistValue) + "\"}");
    sendMqttMessage("cardiac/"+ idOfEmbedded, "{\"cardiac\":\"" + String(globalBPM, 1) + "\"}");
    sendMqttMessage("accel/"+ idOfEmbedded, "{\"accel\":\"" + String(magnitude, 1) + "\"}");
  }
}