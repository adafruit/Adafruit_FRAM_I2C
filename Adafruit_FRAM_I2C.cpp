/*!
 * @file     Adafruit_FRAM_I2C.cpp
 *
 * @mainpage Adafruit FRAM I2C
 *
 * @author   KTOWN (Adafruit Industries)
 *
 * @section intro_sec Introduction
 *
 * Driver for the Adafruit I2C FRAM breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section  HISTORY
 *
 * v1.0 - First release
 *
 * @section license License
 *
 * BSD license, all text above must be included in any redistribution (see
 license.txt)
 */
#include <math.h>
#include <stdlib.h>

#include "Adafruit_FRAM_I2C.h"

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
Adafruit_FRAM_I2C::Adafruit_FRAM_I2C(void) { _framInitialised = false; }

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
bool Adafruit_FRAM_I2C::begin(uint8_t addr, TwoWire *theWire) {
  Adafruit_EEPROM_I2C::begin(addr, theWire);

  // the MB85 has a secondary address too!
  i2c_dev2 = new Adafruit_I2CDevice(MB85RC_SECONDARY_ADDRESS, theWire);
  if (!i2c_dev2->begin()) {
    return false;
  }

  /* Make sure we're actually connected */
  uint16_t manufID, prodID;
  getDeviceID(&manufID, &prodID);
  if (manufID != 0x00A) {
    Serial.print("Unexpected Manufacturer ID: 0x");
    Serial.println(manufID, HEX);
    return false;
  }
  if (prodID != 0x510) {
    Serial.print("Unexpected Product ID: 0x");
    Serial.println(prodID, HEX);
    return false;
  }

  /* Everything seems to be properly initialised and connected */
  _framInitialised = true;

  return true;
}

/**************************************************************************/
/*!
    @brief  Reads the Manufacturer ID and the Product ID frm the IC

    @param[out]  manufacturerID
                  The 12-bit manufacturer ID (Fujitsu = 0x00A)
    @param[out]  productID
                  The memory density (bytes 11..8) and proprietary
                  Product ID fields (bytes 7..0). Should be 0x510 for
                  the MB85RC256V.
*/
/**************************************************************************/
void Adafruit_FRAM_I2C::getDeviceID(uint16_t *manufacturerID,
                                    uint16_t *productID) {
  uint8_t buff[3] = {(uint8_t)(_addr * 2), 0, 0};

  i2c_dev2->write_then_read(buff, 1, buff, 3, false);
  /* Shift values to separate manuf and prod IDs */
  /* See p.10 of
   * http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC256V-DS501-00017-3v0-E.pdf
   */
  *manufacturerID = (buff[0] << 4) + (buff[1] >> 4);
  *productID = ((buff[1] & 0x0F) << 8) + buff[2];
}
