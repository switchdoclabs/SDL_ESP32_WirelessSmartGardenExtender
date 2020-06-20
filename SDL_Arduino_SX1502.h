//
//   SDL_Arduino_SX1502 Library
//   SDL_Arduino_SX1502.cpp Arduino code - runs in continuous mode
//   Version 1.1
//   SwitchDoc Labs   May 26, 2015
//
//

#pragma once

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define SX1502_ADDRESS                         (0x21)    // 0x21 (Addr=VDD)

/*=========================================================================*/

/*=========================================================================
    REGISTERS (R/W)
    -----------------------------------------------------------------------*/

   #define SX1502_REGDATA_ADDR                   (0x00) // RegData
   #define SX1502_REGDIR_ADDR                    (0x01) // Direction
   #define SX1502_REGPULLUP_ADDR                 (0x02) // Pullups
   #define SX1502_REGPULLDOWN_ADDR               (0x03) // Pulldowns
   
   #define SX1502_INTERRUPTMASK_ADDR             (0x05) // Interrupt Mask
   #define SX1502_SENSEHIGH_ADDR                 (0x06) // Interrupt direction 7-4
   #define SX1502_SENSELOW_ADDR                  (0x07) // Interrupt direction 3-0
   
   #define SX1502_INTERRUPTSOURCE_ADDR           (0x08) // Interrupt Source 
   #define SX1502_EVENTSTATUS_ADDR               (0x09) // Event Status of I/Os
   
   #define SX1502_REGPLDMODE_ADDR                (0x10) // PLD Mode  
   #define SX1502_REGPLDPLDTABLE0_ADDR           (0x11) // PLD Truth Table 
   #define SX1502_REGPLDPLDTABLE1_ADDR           (0x12) // PLD Truth Table  
   #define SX1502_REGPLDPLDTABLE2_ADDR           (0x13) // PLD Truth Table  
   #define SX1502_REGPLDPLDTABLE3_ADDR           (0x14) // PLD Truth Table  
   #define SX1502_REGPLDPLDTABLE4_ADDR           (0x15) // PLD Truth Table  
 
   #define SX1502_REGADVANCED_ADDR               (0xAB) // Advanced Settings
   

   
/*---------------------------------------------------------------------*/
   #define SX1502_CONFIG_RESET                    (0x8000)  // Reset Bit
	
   #define SX1502_REG_IO7                     (0x80)  // Channel IO7 
   #define SX1502_REG_IO6                     (0x40)  // Channel IO6 
   #define SX1502_REG_IO5                     (0x20)  // Channel IO5 
   #define SX1502_REG_IO4                     (0x10)  // Channel IO4 
   #define SX1502_REG_IO3                     (0x08)  // Channel IO3 
   #define SX1502_REG_IO2                     (0x04)  // Channel IO2 
   #define SX1502_REG_IO1                     (0x02)  // Channel IO1 
   #define SX1502_REG_IO0                     (0x01)  // Channel IO0 
   
   #define SX1502_INPUT                       (0x01)  // 0 means input
   #define SX1502_OUTPUT                      (0x00)  // 1 means output
   
   #define SX1502_OFF                         (0x00)  // 0 means off
   #define SX1502_ON                          (0x01)  // 1 means on
   
   	
   #define SX1502_REG_SENS_NONE              (0x0)  // None - Interrupt Edge Sensitivity
   #define SX1502_REG_SENS_RISING            (0x1)  // Rising - Interrupt Edge Sensitivity
   #define SX1502_REG_SENS_FALLING           (0x2)  // Falling - Interrupt Edge Sensitivity
   #define SX1502_REG_SENS_BOTH              (0x3)  // None - Interrupt Edge Sensitivity


    
    



class SDL_Arduino_SX1502{
 public:
  SDL_Arduino_SX1502(uint8_t addr = SX1502_ADDRESS);
  void begin(void);
  uint8_t readGPIO();
  uint8_t writeGPIO(uint8_t value);
  uint8_t setDirectionGPIOChannel(uint8_t channel, uint8_t direction);
  uint8_t setPullupGPIOChannel(uint8_t channel, uint8_t state);
  uint8_t setPulldownGPIOChannel(uint8_t channel, uint8_t state);  
  uint8_t setInterruptMaskGPIOChannel(uint8_t channel, uint8_t state);


 private:
  uint8_t SX1502_i2caddr;
  uint8_t SX1502_direction;
  uint8_t SX1502_pullup;
  uint8_t SX1502_pulldown;
  uint8_t SX1502_interruptmask;


  
  void wireWriteRegister(uint8_t reg, uint8_t value);
  void wireReadRegister(uint8_t reg, uint8_t *value);


};
