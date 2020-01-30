#define ONE_WIRE_BUS 23 // plugged into IO 23

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;

int  resolution = 10;
unsigned long lastTempRequest = 0;
int  delayInMillis = 0;
float temperature = 0.0;

char *addr2str(DeviceAddress deviceAddress)
{
    static char return_me[18];
    static char *hex = "0123456789ABCDEF";
    uint8_t i, j;

    for (i=0, j=0; i<8; i++)
    {
         return_me[j++] = hex[deviceAddress[i] / 16];
         return_me[j++] = hex[deviceAddress[i] & 15];
    }
    return_me[j] = '\0';

    return (return_me);
}

void setupTempSensor() {
  sensors.begin();

  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  sensors.getAddress(tempDeviceAddress, 0);
  
  sensors.setResolution(tempDeviceAddress, resolution);
  Serial.print("Address is: ");      
  Serial.println(addr2str(tempDeviceAddress));
  
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  delayInMillis = 750 / (1 << (12 - resolution)); 
  lastTempRequest = millis(); 

  Serial.print("Delay is: ");      
  Serial.print(delayInMillis);
  Serial.print(" LastTempReq: ");
  Serial.println(lastTempRequest);
}

float getTemp() {
  if (millis() - lastTempRequest >= delayInMillis) // have to let the sensor cool down, will make sure we wait long enough before calling it again
  {
    sensors.setWaitForConversion(true);
    sensors.requestTemperatures();
    
    Serial.print(" Temperature: ");
    temperature = sensors.getTempCByIndex(0);
    Serial.println(temperature, resolution - 8);      
    Serial.println(); 
    
    delayInMillis = 750 / (1 << (12 - resolution));
    lastTempRequest = millis(); 
  } 
  // else we'll just return the last temp we got, it's most likely still valid anyways :P 
  
  delay(1);
  
  return temperature;
}

int getRoundedTemp() {
  float temp = getTemp();
  return round(temp);
}
