/*********************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers


  This example is for a 128x64 size display using I2C to communicate

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information

  Heavily modified by SDL
*********************************************************************/

// prototypes

void setDisplayLine(int lineNumber, char *value);
void writeAllDisplayLines(int DisplayMode);


// https://stackoverflow.com/questions/9072320/split-string-into-string-array

char displayLines[25][28];


// WeatherPlus Text Buffer Lines

void updateDisplay(int displayMode)
{

  if (OLED_Present) {

    char buffer[40];
    char returnString[200];
    /*
      #ifdef SGS2EXTDEBUG
      Serial.print("displayMode=");
      Serial.println(displayMode);
      #endif
    */
    String windDirection;

    //Serial.print("StartofupdateDisplay xi2c=");
    //Serial.println(uxSemaphoreGetCount( xSemaphoreUseI2C ));


    String currentTimeString;
    currentTimeString = returnDateTime(now());

    switch (displayMode)
    {


      case DISPLAY_UPDATING:
        Serial.println("Updating SGS2WExt Software");



        setDisplayLine(0,  "SGS2WExt Updating");
        setDisplayLine(1, "");
        setDisplayLine(2, "");
        setDisplayLine(3, "");
        setDisplayLine(4, "");
        setDisplayLine(5, "");

        break;

      case DISPLAY_UPDATE_FINISHED:
        Serial.println("Finish SGS2WExt Software");



        setDisplayLine(0,  "SGS2WExt Updated");
        setDisplayLine(1, "Wait 120 Seconds");
        setDisplayLine(2, "Unplug Power");
        setDisplayLine(3, "Wait 15 Seconds");
        setDisplayLine(4, "Plug Power In");
        setDisplayLine(5, "");

        break;

      case DISPLAY_NO_UPDATE_AVAILABLE:
        setDisplayLine(0,  "You have the most");
        setDisplayLine(1, "recent software");
        setDisplayLine(2, "");
        setDisplayLine(3, "");
        setDisplayLine(4, "");
        setDisplayLine(5, "");

        break;

      case DISPLAY_NO_UPDATE_FAILED:
        setDisplayLine(0,  "Update Failed.");
        setDisplayLine(1, "Try again later.");
        setDisplayLine(2, "");
        setDisplayLine(3, "");
        setDisplayLine(4, "");
        setDisplayLine(5, "");

        break;




      case DISPLAY_POWERUP:
        Serial.println("SGS2WExt Booting Up");

        strcpy(buffer, "Ver: ");
        strcat(buffer, SGSEXTENDERESP32VERSION);

        setDisplayLine(0,  "SGS2WExt Boot");
        setDisplayLine(1, buffer);
        setDisplayLine(2, "");
        setDisplayLine(3, "");

        break;

      case  DISPLAY_ACCESSPOINT:
        {





          setDisplayLine(0,  "SGSExt2 AP");
          buffer[0] = '\0';
          strcpy(buffer, APssid.c_str());
          setDisplayLine(1, buffer);
          buffer[0] = '\0';

          IPAddress myIp2 = WiFi.softAPIP();
          sprintf(buffer, "%d.%d.%d.%d", myIp2[0], myIp2[1], myIp2[2], myIp2[3]);
          setDisplayLine(2, buffer);
        }
        break;


      case  DISPLAY_TRYING_AP:
        {





          setDisplayLine(0,  "Trying WiFi AP");
          buffer[0] = '\0';
          strcpy(buffer, "192.168.4.1");
          setDisplayLine(1, buffer);
          buffer[0] = '\0';

#define WL_MAC_ADDR_LENGTH 6
          // Append the last two bytes of the MAC (HEX'd) to string to make unique
          uint8_t mac[WL_MAC_ADDR_LENGTH];
          WiFi.softAPmacAddress(mac);
          String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                         String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
          macID.toUpperCase();
          String mySSID;
          mySSID = "SGS2WExt-" + macID;
          buffer[0] = '\0';
          strcpy(buffer, mySSID.c_str());
          setDisplayLine(2, buffer);
          setDisplayLine(3, "");

        }
        break;

      case  DISPLAY_TRYING_SMARTCONFIG:
        {
          Serial.println("SGS2WExt Setup");




          setDisplayLine(0,  "Trying WiFi ");
          buffer[0] = '\0';
          strcpy(buffer, "Smart Config");
          setDisplayLine(1, buffer);
          buffer[0] = '\0';
          setDisplayLine(2, "");
          setDisplayLine(3, "");

        }
        break;

      case  DISPLAY_TRYING_WPS:
        {
          Serial.println("SGS2WExt Setup");




          setDisplayLine(0,  "Trying WiFi WPS");
          buffer[0] = '\0';

          setDisplayLine(1, "");
          setDisplayLine(2, "");
          setDisplayLine(3, "");

        }
        break;



      case  DISPLAY_FAILING_AP:
        {

          setDisplayLine(0,  "Failing to connect WiFi AP");
          buffer[0] = '\0';
          strcpy(buffer, WiFi_SSID.c_str());
          setDisplayLine(1, buffer);
          buffer[0] = '\0';
          setDisplayLine(2, "Restarting SGS2WExt");
          setDisplayLine(3, "Try again....");

        }
        break;

      case DISPLAY_FAILED_RECONNECT:
        {

          setDisplayLine(0,  "Failing to reconnect to WiFI");
          buffer[0] = '\0';
          strcpy(buffer, WiFi_SSID.c_str());
          setDisplayLine(1, buffer);
          buffer[0] = '\0';
          setDisplayLine(3, "Will try again....");

        }
        break;

      case DISPLAY_IPDISPLAY:

        {
          setDisplayLine(0, "SGS2WExt Connected");
          setDisplayLine(1, "IP Address:");



          IPAddress myIp2 = WiFi.localIP();
          sprintf(buffer, "%d.%d.%d.%d", myIp2[0], myIp2[1], myIp2[2], myIp2[3]);
          setDisplayLine(2, buffer);
        }
        break;

      case DISPLAY_IPNAMEID:

        {

          setDisplayLine(0, "IP Address:");

          IPAddress myIp2 = WiFi.localIP();
          sprintf(buffer, "%d.%d.%d.%d", myIp2[0], myIp2[1], myIp2[2], myIp2[3]);
          setDisplayLine(1, buffer);
          sprintf(buffer, "%s / %s", stationName.c_str(), myID.c_str());
          setDisplayLine(2, buffer);
        }
        break;

      case DISPLAY_STATUS:

        setDisplayLine(0, "SGS W Ext Status");
        buffer[0] = '\0';


        sprintf(buffer, "VS:  %1i%1i%1i%1i%1i%1i%1i%1i", valveState[0], valveState[1], valveState[2], valveState[3], valveState[4], valveState[5], valveState[6], valveState[7]);
        setDisplayLine(1, buffer);
        buffer[0] = '\0';
        sprintf(buffer, "MSE: %1i%1i%1i%1i", moistureSensorEnable[0], moistureSensorEnable[1], moistureSensorEnable[2], moistureSensorEnable[3]);
        setDisplayLine(2, buffer);
        setDisplayLine(3, "");
        break;

      case DISPLAY_MOISTURE_1:
      case DISPLAY_MOISTURE_2:
      case DISPLAY_MOISTURE_3:
      case DISPLAY_MOISTURE_4:

        int unit;
        unit = displayMode - DISPLAY_MOISTURE_1;

        // Displays Moisture Levels
        buffer[0] = '\0';
        sprintf(buffer, "MSensor #%i", unit+1);
        setDisplayLine(0, buffer);
        buffer[0] = '\0';
        sprintf(buffer, "%3.1f%%", moistureSensors[unit]);
        setDisplayLine(1, buffer);
        setDisplayLine(2, "");
        break;



      case DISPLAY_DEVICEPRESENT:

        {
          setDisplayLine(0, "---Devices Present---");

          String myBuffer;
          myBuffer = "SAP: ";
          if (SunAirPlus_Present == true)
          {
            myBuffer = myBuffer + "X ";
          }
          else
          {
            myBuffer = myBuffer + "- ";
          }

          strcpy(buffer, myBuffer.c_str());
          setDisplayLine(1, buffer );

          myBuffer = "AQ: ";
          if (AirQualityPresent == true)
          {
            myBuffer = myBuffer + "+ ";
          }
          else
          {
            myBuffer = myBuffer + "- ";
          }

          strcpy(buffer, myBuffer.c_str());
          setDisplayLine(2, buffer );

          myBuffer = "SUNLT: ";
          if (TSL2591_Present == true)
          {
            myBuffer = myBuffer + "+ ";
          }
          else
          {
            myBuffer = myBuffer + "- ";
          }




          strcpy(buffer, myBuffer.c_str());
          setDisplayLine(3, buffer );

          myBuffer = "";


          strcpy(buffer, myBuffer.c_str());
          setDisplayLine(4, buffer );

          myBuffer = "SMLA: ";
          if (SolarMAXLA == 1)
          {
            myBuffer = myBuffer + "+ ";
          }
          else
          {
            myBuffer = myBuffer + "- ";
          }

          myBuffer = myBuffer + "SMLP: ";
          if (SolarMAXLiPo == true)
          {
            myBuffer = myBuffer + "+";
          }
          else
          {
            myBuffer = myBuffer + "-";
          }


          strcpy(buffer, myBuffer.c_str());
          setDisplayLine(5, buffer );


        }
        break;



      case DISPLAY_SUNAIRPLUS:

        {
          setDisplayLine(0, "Solar Readings");
          setDisplayLine(1, "----------------");

          String stringSolar;
          stringSolar = "Battery:" + String(BatteryVoltage, 2) + "V/" + String(BatteryCurrent, 1) + "mA";
          setDisplayLine(2, const_cast<char*>(stringSolar.c_str()) );

          stringSolar = "Solar:" + String(SolarPanelVoltage, 2) + "V/" + String(SolarPanelCurrent, 1) + "mA";
          setDisplayLine(3, const_cast<char*>(stringSolar.c_str()) );

          stringSolar = "Load:" + String(LoadVoltage, 2) + "V/" + String(LoadCurrent, 1) + "mA";
          setDisplayLine(4, const_cast<char*>(stringSolar.c_str()) );




        }
        break;



      default:
        break;
    }

    xSemaphoreTake( xSemaphoreUseI2C, 30000);

    writeAllDisplayLines(displayMode);

    xSemaphoreGive( xSemaphoreUseI2C);   // initialize
  }

}



void setDisplayLine(int lineNumber, char *value)
{
  if (lineNumber < 32)
  {

    strncpy( displayLines[lineNumber], value, 20);

  }




}


void writeAllDisplayLines(int DisplayMode)
{
  display.clearDisplay();

  switch (DisplayMode)
  {
    case DISPLAY_POWERUP:
    case DISPLAY_UPDATE_FINISHED:
    case DISPLAY_UPDATING:
    case DISPLAY_NO_UPDATE_AVAILABLE:
    case DISPLAY_NO_UPDATE_FAILED:
    case DISPLAY_STATUS:

      {
        int textSize = 1;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 6; i++)
        {
          display.setCursor(0, (30 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }

      break;

    case DISPLAY_MOISTURE_1:
    case DISPLAY_MOISTURE_2:
    case DISPLAY_MOISTURE_3:
    case DISPLAY_MOISTURE_4:
      {
        int textSize = 2;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 6; i++)
        {
          display.setCursor(0, (30 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }
      break;

    case DISPLAY_IPDISPLAY:
    case DISPLAY_IPNAMEID:

    case DISPLAY_ACCESSPOINT:
    case DISPLAY_TRYING_AP:
    case DISPLAY_FAILING_AP:
    case DISPLAY_SDL2MQTTServer:
    case DISPLAY_TRYING_SMARTCONFIG:
    case DISPLAY_TRYING_WPS:

    case DISPLAY_FAILED_RECONNECT:


      {
        int textSize = 1;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 3; i++)
        {

          if (i == 0)
            textSize = 1;
          if (i == 1)
            textSize = 1;
          if (i == 2)
            textSize = 1;
          display.setTextSize(textSize);

          display.setCursor(0, 20 * (i));

          //display.setCursor(0, (30 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }

      break;


    default:
      break;

  }

}






#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

//static const unsigned char PROGMEM logo16_glcd_bmp[] =
static const unsigned char logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};



void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
    }
    display.display();
    vTaskDelay(200 / portTICK_PERIOD_MS);

    // then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i = 0; i < display.height(); i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawrect(void) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display();
  }
}

void testdrawline() {
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, WHITE);
    display.display();
  }
  vTaskDelay(250 / portTICK_PERIOD_MS);

  display.clearDisplay();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
    display.display();
  }
  vTaskDelay(250 / portTICK_PERIOD_MS);

  display.clearDisplay();
  for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
    display.display();
  }
  vTaskDelay(250 / portTICK_PERIOD_MS);

  display.clearDisplay();
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  vTaskDelay(250 / portTICK_PERIOD_MS);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.stopscroll();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  display.startscrollleft(0x00, 0x0F);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.stopscroll();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  display.startscrolldiagright(0x00, 0x07);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.startscrolldiagleft(0x00, 0x07);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  display.stopscroll();
}

#if (SSD1306_LCDHEIGHT != 64)
// #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void OLEDDisplaySetup()   {



  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  vTaskDelay(4000 / portTICK_PERIOD_MS);

  // Clear the buffer.
  display.clearDisplay();
  /*
    // draw a single pixel
    display.drawPixel(10, 10, WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    display.display();
    delay(2000);
    display.clearDisplay();


     // draw many lines
     testdrawline();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw rectangles
     testdrawrect();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw multiple rectangles
     testfillrect();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw mulitple circles
     testdrawcircle();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw a white circle, 10 pixel radius
     display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
     display.display();
     delay(2000);
     display.clearDisplay();

     testdrawroundrect();
     delay(2000);
     display.clearDisplay();

     testfillroundrect();
     delay(2000);
     display.clearDisplay();

     testdrawtriangle();
     delay(2000);
     display.clearDisplay();

     testfilltriangle();
     delay(2000);
     display.clearDisplay();

     // draw the first ~12 characters in the font
     testdrawchar();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw scrolling text
     testscrolltext();
     delay(2000);
     display.clearDisplay();

     // text display tests
     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(0,0);
     display.println("Hello, world!");
     display.setTextColor(BLACK, WHITE); // 'inverted' text
     display.println(3.141592);
     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.print("0x"); display.println(0xDEADBEEF, HEX);
     display.display();
     delay(2000);

     // miniature bitmap display
     display.clearDisplay();
     display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
     display.display();

     // invert the display
     display.invertDisplay(true);
     delay(1000);
     display.invertDisplay(false);
     delay(1000);

     // draw a bitmap icon and 'animate' movement
     testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
  */
}

/*
  void loop() {

  }
*/
