#include <OneWire.h>  // temp sensor stuff
#include <DallasTemperature.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

void waitForConnection() {
  bool deviceConnected = false;
  
  while(!deviceConnected) {
//    displayBluetoothLogo(500, 500);
//    deviceConnected = isConnected();
      deviceConnected = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();         // setup the OLED display
  setupBluetooth();     // setup the BluetoothLE connection
  waitForConnection();  // wait for a device to be connected (might not want this, idk)
  setupTempSensor();    // setup the temp sensor
  setupTecs();
}

void loop() {
  // put your main code here, to run repeatedly:

  int temp = getRoundedTemp();  // round the temp first to an int, might want a 1 decimal float but idk
  sendTempUpdate(temp);         // send new temp to connected BluetoothLE device, if applicable
  displayTemp(temp, true, true, true);  // int temp, bool showUnits, bool showDiff, bool showBluetoothLogo
  controlTecs();
  
  delay(50);  // wait a little, in order to not spam the Bluetooth connection (and because temps won't change that quickly)
} 
