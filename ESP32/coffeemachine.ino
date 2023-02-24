#include <WiFi.h>
#include "bluetooth.h"

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

  Serial.print("WiFi: ");
  Serial.print(ssid);
  Serial.print(" ");
  Serial.println(password);
  Serial.printf("Status: %s\n", WiFi.status() == WL_CONNECTED ? "connected" : "not connected");

  delay(500);
}
