#include <WiFi.h>
#include <HTTPClient.h>

#include "bluetooth.h"
#include "jwt.h"

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
    WiFiClient client;
    HTTPClient http;
    Jwt jwt;
    
    String token = jwt.getJwt();

    http.begin(client, "https://europe-west3-apps-353612.cloudfunctions.net/coffee");
    http.addHeader("Authorization", "Bearer " + token);
    Serial.println("Bearer " + token);
    int httpResponseCode = http.GET();

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      delay(60000);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(2000);
}
