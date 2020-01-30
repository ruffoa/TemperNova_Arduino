int tempPin = A0;

int analogVal;
float R1 = 10000; // value of R1 on board
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor

void setupAnalogTemp() {
//  pinMode(tempPin, INPUT);
}

float getAnalogTemp() {
  analogVal = analogRead(tempPin);
  Serial.print("Read value: ");
  Serial.print(analogVal);

  float voltage = analogVal * 3.3;
  voltage /= 1024.0; 
  Serial.print(" Sketchy way: ");
  Serial.print(voltage);
  
  float temp, Vo;
  Vo=float(analogVal)*(3.3/4095);
  temp = log(10000*(3.3/Vo-1));
  temp = 1 / (c1 + (c2 + (c3 * temp * temp ))* temp );
  temp = temp - 273.15 + 47;// Convert Kelvin to Celcius and add fudge factor

  Serial.print(" Temperature: "); 
  Serial.print(temp);
  Serial.println(" C"); 
  
//  return temp;

  R2 = R1 * (1023.0 / (float)(analogVal - 1.0)); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  T = T - 273.15; //convert Kelvin to Celcius

  Serial.print(" Temperature: "); 
  Serial.print(T);
  Serial.println(" C"); 
//  return T;

//  return voltage + 20;  // super sketch!!
  return temp + 8;
}

int getRoundedAnalogTemp() {
  float temp = getAnalogTemp();
  return round(temp);
}
