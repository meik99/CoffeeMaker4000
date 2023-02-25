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

    WiFiClientSecure client;
    HTTPClient http;
    
    client.setTimeout(20000);
    client.setInsecure();

    http.begin(client, "https://europe-west3-apps-353612.cloudfunctions.net/coffee");
    http.addHeader("Authorization", String("Basic ") + base64::encode(FUNCTION_AUTH_HEADER));

    int httpResponseCode = http.GET();

    if (httpResponseCode < 0) {
      Serial.println(http.errorToString(httpResponseCode));
    } else {
      Serial.println(http.getString());
    }

  }

  delay(2000);
}
