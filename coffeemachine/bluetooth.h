#ifndef BLUETOOTH_SERVICE_H
#define BLUETOOTH_SERVICE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "credentials.h"

#define COFFEE_SERVICE_UUID "9958874b-4898-4978-ae30-5d8d3a1f793b"
#define COFFEE_CHARACTERISTIC_UUID "07f00547-3d43-497f-a899-f3b978362793"

#define WIFI_SERVICE_UUID "3b9e0ecb-1e4c-4f0e-99a7-4e0dbfbc975c"
#define WIFI_SSID_UUID "22c745b4-6764-4b8c-bf47-32c3341fdb74"
#define WIFI_PASSWORD_UUID "5a961f11-efff-4265-ba45-68f9a36d98eb"
#define WIFI_COMMAND_UUID "fb80a1d2-8c94-4583-814c-f5546cb4219c"

#define DEVICE_NAME "COFFEE MAKER 4000"

#define COFFEE_IDLE "IDLE"
#define COFFEE_RUN "RUN"

#define WIFI_IDLE "IDLE"
#define WIFI_CONNECT "CONNECT"

class BluetoothService {
private:
  BLEServer *server;

  BLEService *coffeeService;
  BLEService *wifiService;

  BLECharacteristic *coffeeRunCharacteristic;
  BLECharacteristic *wifiSsidCharacteristic;
  BLECharacteristic *wifiPasswordCharacteristic;
  BLECharacteristic *wifiCommandCharacteristic;


  BLEAdvertising *advertising;

  void startAdvertising() {
    advertising = BLEDevice::getAdvertising();

    advertising->addServiceUUID(COFFEE_SERVICE_UUID);
    advertising->addServiceUUID(WIFI_SERVICE_UUID);
    advertising->setScanResponse(true);

    // functions that help with iPhone connections issue
    advertising->setMinPreferred(0x06);
    advertising->setMinPreferred(0x12);

    BLEDevice::startAdvertising();
  }

  void setupCoffeeService() {
    coffeeService = server->createService(COFFEE_SERVICE_UUID);
    coffeeRunCharacteristic = coffeeService->createCharacteristic(
      COFFEE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    coffeeRunCharacteristic->setValue(COFFEE_IDLE);
    coffeeService->start();
  }

  void setupWifiService() {
    wifiService = server->createService(WIFI_SERVICE_UUID);
    wifiSsidCharacteristic = wifiService->createCharacteristic(
      WIFI_SSID_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    wifiPasswordCharacteristic = wifiService->createCharacteristic(
      WIFI_PASSWORD_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    wifiCommandCharacteristic = wifiService->createCharacteristic(
      WIFI_COMMAND_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    #ifdef WIFI_SSID
    wifiSsidCharacteristic->setValue(WIFI_SSID);
    #endif
    #ifdef WIFI_PASSWORD
    wifiPasswordCharacteristic->setValue(WIFI_PASSWORD);
    wifiCommandCharacteristic->setValue(WIFI_CONNECT);
    #else
    wifiCommandCharacteristic->setValue(WIFI_IDLE);
    #endif

    wifiService->start();
  }

public:
  void init() {
    BLEDevice::setMTU(517);
    BLEDevice::init(DEVICE_NAME);
    server = BLEDevice::createServer();

    setupCoffeeService();
    setupWifiService();

    startAdvertising();
  }

  void setCoffeeRun() {
    coffeeRunCharacteristic->setValue(COFFEE_RUN);
  }

  void setCoffeeIdle() {
    coffeeRunCharacteristic->setValue(COFFEE_IDLE);
  }

  String getCoffeeState() {
    return String(coffeeRunCharacteristic->getValue().c_str());
  }

  String getWifiCommand() {
    return String(wifiCommandCharacteristic->getValue().c_str());
  }

  String getWifiSsid() {
    return String(wifiSsidCharacteristic->getValue().c_str());
  }

  String getWifiPassword() {
    return String(wifiPasswordCharacteristic->getValue().c_str());
  }

  void setWifiIdle() {
    wifiCommandCharacteristic->setValue(WIFI_IDLE);
  }

};

#endif