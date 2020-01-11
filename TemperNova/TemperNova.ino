#include <U8g2lib.h>
#include <U8x8lib.h>

#include <SPI.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

void waitForConnection() {
  bool deviceConnected = false;
  
  while(!isConnected) {
    displayBluetoothLogo(500, 500);
    deviceConnected = isConnected();
  }
}

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  setupBluetooth();
}

void loop() {
  // put your main code here, to run repeatedly:
//  displayLoadingMessage();

  displayBluetoothLogo(0, 0);
  sendTempUpdate(80);
  displayTemp(80, true, true, true);  // int temp, bool showUnits, bool showDiff, bool showBluetoothLogo

  delay(3000);
  sendTempUpdate(60);
  displayTemp(60, true, true, true);
  
  delay(3000);
  sendTempUpdate(72);
  displayTemp(72, true, true, true);

  delay(3000);
}
