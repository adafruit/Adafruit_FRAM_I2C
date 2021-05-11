/**************************************************************************/
/*!
    @file     Adafruit_FRAM_I2C.h
    @author   KTOWN (Adafruit Industries)

    Software License Agreement (BSD License)

    Copyright (c) 2013, Adafruit Industries
    All rights reserved.
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
*/
/**************************************************************************/
#ifndef _ADAFRUIT_FRAM_I2C_H_
#define _ADAFRUIT_FRAM_I2C_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Wire.h>

#define MB85RC_DEFAULT_ADDRESS                                                 \
  (0x50)                       ///<* 1010 + A2 + A1 + A0 = 0x50 default */
#define MB85RC_SLAVE_ID (0xF8) ///< SLAVE ID

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            I2C FRAM chips
 */
class Adafruit_FRAM_I2C {
public:
  Adafruit_FRAM_I2C(void);

  boolean begin(uint8_t addr = MB85RC_DEFAULT_ADDRESS);
  void write8(uint16_t framAddr, uint8_t value);
  uint8_t read8(uint16_t framAddr);
  void getDeviceID(uint16_t *manufacturerID, uint16_t *productID);

  //write anything to FRAM
  template <class T> uint16_t writeAnything(uint16_t ee, const T& value)
  {
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++) {
      Adafruit_FRAM_I2C::write8(ee++, *p++);
    }
    return i;
  }

  //read anything from FRAM
  template <class T> uint16_t readAnything(uint16_t ee, T& value)
  {
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++) {
      *p++ = Adafruit_FRAM_I2C::read8(ee++);	
    } 
    return i;
  }

private:
  uint8_t i2c_addr;
  boolean _framInitialised;
};

#endif
