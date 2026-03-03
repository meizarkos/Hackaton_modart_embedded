#ifndef MQTT_H
#define MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>

void initMqttClient();
void sendMqttMessage(String topic, String payload);
void keepAliveMqtt();

#endif
