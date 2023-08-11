#ifndef BLE_H__
#define BLE_H__

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "645169ec-98e5-49ae-a04e-4690fc92a834"
#define CHARACTERISTIC_UUID "d8a7ce73-3cc2-4b51-a1a6-ce35438e2acc"

namespace BLE_System {
  bool refreshed = false;
  BLEServer* pServer = NULL;
  BLECharacteristic* pCharacteristic = NULL;
  BLEService* pService;
  BLEAdvertising* pAdvertising;
  bool deviceConnected = false;
  bool justDisconnected = true;
  uint32_t lastConnection;
  class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Device connected");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Device disconnected");
      deviceConnected = false;
      justDisconnected = true;
    }
  };

  class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        // json.set(rxValue.c_str());
      }
    }
    void onRead(BLECharacteristic* pCharacteristic) {
      Serial.print("aaaReceived Value: ");
    }
  };
  void restart();
  void end();
  void checkConnection();
  void write(String );
  void begin();

  void restart() {
    end();
    begin();
  }

  void end() {
    BLEDevice::deinit(false);
  }
  void checkConnection() {
    if (justDisconnected) {
      restart();
    }
  }

  void write(String data) {
    pCharacteristic->setValue((uint8_t*)data.c_str(), 4);
    pCharacteristic->notify();
  }

  void begin() {
    justDisconnected = false;
    lastConnection = millis();
    String BT = "SMARTSHUTTER";
    BLEDevice::init(BT.c_str());

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->setValue("Hello from SD Smart");
    pService->start();
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
  }
};
#endif