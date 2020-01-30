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

BLEServer* pServer = NULL;        // the BluetoothLE server
BLECharacteristic* pCharacteristic = NULL;  // the BluetoothLE characteristic.  This holds data and some information about the service

bool deviceConnected = false;     // do we have a device currently connected?
bool oldDeviceConnected = false;  // true if a device WAS connected, but has now disconnected
uint32_t value = 0; // holds the raw value returned from the Bluetooth characteristic, aka raw data from the app
int targetTemp = 0; // holds our larget temp, aka what has been set in the app

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  // the "unique" id of the service, this can be whatever, and is random.  Preset values exist for common services - https://www.bluetooth.com/specifications/gatt/services/ 
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"  // the unique characteristic id -> this does actually have to be different from all others on the same service

class BluetoothServerCallbacks: public BLEServerCallbacks { // a collection of callbacks used in the Bluetooth server
    void onConnect(BLEServer* pServer) {    // on device connected...
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) { // on device disconnected
      deviceConnected = false;
      targetTemp = 0;
    }
};

class BluetoothCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {  // on information recieved from the ble characteristic from the connected device
      std::string value = pCharacteristic->getValue();  // get the raw data from the Bluetooth connection
      String valueStr = "";
      
      if (value.length() > 0) {
        Serial.print("New Bluetooth value: ");
        for (int i = 0; i < value.length(); i++) {
          Serial.print(value[i]);
          valueStr+= value[i];
        }
          
        Serial.println();
        int temp = valueStr.toInt();
        if (temp != 0) {
          targetTemp = temp;
        }
      }
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

  pCharacteristic->setCallbacks(new BluetoothCharacteristicCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setValue("69");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Starting to advertise connection...");
}

bool isConnected() {
  return deviceConnected;  
}

void sendTempUpdate(int newTemp) {
  // check to see if device is connected...
  if (deviceConnected) {
    char txString[8]; // make sure this is big enuff
    itoa(newTemp, txString, 10);
    double temp = (double)newTemp;

    pCharacteristic->setValue(txString);

    pCharacteristic->notify();
    delay(3);
  }
}

int getTargetTemp() {
  return targetTemp;
}
