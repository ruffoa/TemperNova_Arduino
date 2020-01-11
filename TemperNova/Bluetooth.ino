/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

// more help from https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/examples/BLE_notify/BLE_notify.ino

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
int targetTemp = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class BluetoothServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setupBluetooth() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("TemperNova Mug");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BluetoothServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setValue("TEMP: 69, UP");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

bool isConnected() {
  return deviceConnected;  
}

void sendTempUpdate(int newTemp) {
  // check to see if device is connected...
  if (deviceConnected) {
    String message = "TEMP: " + String(newTemp) + ", UP";
//    std::string msg = "TEMP: "  + std::string(newTemp) + ", UP";
    pCharacteristic->setValue("TEMP: 80, UP");

    char txString[8]; // make sure this is big enuffz
    itoa(newTemp, txString, 10);
    double temp = (double)newTemp;
//    dtostrf(temp, 1, 2, txString);
//    pCharacteristic->setValue(&temp, 1);
    pCharacteristic->setValue(txString);

    pCharacteristic->notify();
    delay(3);
  }
}
