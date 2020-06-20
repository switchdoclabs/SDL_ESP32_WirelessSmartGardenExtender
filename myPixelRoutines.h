
// my pixel effects

void blinkPixel(int pixel, int count, RgbwColor color, int time)
{

  Serial.print("Blink: ");
  Serial.print(pixel);
  Serial.print(",");
  Serial.print(count);
  Serial.print(",");
  Serial.println(time);


  int i;
  xSemaphoreTake( xSemaphorePixelPulse, 1000);
  strip.SetPixelColor(0, black);

  strip.Show();
  vTaskDelay(time / portTICK_PERIOD_MS);

  for (i = 0; i < count; i++)
  {




    strip.SetPixelColor(0, color);

    strip.Show();
    vTaskDelay(time / portTICK_PERIOD_MS);

    strip.SetPixelColor(0, black);

    strip.Show();
    vTaskDelay(time / portTICK_PERIOD_MS);


    xSemaphoreGive( xSemaphorePixelPulse);

  }

}


void blinkWhitePixel(int pixel, int count, int time)
{

  Serial.print("Blink: ");
  Serial.print(pixel);
  Serial.print(",");
  Serial.print(count);
  Serial.print(",");
  Serial.println(time);


  int i;
  xSemaphoreTake( xSemaphorePixelPulse, 1000);
  strip.SetPixelColor(0, black);

  strip.Show();
  vTaskDelay(time / portTICK_PERIOD_MS);

  for (i = 0; i < count; i++)
  {




    strip.SetPixelColor(0, white);

    strip.Show();
    vTaskDelay(time / portTICK_PERIOD_MS);

    strip.SetPixelColor(0, black);

    strip.Show();
    vTaskDelay(time / portTICK_PERIOD_MS);


    xSemaphoreGive( xSemaphorePixelPulse);

  }

}
