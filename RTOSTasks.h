// RTOS Tasks for various modes and periphials

#include "evaluateValves.h"
/*

  void taskSingleBlink( void * parameter)
  {
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreSingleBlink ) > 0)
    {

      blinkLED(Blue, 300);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);

  }

  }
*/

void taskReadSensors( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreReadSensor ) > 0)
    {
      xSemaphoreTake( xSemaphoreSensorsBeingRead, 10000);
      readSensors();
      xSemaphoreGive( xSemaphoreSensorsBeingRead);
      sendMQTT(MQTTSENSORS, "");
    }
    //vTaskDelay(600000 / portTICK_PERIOD_MS);
    vTaskDelay(300000 / portTICK_PERIOD_MS);
  }

}


void taskSetValves( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreEvaluateValves ) > 0)
    {
      xSemaphoreTake( xSemaphoreEvaluatingValves, 1000);
      xSemaphoreTake( xSemaphoreSensorsBeingRead, 1000);
      evaluateValves(100);
      xSemaphoreGive( xSemaphoreEvaluatingValves);
      //vTaskDelay(50 / portTICK_PERIOD_MS);
      xSemaphoreGive( xSemaphoreSensorsBeingRead);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);

  }

}







void taskRESTCommand( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreRESTCommand ) > 0)
    {


      xSemaphoreTake( xSemaphoreSensorsBeingRead, 10000);
      // Handle REST calls
      WiFiClient client = MyServer.available();

      int timeout;
      timeout = 0;
      if (client)
      {
        /*
          Serial.print("SA client=");
          Serial.println(client);

          Serial.print("SA client.available()=");
          Serial.println(client.available());

          // Thank you to MAKA69 for this suggestion
          while (!client.available()) {
          Serial.print(".");
          vTaskDelay(10 / portTICK_PERIOD_MS);
          timeout++;
          Serial.print("IL client.available()=");
          Serial.println(client.available());
          if (timeout > 1000) {
            Serial.println("INFINITE LOOP BREAK!");
            break;
          }
          }

          Serial.print("AI client.available()=");
          Serial.println(client.available());
        */
        if (client.available())
        {



          rest.handle(client);

        }
      }
      client.stop();
      xSemaphoreGive( xSemaphoreSensorsBeingRead);

      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    else
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


  }

}

// Pixel Task and routines



void taskPixelCommand( void * parameter)
{
  int i = 0;


  // Enter RTOS Task Loop
  for (;;)  {

    xSemaphoreTake( xSemaphorePixelPulse, 10000);


    for (i = 5; i < 100; i++)
    {

      strip.SetPixelColor(0, RgbwColor(i, 0, 0, 0));

      strip.Show();
      vTaskDelay(15 / portTICK_PERIOD_MS);
      xSemaphoreGive( xSemaphorePixelPulse);
      xSemaphoreTake( xSemaphorePixelPulse, 10000);

    }
    xSemaphoreGive( xSemaphorePixelPulse);
    xSemaphoreTake( xSemaphorePixelPulse, 10000);

    for (i = 100; i > 5; i--)
    {
      strip.SetPixelColor(0, RgbwColor(i, 0, 0, 0));
      strip.Show();
      vTaskDelay(15 / portTICK_PERIOD_MS);
      xSemaphoreGive( xSemaphorePixelPulse);
      xSemaphoreTake( xSemaphorePixelPulse, 10000);
    }


    xSemaphoreGive( xSemaphorePixelPulse);

  }

}





//
// Main weather display loop
//


void taskMainWeatherOLEDLoopDisplay(void * parameter)
{

  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreOLEDLoopUpdate ) > 0)
    {
      if (OLEDOn)
      {

        //Serial.println("--------->Entering Display Loop");

        updateDisplay(DISPLAY_IPNAMEID);
        vTaskDelay(3000 / portTICK_PERIOD_MS);

        updateDisplay(DISPLAY_STATUS);
        vTaskDelay(3000 / portTICK_PERIOD_MS);

        int i;
        for (i = 0; i < 4; i++)
        {
          if (moistureSensorEnable[i] == 1) // enabled
          {
            updateDisplay(DISPLAY_MOISTURE_1 + i);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
          }
        }



        if (SunAirPlus_Present)
        {

          updateDisplay(DISPLAY_SUNAIRPLUS);


          vTaskDelay(3000 / portTICK_PERIOD_MS);
        }




        //Serial.println("<-----------OutOfDisplay");
      }
      else
      {
        Serial.println("OLED Turned Off");
      }
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
