#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <HTTPClient.h>

extern bool isTryingToConnectToWifi;

bool isConnectedToWifi();
bool initWifi(String ssid, String password);

#endif