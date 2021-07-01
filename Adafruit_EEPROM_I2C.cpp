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
  return i2c_dev->begin();
}

/**************************************************************************/
/*!
    @brief  Writes a byte at the specific EEPROM address

    @param[in] addr
                The 16-bit address to write to in EEPROM memory
    @param[in] value
                The 8-bit value to write at addr
*/
/**************************************************************************/
bool Adafruit_EEPROM_I2C::write(uint16_t addr, uint8_t value) {
  uint8_t buff[3] = {addr >> 8, addr & 0xFF, value};

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
  uint8_t buff[2] = {addr >> 8, addr & 0xFF};

  if (!i2c_dev->write_then_read(buff, 2, buff, 1))
    return 0x0;

  return buff[0];
}
