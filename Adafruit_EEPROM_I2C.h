/**************************************************************************/
/*!
 * @file Adafruit_EEPROM_I2C.h
 */
/**************************************************************************/
#ifndef _ADAFRUIT_EEPROM_I2C_H_
#define _ADAFRUIT_EEPROM_I2C_H_

#include <Adafruit_I2CDevice.h>
#include <Arduino.h>

///<* 1010 + A2 + A1 + A0 = 0x50 default */
#define EEPROM_DEFAULT_ADDRESS (0x50)

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            I2C EEPROM chips
 */
class Adafruit_EEPROM_I2C {
public:
  Adafruit_EEPROM_I2C(void);

  bool begin(uint8_t addr = EEPROM_DEFAULT_ADDRESS, TwoWire *theWire = &Wire);
  bool write(uint16_t addr, uint8_t value);
  uint8_t read(uint16_t addr);
  bool write(uint16_t addr, uint8_t *buffer, uint16_t num);
  bool read(uint16_t addr, uint8_t *buffer, uint16_t num);

  /**************************************************************************/
  /*!
      @brief  Write any object to memory
      @param addr
                  The 16-bit address to write to in EEPROM memory
      @param value  The templated object we will be writing
      @returns    The number of bytes written
  */
  /**************************************************************************/
  template <class T> uint16_t writeObject(uint16_t addr, const T &value) {
    const byte *p = (const byte *)(const void *)&value;
    uint16_t n;
    for (n = 0; n < sizeof(value); n++) {
      write(addr++, *p++);
    }
    return n;
  }

  /**************************************************************************/
  /*!
      @brief Read any object from memory
      @param addr
                  The 16-bit address to write to in EEPROM memory
      @param value  The address of the templated object we will be writing INTO
      @returns    The number of bytes read
  */
  /**************************************************************************/

  template <class T> uint16_t readObject(uint16_t addr, T &value) {
    byte *p = (byte *)(void *)&value;
    uint16_t n;
    for (n = 0; n < sizeof(value); n++) {
      *p++ = read(addr++);
    }
    return n;
  }

protected:
  /*! The internal I2C device for communication */
  Adafruit_I2CDevice *i2c_dev = NULL;
  /*! The I2C address, sometimes needed! */
  uint8_t _addr = 0;
};

#endif
