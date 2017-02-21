#ifndef WIFI_UTIL_H
#define WIFI_UTIL_H

#include <WiFiEsp.h>
#include <WiFiEspUDP.h>
#include <WiFiEspServer.h>

#define WIFI_SERIAL Serial1
#define WIFI_BAUD 115200

void setupWiFi(char *ssid, char *password, int baud);
void printWifiStatus(Stream &stream);

#endif // WIFI_UTIL_H
