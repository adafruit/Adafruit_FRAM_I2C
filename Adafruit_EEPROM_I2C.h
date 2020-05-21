/**************************************************************************/
/*!
 * @file Adafruit_EEPROM_I2C.h
 */
/**************************************************************************/
#ifndef _ADAFRUIT_EEPROM_I2C_H_
#define _ADAFRUIT_EEPROM_I2C_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

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

#endif
