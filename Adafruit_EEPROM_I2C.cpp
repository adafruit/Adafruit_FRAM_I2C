/**************************************************************************/
/*!
 * @file Adafruit_EEPROM_I2C.cpp
 */
/**************************************************************************/
#include <math.h>
#include <stdlib.h>

#include "Adafruit_EEPROM_I2C.h"

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
Adafruit_EEPROM_I2C::Adafruit_EEPROM_I2C(void) {}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  addr
 *            The I2C address to be used.
 *    @param  theWire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_EEPROM_I2C::begin(uint8_t addr, TwoWire *theWire) {
  i2c_dev = new Adafruit_I2CDevice(addr, theWire);
  _addr = addr;

  return i2c_dev->begin();
}

/**************************************************************************/
/*!
    @brief  Writes a byte at the specific EEPROM address

    @param[in] addr
                The 16-bit address to write to in EEPROM memory
    @param[in] value
                The 8-bit value to write at addr
    @returns True on I2C command success, false on timeout or I2C failure
*/
/**************************************************************************/
bool Adafruit_EEPROM_I2C::write(uint16_t addr, uint8_t value) {
  uint8_t buff[3] = {(uint8_t)(addr >> 8), (uint8_t)addr, value};

  if (!i2c_dev->write(buff, 3))
    return false;

  // Wait until it acks!
  uint8_t timeout = 100;
  while (timeout--) {
    if (i2c_dev->detected())
      return true;
    delay(1);
  }

  // timed out :(
  return false;
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value from the specified EEPROM address
    @param addr
                The 16-bit address to read from in EEPROM memory
    @returns    The 8-bit value retrieved at addr
*/
/**************************************************************************/
uint8_t Adafruit_EEPROM_I2C::read(uint16_t addr) {
  uint8_t buff[2] = {(uint8_t)(addr >> 8), (uint8_t)addr};

  if (!i2c_dev->write_then_read(buff, 2, buff, 1))
    return 0x0;

  return buff[0];
}

/**************************************************************************/
/*!
    @brief  Writes multiple bytes at the specific EEPROM address

    @param[in] addr
                The 16-bit address to write to in EEPROM memory
    @param[in] buffer Pointer to buffer of bytes to write
    @param num How many bytes to write!
    @returns True on I2C command success, false on timeout or I2C failure
*/
/**************************************************************************/
bool Adafruit_EEPROM_I2C::write(uint16_t addr, uint8_t *buffer, uint16_t num) {
  while (num--) {
    if (!write(addr++, buffer[0])) {
      return false;
    }
    buffer++;
  }
  return true;
}

/**************************************************************************/
/*!
    @brief  Reads multiple bytes from the specified EEPROM address
    @param addr
                The 16-bit address to read from in EEPROM memory
    @param buffer Pointer to buffer of bytes that will be filled!
    @param num How many bytes to write!
    @returns    The 8-bit value retrieved at addr
*/
/**************************************************************************/
bool Adafruit_EEPROM_I2C::read(uint16_t addr, uint8_t *buffer, uint16_t num) {

  for (uint16_t i = 0; i < num; i++) {
    uint8_t buff[2] = {(uint8_t)(addr >> 8), (uint8_t)addr};

    if (!i2c_dev->write_then_read(buff, 2, buff, 1))
      return false;
    buffer[i] = buff[0];

    addr++;
  }

  return true;
}
