#include "SparkFunBME280.h"
BME280 BME;
float hums;
float temps;
float pressure;

void setup(){
  Serial.begin(9600);
  delay(1000);
  BME.setI2CAddress(0x76);
  if (BME.beginI2C() == false) Serial.println("Sensor connect failed");

}

void loop(){ 
  hums = BME.readFloatHumidity();
  temps = BME.readTempC();
  pressure= BME.readFloatPressure();
  Serial.print("Humidity: ");
  Serial.print(hums, 2);
  Serial.print("%  Temp: ");
  Serial.print(temps, 2);
  Serial.print("°C  Press: ");
  Serial.print(pressure/100, 2);
   Serial.print(" hPa");
  Serial.println();
  delay(1000);
}
