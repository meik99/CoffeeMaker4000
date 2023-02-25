#define _DISABLE_TLS_

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <base64.h>

#include "bluetooth.h"
#include "credentials.h"

BluetoothService service;

void setup() {
  pinMode(16, OUTPUT);

  Serial.begin(115200);

  Serial.println("Starting up");

  service.init();

  Serial.println("Bluetooth service initialised");
}

void loop() {
  String bluetoothState = service.getCoffeeState();

  int status = WiFi.status();
  String ssid = service.getWifiSsid();
  String password = service.getWifiPassword();
  String wifiCommand = service.getWifiCommand();

  if (bluetoothState == COFFEE_RUN) {
    digitalWrite(16, HIGH);
    delay(500);
    digitalWrite(16, LOW);
    delay(500);

    service.setCoffeeIdle();
  }

  if (wifiCommand == WIFI_CONNECT) {
    if (status == WL_CONNECTED) {
      WiFi.disconnect();
    }

    WiFi.begin(ssid.c_str(), password.c_str());

    service.setWifiIdle();
  }

  if (status == WL_CONNECTED) {
    service.pause();

    WiFiClientSecure client;

    client.setTimeout(20000);
    client.setInsecure();

    int conn = client.connect("europe-west3-apps-353612.cloudfunctions.net", 443);

    if (conn == 1) {
      client.println("GET /coffee HTTP/1.1");
      client.print("Host: europe-west3-apps-353612.cloudfunctions.net");
      client.println("Connection: Close");
      client.print("Authorization: Basic ");
      client.println(base64::encode(FUNCTION_AUTH_HEADER));

      client.println();

      //Wait for server response
      while (client.available() == 0)
        ;

      //Print Server Response
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    } else {
      client.stop();
      Serial.println("Connection Failed");
    }


    service.resume();
  }

  delay(2000);
}
