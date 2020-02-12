// sda - 21
// scl - 22

void displayBluetoothLogo(int time, int del) {  // displays the Bluetooth logo on the OLED display
  u8g2.clearBuffer();         // clear the internal memory
  if (del && del > 0) {
    u8g2.sendBuffer();         // transfer internal memory to the display
    delay(del);
  }

  u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(0, 31, "\x004A"); // write something to the internal memory
  u8g2.sendBuffer();         // transfer internal memory to the display
  delay(time);
}

String convertToString(char* a, int size) // converts a char array to a string
{ 
    int i; 
    String s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
} 

void displayTemp(int temp, bool showUnits, bool showDiff, bool showBluetoothLogo) { // displays the current temp to the OLED display. Has a few options, but it basically always called with SHowUnits as true
  if (!temp) {
    return;
  }

  u8g2.clearBuffer();         // clear the internal memory

  // Write the temp to the buffer
  u8g2.setFont(u8g2_font_logisoso22_tf);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  
  char tempStr[4]; // make sure this is big enuff
  itoa(temp, tempStr, 10);
  
  if (showUnits) {  // show the "degree" symbol on screen
    u8g2.drawStr(63, 30, "\xb0"); // write something to the internal memory
  }
  
  u8g2.drawStr(38, 30, tempStr); // write something to the internal memory
  

  if (showDiff) { // show an arror pointing in the direction of change (eg: down if the drink is hotter than the temp set in the app). Will only have an effect after the app is connected
    int targetTemp = getTargetTemp();
    
    u8g2.setFont(u8g2_font_open_iconic_arrow_4x_t);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall

    if (targetTemp != 0) {
      if (targetTemp > temp)
        u8g2.drawStr(90, 31, "\x004B"); // write something to the internal memory
      else if (targetTemp < temp) {
        u8g2.drawStr(90, 31, "\x0048"); // write something to the internal memory
      }
    }
  }
  
  if (showBluetoothLogo && isConnected()) { // show the Bluetooth logo on screen. This is flag in the call statement is true when the app is connected via Bluetooth LE
    u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.drawStr(0, 31, "\x004A"); // write something to the internal memory
  }

  u8g2.sendBuffer();        // transfer internal memory to the display
  delay(5);                 // add a small delay
}
