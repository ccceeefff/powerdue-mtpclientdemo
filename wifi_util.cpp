#include "wifi_util.h"

#include <Arduino.h>
#include <IPAddress.h>

#include "debug.h"


void setupWiFi(char *ssid, char *pass, int baud){
  int status = WL_IDLE_STATUS;     // the Wifi radio's status

  WIFI_SERIAL.begin(baud);
  
  // initialize ESP module
  WiFi.init(&WIFI_SERIAL);
  
  // reset the ESP module
  WiFi.reset();

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    PRINT_DEBUGLN("WiFi shield not present");
    // don't continue
    while (true);
  }
  
  // disconnect from any previous networks
  WiFi.disconnect();

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED ) {
    PRINT_DEBUG("Attempting to connect to WPA SSID: ");
    PRINT_DEBUGLN(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  PRINT_DEBUGLN("You're connected to the network");
}

void printWifiStatus(Stream &stream)
{
  // print the SSID of the network you're attached to
  stream.print("SSID: ");
  stream.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  stream.print("IP Address: ");
  stream.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  stream.print("Signal strength (RSSI):");
  stream.print(rssi);
  stream.println(" dBm");
}
