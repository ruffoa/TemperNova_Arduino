//// Lib from this helpful guy: https://awtfy.com/2010/11/09/an-arduino-library-for-the-adt7310-spi-temperature-sensor/
//
//// hardcoded library pins to be 
//// DOUT - 23 - MOSI
//// DIN - 19 - MISO
//// CLK - 25
//
//#define SELPIN 26            //Selection Pin  - to chip pin 4 (CS-)
//#define MUST_BE_OUTPUT 16    // This pin must be set to output for Hardware SPI to work, even if we don't use it.
//
//ADT7310 adt7310(SELPIN); 
//unsigned int ADTcheck = 0; 
//
//void setupTempSensor() {
//  Serial.println("\nSetting up temp sensor..."); 
//    
//  pinMode(SELPIN, OUTPUT); 
//  pinMode(MUST_BE_OUTPUT,OUTPUT); 
//
//  adt7310.init(); 
//  adt7310.reset(); 
//
//  Serial.print("Writing config: "); 
//  adt7310.setmode(ADT7310_1FAULT | ADT7310_CT_POLARITY_LOW | ADT7310_INT_POLARITY_LOW | 
//    ADT7310_INTCT_INTERRUPT | ADT7310_CONTINUOUS | ADT7310_16BIT); 
//  Serial.println(" done"); 
//
//  // On powerup, register 0x04 Tcrit should be value 0x4980
//  Serial.print("reading Tcrit: "); 
//  unsigned int value = adt7310.read(0x04, 16);
//  Serial.print(" Read complete [0x");
//  Serial.print(value,HEX);
//  Serial.println("]");
//  // Save this value for later to compare against to make sure communications still synchronized.  
//  ADTcheck=value;     
//  delay(240);   // First conversion takes ~ 240 ms 
//}
//
//float checkTemp() {
//  unsigned int value = 0; 
//  float t = 0; 
//
//  value=0;
//  // Read the 16-bit temperature value from register 0x02
//  value = adt7310.read(0x02, 16);
//  // Convert to a floating point temperature in C
//  t=adt7310.temperature(value,16); 
//
//  Serial.print("Temperature: "); 
//  Serial.print(t,3); 
//  Serial.print(" C, [0x"); 
//  Serial.print(value,HEX); 
//  Serial.print("], [0b"); 
//  Serial.print(value,BIN); 
//  Serial.println("]"); 
//
//  // Read Tcrit register 0x04 again, if the communications are still good
//  // then should return the value read at the start, 0x4980. 
//  value = adt7310.read(0x04, 16);
//  
//  if (ADTcheck != value) {
//    Serial.print(" ERROR expect [0x"); 
//    Serial.print(ADTcheck,HEX);
//    Serial.print(" - 0b"); 
//    Serial.print(ADTcheck,BIN); 
//    Serial.print("] got [0x"); 
//    Serial.print(value,HEX); 
//    Serial.print(" - 0b"); 
//    Serial.print(value,BIN); 
//    Serial.println("] - reset"); 
//    // No choice, reset chip to get communications back. 
//    adt7310.reset(); 
//    adt7310.setmode(ADT7310_1FAULT | ADT7310_CT_POLARITY_LOW | ADT7310_INT_POLARITY_LOW | 
//      ADT7310_INTCT_INTERRUPT | ADT7310_CONTINUOUS | ADT7310_16BIT); 
//
//    return NULL;
//  }
//
//  return t; // life seems to be a-ok atm, so return our temp value
//}
//
//int getRoundedTemp() {
//  float temp = checkTemp();
//  return round(temp);
//}
