
#include "multi_channel_relay.h"
#include "SDL_Arduino_SX1502.h"



Multi_Channel_Relay relay;



void setupGPIOBits()
{



#ifdef SGS2EXTDEBUG

  Serial.println("Turning off USB valves and Sensors");
#endif
  // run befor semaphore setup
   xSemaphoreTake( xSemaphoreUseI2C, portMAX_DELAY);
  sx1502.writeGPIO(0x00);

  sx1502.setPulldownGPIOChannel(SX1502_REG_IO0, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO1, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO2, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO3, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO4, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO5, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO6, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO7, SX1502_ON);

  sx1502.setDirectionGPIOChannel(SX1502_REG_IO0, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO1, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO2, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO3, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO4, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO5, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO6, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO7, SX1502_OUTPUT);


  xSemaphoreGive( xSemaphoreUseI2C);

}
void startupSetupGPIOBits()
{



#ifdef SGS2EXTDEBUG

  Serial.println("Turning off USB valves and Sensors");
#endif


  sx1502.writeGPIO(0x00);


  sx1502.setPulldownGPIOChannel(SX1502_REG_IO0, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO1, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO2, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO3, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO4, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO5, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO6, SX1502_ON);
  sx1502.setPulldownGPIOChannel(SX1502_REG_IO7, SX1502_ON);

  sx1502.setDirectionGPIOChannel(SX1502_REG_IO0, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO1, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO2, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO3, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO4, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO5, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO6, SX1502_OUTPUT);
  sx1502.setDirectionGPIOChannel(SX1502_REG_IO7, SX1502_OUTPUT);


  //xSemaphoreGive( xSemaphoreUseI2C);
}

void writeGPIOBit(byte pin, byte value)
{

  byte currentValue;
  byte pinBit;
  byte writeValue;
  byte valuebit;
  xSemaphoreTake( xSemaphoreUseI2C, portMAX_DELAY);


  currentValue = sx1502.readGPIO();



  valuebit = value << pin;
  pinBit = 1 << pin;



  if (value == 1)
  {
    writeValue = (valuebit | currentValue);
  }
  if (value == 0)
  {
    writeValue = (~pinBit & currentValue);
  }




  sx1502.writeGPIO(writeValue);

  xSemaphoreGive( xSemaphoreUseI2C);

}

void setupRelays()
{
  xSemaphoreTake( xSemaphoreUseI2C, portMAX_DELAY);

  // Set I2C address and start relay
  relay.begin(RELAYADDRESS);
#ifdef SGS2EXTDEBUG

  Serial.println("Turning off relays");
#endif



  int i;
  for (i = 1; i < 5; i++)
  {
    relay.turn_off_channel(i);
#ifdef SGS2EXTDEBUG

    Serial.print("Turning off relay:");
    Serial.println(i);
#endif


  }
  xSemaphoreGive( xSemaphoreUseI2C);

}

void writeRelay(byte pin, byte value)
{

  Serial.print("WRxSemaphoreUseI2C=");
  Serial.println(uxSemaphoreGetCount( xSemaphoreUseI2C ));
  xSemaphoreTake( xSemaphoreUseI2C, portMAX_DELAY);


  // 1-4

  if (value == 0)
  {
    relay.turn_off_channel(pin);
  }
  if (value == 1)
  {
    relay.turn_on_channel(pin);
  }

  xSemaphoreGive( xSemaphoreUseI2C);
  Serial.print("PTWRxSemaphoreUseI2C=");
  Serial.println(uxSemaphoreGetCount( xSemaphoreUseI2C ));
}


void initializeValvesAndSensors()
{

#ifdef SGS2EXTDEBUG

  Serial.println("Initializing Valves and Sensors");
#endif

  setupGPIOBits();

  setupRelays();


#ifdef SGS2EXTDEBUG

  Serial.println("Exiting Initializing Valves and Sensors");
#endif

}

int writeValve(int number, byte OnOff)
{
  Serial.print("xSemaphoreUseI2C=");
  Serial.println(uxSemaphoreGetCount( xSemaphoreUseI2C ));


  switch (number)
  {
    case 1:
    case 2:
    case 3:
    case 4:
      Serial.println("Before writeGPIOBit");
      writeGPIOBit(number + 3, OnOff);

      break;

    case 5:
    case 6:
    case 7:
    case 8:
      Serial.println("Before writeRelay");
      writeRelay(number - 4, OnOff);
      break;

    default:

      return 1;
  }

  return 0;
}

void printValveState()
{
  int i;
  for (i = 0; i < 8; i++)
  {

    Serial.print(i + 1); Serial.print(":");
    Serial.print(valveState[i]);
    Serial.print(", ");
    Serial.print(valveTime[i]);
    Serial.print("|");


  }
  Serial.println();
}


void turnOnAppropriateValves()
{
  int i;
  bool valveChange = false;

  for (i = 0; i < 8; i++)
  {
    if (valveState[i] == 1)
    {
      writeValve(i + 1, 1);
      valveChange = true;

    }
    else
    {
      writeValve(i + 1, 0);
    }
  }

  if (valveChange == true)
  {
    sendMQTT(MQTTVALVECHANGE, "");
  }


}
