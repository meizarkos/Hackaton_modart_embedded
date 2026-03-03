#include "mqtt.h"
#include "wifiSide.h"

// MQTT broker IP and port
const char* mqtt_server = "helpother.fr";
const int mqtt_port = 1883;

// Create WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void sendMqttMessage(String topic, String payload) {
  if(client.connected() && isConnectedToWifi()){
    client.publish(topic.c_str(), payload.c_str(),false);
  }
}

void initMqttClient() {
  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!client.connected() && isConnectedToWifi()) {
    Serial.print("Attempting MQTT connection... ");

    String clientId = "ESP32-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);
    Serial.print(clientId);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
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