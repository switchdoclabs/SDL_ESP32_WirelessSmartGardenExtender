//
// Read moisture sensors
//

#include "OWMAdafruit_ADS1015.h"

Adafruit_ADS1015 ads1015;


// C1 - Grove Capacitive Moisture Sensor - SwitchDoc Labs

#define CAP0PERCENT 365
#define CAP100PERCENT 150

float scaleMoisture(int rawValue)
{
  float moisture_humidity = 0.0;

  rawValue = rawValue / 4; // Scale to 10 bits

  if (rawValue < CAP100PERCENT)
  {
    moisture_humidity = 100.0;
  }
  else
  {
    moisture_humidity = ((CAP0PERCENT - rawValue) * 100.0) / (CAP0PERCENT - CAP100PERCENT);
  }

  if (moisture_humidity < 0.0)
    moisture_humidity = 0.0;

  if (moisture_humidity > 100.0)
    moisture_humidity = 100.0;

  return moisture_humidity;



}


float reportMoisture(int rawValue)
{
  float moisture_humidity = 0.0;

  return scaleMoisture(rawValue);


}

void printSensors();


void readSensors()
{


  int i;
  for (i = 0; i < 4; i++)
  {
    if (moistureSensorEnable[i] == 1)
      writeGPIOBit(i, true);
    else
      writeGPIOBit(i, false);
  }


  xSemaphoreTake( xSemaphoreUseI2C,  portMAX_DELAY);
  int rawValue;
  for (i = 0; i < 4; i++)
  {
    if (moistureSensorEnable[i] == 1)
    {
      rawValue = ads1015.readADC_SingleEnded(i);
      moistureSensors[i] = scaleMoisture(rawValue);
#ifdef SGS2EXTDEBUG
      Serial.print("Channel:"); Serial.print(i); Serial.print(" Raw: "); Serial.println(rawValue);
#endif
    }

  }

  xSemaphoreGive( xSemaphoreUseI2C);
  for (i = 0; i < 4; i++)
  {
    writeGPIOBit(i, false);
  }


#ifdef SGS2EXTDEBUG
  printSensors();
#endif

}

void printSensors()
{
  bool printone;
  printone = false;
  int i;
  for (i = 0; i < 4; i++)
  {
    if (moistureSensorEnable[i] == 1)
    {
#ifdef SGS2EXTDEBUG
      Serial.print("Channel: "); Serial.print(i + 1); Serial.print("   Scaled:"); Serial.println (moistureSensors[i]);
#endif
      if (printone == false) printone = true;
    }


  }

  if (printone)
    Serial.println(" ");

}
