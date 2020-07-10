//
//   SDL_Arduino_SX1502 Library
//   SDL_Arduino_SX1502.cpp Arduino code - runs in continuous mode
//   Version 1.1
//   SwitchDoc Labs   May 26, 2015
//
//


#include "MQTTMessages.h"

#include <assert.h>
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "SDL_Arduino_SX1502.h"


/**************************************************************************/
/*!
  @brief  Sends a single command byte over I2C
*/
/**************************************************************************/
void SDL_Arduino_SX1502::wireWriteRegister (uint8_t reg, uint8_t value)
{
  //Serial.print("I2C Addr= 0x");
  //Serial.println(SX1502_i2caddr,HEX);
  //Serial.print("WriteAddress=0x");
  //Serial.print(reg, HEX);
  //Serial.print(" value=0x");
  //Serial.println(value, HEX);

  Wire.beginTransmission(SX1502_i2caddr);
#if ARDUINO >= 100

  Wire.write(reg);
  vTaskDelay(100 / portTICK_PERIOD_MS);
  // Register

  // Lower 8-bits
  Wire.write(value & 0xFF);
#else
  Wire.send(reg);                        // Register
  Wire.send(value & 0xFF);               // Lower 8-bits
#endif
  Wire.endTransmission();
}

/**************************************************************************/
/*!
  @brief  Reads a8 bit values over I2C
*/
/**************************************************************************/
void SDL_Arduino_SX1502::wireReadRegister(uint8_t reg, uint8_t *value)
{

  Wire.beginTransmission(SX1502_i2caddr);
#if ARDUINO >= 100
  Wire.write(reg);                       // Register
#else
  Wire.send(reg);                        // Register
#endif
  Wire.endTransmission();



  Wire.requestFrom(SX1502_i2caddr, (uint8_t)1);
  //Serial.print("Wire.available =");
  //Serial.println(Wire.available());
#if ARDUINO >= 100
  // Shift values to create properly formed integer
  *value = Wire.read();
#else
  // Shift values to create properly formed integer
  *value = Wire.receive();
#endif
}



/**************************************************************************/
/*!
  @brief  Instantiates a new SDL_Arduino_SX1502 class
*/
/**************************************************************************/
SDL_Arduino_SX1502::SDL_Arduino_SX1502(uint8_t addr) {

  SX1502_i2caddr = addr;

  SX1502_direction = 0xff;
  SX1502_pullup = 0;
  SX1502_pulldown = 0;
  SX1502_interruptmask = 0;


}

/**************************************************************************/
/*!
  @brief  Setups the HW (defaults to 32V and 2A for calibration values)
*/
/**************************************************************************/
void SDL_Arduino_SX1502::begin() {
  Wire.begin();

  //Serial.print("address="); Serial.println(SX1502_i2caddr);

}

/**************************************************************************/
/*!
  @brief  Reads 8 bits of GPIO
*/
/**************************************************************************/
uint8_t SDL_Arduino_SX1502::readGPIO() {
  uint8_t value;
  wireReadRegister(SX1502_REGDATA_ADDR, &value);
  //Serial.print("GPIO Read Raw=");
  //Serial.println(value,HEX);

  return value;
}
int sendMQTT(int messageType, String argument);
/**************************************************************************/
/*!
  @brief  Writes 8 bits of GPIO
*/
/**************************************************************************/

#include <rom/rtc.h>


uint8_t SDL_Arduino_SX1502::writeGPIO(uint8_t value) {

  wireWriteRegister(SX1502_REGDATA_ADDR, value);

  //Serial.print("writeGPIO: Value = 0x");
  //Serial.println(value, HEX);
 
  //Serial.print("GPIO Write Raw=");
  //Serial.println(value,HEX);

  return value;
}

/**************************************************************************/
/*!
  @brief  Set direction of GPIO channel
*/
/**************************************************************************/
uint8_t SDL_Arduino_SX1502::setDirectionGPIOChannel(uint8_t channel, uint8_t direction) {
  uint8_t value;

  if (direction == SX1502_INPUT)
  {
    value = SX1502_direction | channel;


  }
  else
  {
    // assume output
    value = SX1502_direction & ((~channel) & 0xFF) ;

  }

  SX1502_direction = value;

  wireWriteRegister(SX1502_REGDIR_ADDR, value);

  // Serial.print("GPIO Direction=");
  // Serial.println(value,HEX);

  return SX1502_direction;
}


/**************************************************************************/
/*!
  @brief  Set Pullup of GPIO channel
*/
/**************************************************************************/
uint8_t SDL_Arduino_SX1502::setPullupGPIOChannel(uint8_t channel, uint8_t state) {
  uint8_t value;
  if (state == SX1502_OFF)
  {
    value = SX1502_pullup & ((~channel) & 0xFF) ;
  }
  else
  {
    // assume output
    value = SX1502_pullup | channel;

  }


  SX1502_pullup = value;
  wireWriteRegister(SX1502_REGPULLUP_ADDR, value);
  //  Serial.print("GPIO Pullup=");
  //  Serial.println(value,HEX);

  return SX1502_pullup;
}

/**************************************************************************/
/*!
  @brief  Set Pulldown of GPIO channel
*/
/**************************************************************************/
uint8_t SDL_Arduino_SX1502::setPulldownGPIOChannel(uint8_t channel, uint8_t state) {
  uint8_t value;

  if (state == SX1502_OFF)
  {
    value = SX1502_pulldown & ((~channel) & 0xFF) ;
  }
  else
  {
    // assume output
    value = SX1502_pulldown | channel;

  }

  SX1502_pulldown = value;
  wireWriteRegister(SX1502_REGPULLDOWN_ADDR, value);
  // Serial.print("GPIO Pulldown=");
  //Serial.println(value,HEX);

  return SX1502_pulldown;
}


/**************************************************************************/
/*!
  @brief  Set Interrupt Mask of GPIO channel
*/
/**************************************************************************/
uint8_t SDL_Arduino_SX1502::setInterruptMaskGPIOChannel(uint8_t channel, uint8_t state) {
  uint8_t value;
  if (state == SX1502_OFF)
  {
    value = SX1502_interruptmask & ((~channel) & 0xFF) ;
  }
  else
  {
    // assume output
    value = SX1502_interruptmask | channel;

  }


  SX1502_interruptmask = value;
  wireWriteRegister(SX1502_INTERRUPTMASK_ADDR, value);
  //  Serial.print("GPIO Interrupt Mask=");
  //  Serial.println(value,HEX);

  return SX1502_interruptmask;
}
