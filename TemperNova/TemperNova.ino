#include <U8g2lib.h>
#include <U8x8lib.h>

#include <SPI.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  setupBluetooth();
}

void loop() {
  // put your main code here, to run repeatedly:
//  displayLoadingMessage();
  displayBluetoothLogo(500, 500);
}
