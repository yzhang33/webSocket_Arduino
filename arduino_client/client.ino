#include <WiFiNINA.h>       // use this for MKR1010 and Nano 33 IoT boards
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"

WiFiClient netSocket;               // network socket to server
const char server[] = "10.23.10.233";  // server name
String route = "/msg";              // API route
// request timestamp in ms:
long lastRequest = 0;
// interval between requests:
int interval = 10000;

void setup() {
  Serial.begin(9600);               // initialize serial communication
  while (!Serial);        // wait for serial monitor to open

  // while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);           // print the network name (SSID)
    WiFi.begin(SECRET_SSID, SECRET_PASS);         // try to connect
    delay(2000);
  }

  // When you're connected, print out the device's network status:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  if (millis() - lastRequest > interval ) {

    Serial.println("making request");
    HttpClient http(netSocket, server, 3000);      // make an HTTP client
    http.get(route);  // make a GET request

    while (http.connected()) {       // while connected to the server,
      if (http.available()) {        // if there is a response from the server,
        String result = http.readStringUntil('\n');  // read it
        int idx = result.indexOf("^");
        String res = result.substring(result.length() - 8,result.length());
        Serial.print(res);               // and print it
      }
    }
    //  // when there's nothing left to the response,
    http.stop();                     // close the request
    lastRequest = millis();
  }
}