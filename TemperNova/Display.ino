void displayLoadingMessage() {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso16_tr );  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(8, 30, "Please"); // write something to the internal memory
  u8g2.sendBuffer();         // transfer internal memory to the display
  delay(3000);

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso16_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(29, 30, "wait"); // write something to the internal memory
  u8g2.sendBuffer();         // transfer internal memory to the display
  delay(800);

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso16_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(10, 30, "loading"); // write something to the internal memory
  u8g2.sendBuffer();         // transfer internal memory to the display
  delay(800);

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso16_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(0, 30, "bluetooth"); // write something to the internal memory
  u8g2.sendBuffer();         // transfer internal memory to the display
  delay(1000);
}

void displayBluetoothLogo(int time, int del) {
  u8g2.clearBuffer();         // clear the internal memory
  if (del) {
      u8g2.sendBuffer();         // transfer internal memory to the display
      delay(del);
  }
  
  u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(0, 31, "\x004A"); // write something to the internal memory
  u8g2.sendBuffer();         // transfer internal memory to the display
  delay(time);
}
