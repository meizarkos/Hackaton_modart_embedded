#include "mqtt.h"
#include "wifiSide.h"
#include "globale.h"
#include <ESP32Servo.h>
#include <ArduinoJson.h>
// MQTT broker IP and port
const char* mqtt_server = "helpother.fr";
const int mqtt_port = 1883;

int servoState = 0;
const int tightenStep = 25;
Servo myServo;

// Create WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void sendMqttMessage(String topic, String payload) {
  if(client.connected() && isConnectedToWifi()){
    client.publish(topic.c_str(), payload.c_str(),false);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr(topic);
  String payloadStr;
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  Serial.print("Message reçu sur le topic: ");
  Serial.print(topicStr);
  Serial.print(" avec le payload: ");
  Serial.println(payloadStr);

  if(topicStr == "servo/night/" + idOfEmbedded) {
    servoState = 0;
    myServo.write(servoState);
    Serial.println("Action: Night Mode");
  } else if (topicStr == "servo/normal/" + idOfEmbedded) {
    servoState = 90;
    myServo.write(servoState);
    Serial.println("Action: Normal Mode");
  } else if (topicStr == "servo/pushup/" + idOfEmbedded) {
    servoState = 180; // Position for push-up mode
    myServo.write(servoState);
    Serial.println("Action: Push-up Mode");
  } else

  if (topicStr == "servo/tighten/" + idOfEmbedded) {
    if(servoState + tightenStep <= 180) {
      servoState += tightenStep;
    } else {
      servoState = 180;
    }
    myServo.write(servoState); // Tighten
    Serial.println("Action: Tighten");
  } else if (topicStr == "servo/loosen/" + idOfEmbedded) {
    if(servoState - tightenStep >= 0) {
      servoState -= tightenStep;
    } else {
      servoState = 0;
    }
    myServo.write(servoState); // Loosen
    Serial.println("Action: Loosen");
  }
}

void initMqttClient() {
  myServo.attach(servoPin);
  myServo.write(servoState);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected() && isConnectedToWifi()) {
    Serial.print("Attempting MQTT connection... ");

    String clientId = "ESP32-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);
    Serial.print(clientId);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(("servo/tighten/" + idOfEmbedded).c_str());
      client.subscribe(("servo/loosen/" + idOfEmbedded).c_str());
      client.subscribe(("servo/night/" + idOfEmbedded).c_str());
      client.subscribe(("servo/normal/" + idOfEmbedded).c_str());
      client.subscribe(("servo/pushup/" + idOfEmbedded).c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void keepAliveMqtt() {
  if (!client.connected() && isConnectedToWifi()) {
    reconnect();
  }
  client.loop();
}