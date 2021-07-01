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

#include <Adafruit_EEPROM_I2C.h>

#define MB85RC_DEFAULT_ADDRESS                                                 \
  (0x50) ///<* 1010 + A2 + A1 + A0 = 0x50 default */
#define MB85RC_SECONDARY_ADDRESS                                               \
  (0x7C) ///< secondary ID for manufacture id info

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            I2C FRAM chips
 */
class Adafruit_FRAM_I2C : public Adafruit_EEPROM_I2C {
public:
  Adafruit_FRAM_I2C(void);

  bool begin(uint8_t addr = MB85RC_DEFAULT_ADDRESS, TwoWire *theWire = &Wire);
  void getDeviceID(uint16_t *manufacturerID, uint16_t *productID);

private:
  Adafruit_I2CDevice *i2c_dev2 = NULL;
  boolean _framInitialised;
};

#endif
