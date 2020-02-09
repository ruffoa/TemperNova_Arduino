#define ONE_WIRE_BUS 26 // plugged into IO 26 - https://lh3.googleusercontent.com/masktxO3sTpWCfj9ZQ6AHwEMPhXcx7WANLA63M-qYiTZEPTFEk9DF9K3AzHy0Ct8meEjXf3tZRvrLleTd1yB7FUrbIVHUr_tclueBALq_lZCm-X1hLJjTorYjqZYZkAA_BvucyH0SFHNf97xfxz8Cy7Gqc938HxB6xBof48jTu4l2oRNMuT9PhUWLo04FRA6sb7VNJlc-NNwVQpcfeEJaqaKXL87BXz3Ir73aO776YeNlFlqrdksBnQFweJN4BZTNqZD1kRika4iWz-ZAzIkBF_rBqZ6m3bgSzeL5ZqbYbBWBYkXn9UV2skoV6qk4xF7Zj8DnV96rCIsMG4W4cTo5XGEJEibo6ty7yeuVss0GvJ0tHE0-E7wGtzA1sMIIgS-9A7NvOPfX-iZdqPjIe6wVb6sXdAbkQgu3t0uY5XdR6r8e7wSROKbtQbN2TBrQnIlXzS8IvyCEMvweZU-p1geAANlX91tuvfnmIFjXEfhcLCctHzZSlRQI80J5cJaV-y08Rs6AKLWDhHi7dVjf38SCSlKrMjQ9cwKMLJNCB577ftwh1CVauK4BrbJOBLtGEv00e0VRwaCZ7GQN9szM2gQ0xqG7Apm2pcoiVb1uK3i40SZ7LDLbbaLadDf1gIyexdZvnxT_iYYeB7d5YwnzT7euFqleCs-I0900cB2bktALSlEOCLwSCGIOc4=w984-h1312-no
#define TECS_PIN 35
#define TEMP_CONTROL_PIN 33
#define SWAP_COUNT 500

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;

int  resolution = 10;
unsigned long lastTempRequest = 0;
int  delayInMillis = 0;
float temperature = 0.0;
bool lastOn = false;
bool wasCold = false;
int count = 520;

char *addr2str(DeviceAddress deviceAddress) // translates the device address to a string, used for debugging
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

void setupTecs() {
  pinMode(TECS_PIN, OUTPUT); 
  pinMode(TECS_PIN, OUTPUT); 
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

void controlTecs() {
  int target = getTargetTemp();
  
  if (temperature > target) {
    // send cooldown command
    digitalWrite(TEMP_CONTROL_PIN, LOW);
    Serial.print("Sending cooldown command to the TECS ");
  } else if (temperature < target) {
    // send heat command
    digitalWrite(TEMP_CONTROL_PIN, HIGH);
    Serial.print("Sending heat-up command to the TECS ");
  } else {
    if (wasCold) {
      digitalWrite(TEMP_CONTROL_PIN, HIGH);
      Serial.print("Sending heating command to the TECS (it's currently at temp though) ");
    } else {
      digitalWrite(TEMP_CONTROL_PIN, LOW);
      Serial.print("Sending cooldown command to the TECS (it's currently at temp though) ");
    }
  }

  if (count <= 0) {
    wasCold = !wasCold;
    
    count = SWAP_COUNT;
    
    lastOn = !lastOn;
    Serial.println("Hit the 500 cycles, swapping which TEC, and heat / cool state for neutral");
  } 
  
  // turn on and off to keep it at temp
  if (lastOn) {
      digitalWrite(TECS_PIN, LOW);
      Serial.println("Using TEC 1 (PIN LOW)");
  } else {
      digitalWrite(TECS_PIN, HIGH);
      Serial.println("Using TEC 2 (PIN HIGH)");
  }
  
  count--;
}
