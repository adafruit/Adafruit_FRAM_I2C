/**************************************************************************/
/*!
    @file     Adafruit_FRAM_I2C.cpp
    @author   KTOWN (Adafruit Industries)
    @license  BSD (see license.txt)

    Driver for the Adafruit I2C FRAM breakout.

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/
//#include <avr/pgmspace.h>
//#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "Adafruit_FRAM_I2C.h"

// DEV_DBG: Do not check in with this defined.
//#define DEV_DBG

// This is the maximum number of bytes that can be received in one go (Arduino API)
#define MULTIBYTE_BLOCK_RX_LEN 32
// This is the maximum number of data bytes that can be sent in one go (Arduino API)
#define MULTIBYTE_BLOCK_TX_LEN 30

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
Adafruit_FRAM_I2C::Adafruit_FRAM_I2C()
{
  _framInitialised = false;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
    Initializes I2C and configures the chip (call this function before
    doing anything else)
*/
/**************************************************************************/
boolean Adafruit_FRAM_I2C::begin(uint8_t addr)
{
  i2c_addr = addr;

  Wire.begin();

  /* Make sure we're actually connected */
  uint16_t manufID, prodID;
  getDeviceID(&manufID, &prodID);

  /* PRH: The 1mbit (Cypress-FM24V10) FRAM these changes were tested with uses 04:400 for manufID:prodID */
  if (manufID != 0x00A && manufID != 0x04)
  {
#ifdef DEV_DBG
    Serial.print("Unexpected Manufacturer ID: 0x"); Serial.println(manufID, HEX);
#endif
    return false;
  }
  if (prodID != 0x510 && prodID != 0x400)
  {
#ifdef DEV_DBG
    Serial.print("Unexpected Product ID: 0x"); Serial.println(prodID, HEX);
#endif
    return false;
  }
  /* Everything seems to be properly initialised and connected */
  _framInitialised = true;

  return true;
}

/**************************************************************************/
/*!
    @brief  Writes a byte at the specific FRAM address

    @params[in] framAddr
                The 32bit address to write to in FRAM memory
    @params[in] i2cAddr
                The 8-bit value to write at framAddr
*/
/**************************************************************************/
void Adafruit_FRAM_I2C::write8 (uint32_t framAddr, uint8_t value)
{
  write(framAddr, &value, 1);
}

/**************************************************************************/
/*!
    @brief  Writes count bytes starting at the specific FRAM address

    @params[in] framAddr
                The 32bit address to write to in FRAM memory
    @params[in] values
                The pointer to an array of 8-bit values to write starting at addr
    @params[in] count
                The number of bytes to write
*/
/**************************************************************************/
void Adafruit_FRAM_I2C::write (uint32_t framAddr, const uint8_t *values, uint32_t count)
{
  uint32_t hasWritten = 0;
  uint32_t toWrite = count;

  while (toWrite > 0)
  {
	uint32_t addr = framAddr+hasWritten;
	uint8_t pageBit = (addr & 0x10000) ? MB85RC_PAGE_BIT : 0;
    Wire.beginTransmission(i2c_addr | pageBit);
    writeAddress(addr);
    uint8_t block = (toWrite > MULTIBYTE_BLOCK_TX_LEN) ? MULTIBYTE_BLOCK_TX_LEN : toWrite;
    uint8_t done = Wire.write(&values[hasWritten], block);
    toWrite -= done;
    hasWritten += done;
    Wire.endTransmission();
  }
#ifdef DEV_DBG
  if (hasWritten != count)
  {
	  // Something went wrong...
	  Serial.print("I2C FRAM Write failed, ");
	  Serial.print(count);
	  Serial.print(" requested but wrote ");
	  Serial.println(hasWritten);
  }
#endif
}


/**************************************************************************/
/*!
    @brief  Reads an 8 bit value from the specified FRAM address

    @params[in] framAddr
                The 32bit address to read from in FRAM memory

    @returns    The 8-bit value retrieved at framAddr
*/
/**************************************************************************/
uint8_t Adafruit_FRAM_I2C::read8 (uint32_t framAddr)
{
  uint8_t data=0;
  read(framAddr, &data, 1);
  return data;
}

/**************************************************************************/
/*!
    @brief  Read count bytes starting at the specific FRAM address

    @params[in] framAddr
                The 32bit address to write to in FRAM memory
    @params[out] values
                The pointer to an array of 8-bit values to read starting at addr
    @params[in] count
                The number of bytes to read
*/
/**************************************************************************/
void Adafruit_FRAM_I2C::read (uint32_t framAddr, uint8_t *values, uint32_t count)
{
  uint32_t hasRead = 0;
  uint32_t toRead = count;

  // Read in <= 32 byte blocks
  while (toRead > 0)
  {
	uint32_t addr = framAddr+hasRead;
	uint8_t pageBit = (addr & 0x10000) ? MB85RC_PAGE_BIT : 0;
    uint8_t block = (toRead > MULTIBYTE_BLOCK_RX_LEN) ? MULTIBYTE_BLOCK_RX_LEN : toRead;
    Wire.beginTransmission(i2c_addr | pageBit);
    writeAddress(addr);
    Wire.endTransmission();
    Wire.requestFrom(i2c_addr, block);
    while (Wire.available())
    {
      values[hasRead++] = Wire.read();
    }
    toRead -= block;
  }
#ifdef DEV_DBG
  if (hasRead != count)
  {
	  // Something went wrong...
	  Serial.print("I2C FRAM Read failed, ");
	  Serial.print(count);
	  Serial.print(" requested but got ");
	  Serial.println(hasRead);
  }
#endif
}

/**************************************************************************/
/*!
    @brief  Reads the Manufacturer ID and the Product ID frm the IC

    @params[out]  manufacturerID
                  The 12-bit manufacturer ID (Fujitsu = 0x00A)
    @params[out]  productID
                  The memory density (bytes 11..8) and proprietary
                  Product ID fields (bytes 7..0). Should be 0x510 for
                  the MB85RC256V.
*/
/**************************************************************************/
void Adafruit_FRAM_I2C::getDeviceID(uint16_t *manufacturerID, uint16_t *productID)
{
  uint8_t a[3] = { 0, 0, 0 };
  uint8_t results;

  Wire.beginTransmission(MB85RC_SLAVE_ID >> 1);
  Wire.write(i2c_addr << 1);
  results = Wire.endTransmission(false);

  Wire.requestFrom(MB85RC_SLAVE_ID >> 1, 3);
  a[0] = Wire.read();
  a[1] = Wire.read();
  a[2] = Wire.read();

  /* Shift values to separate manuf and prod IDs */
  /* See p.10 of http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC256V-DS501-00017-3v0-E.pdf */
  *manufacturerID = (a[0] << 4) + (a[1]  >> 4);
  *productID = ((a[1] & 0x0F) << 8) + a[2];
}

void Adafruit_FRAM_I2C::writeAddress(uint32_t addr)
{
  Wire.write((uint8_t)(addr >> 8));
  Wire.write((uint8_t)(addr & 0xFF));
}
