/**************************************************************************/
/*!
    @file     Adafruit_EEPROM_I2C.h
    @author   KTOWN (Adafruit Industries)

 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */
/**************************************************************************/
/// @cond DISABLE
#ifndef _ADAFRUIT_EEPROM_I2C_H_
/// @endcond
#define _ADAFRUIT_EEPROM_I2C_H_

/// @cond DISABLE
#if ARDUINO >= 100
/// @endcond
#include <Arduino.h>
/// @cond DISABLE
#else
/// @endcond
#include <WProgram.h>
/// @cond DISABLE
#endif
/// @endcond

#include <Wire.h>

#define EEPROM_DEFAULT_ADDRESS                                                 \
  (0x50) ///<* 1010 + A2 + A1 + A0 = 0x50 default */
/*!
 *    @brief  Class that stores state and functions for interacting with
 *            I2C EEPROM chips
 */
class Adafruit_EEPROM_I2C {
public:
  Adafruit_EEPROM_I2C(void);

  bool begin(uint8_t addr = EEPROM_DEFAULT_ADDRESS, TwoWire *theWire = &Wire);
  void write8(uint16_t addr, uint8_t value);
  uint8_t read8(uint16_t addr);

private:
  uint8_t i2c_addr;
  TwoWire *_wire;
};

/// @cond DISABLE
#endif
/// @endcond
