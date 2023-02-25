#include <WiFi.h>

#include "SPIFFS.h"

#include "bluetooth.h"
#include "jwt.h"

BluetoothService service;

void printFilesystem() {  
  File root = SPIFFS.open("/");

  if (!root) {
    Serial.println("error opening root filesystem");
  } else {
    File file = root.openNextFile();

    if (!file) {
      Serial.println("No files found");
    }

    while (file) {
      Serial.print("FILE: ");
      Serial.println(file.name());

      file = root.openNextFile();
    }
  }
}

void setup() {
  pinMode(16, OUTPUT);

  Serial.begin(115200);

  Serial.println("Starting up");

  service.init();

  Serial.println("Bluetooth service initialised");

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  printFilesystem();

  Jwt jwt;

  jwt.getJwt();
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

  delay(500);
}
