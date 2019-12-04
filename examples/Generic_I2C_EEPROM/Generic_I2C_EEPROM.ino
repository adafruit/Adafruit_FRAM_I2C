#include <Wire.h>
#include "Adafruit_EEPROM_I2C.h"

/* Example code for the Adafruit I2C EEPROM breakout */

/* Connect SCL    to SCL
   Connect SDA    to SDA
   Connect VDD    to 3 - 5V DC
   Connect GROUND to common ground */
   
Adafruit_EEPROM_I2C i2ceeprom;

#define EEPROM_ADDR 0x50  // the default address!

void setup(void) {
  Serial.begin(115200);
  
  if (i2ceeprom.begin(0x50)) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C EEPROM");
  } else {
    Serial.println("I2C EEPROM not identified ... check your connections?\r\n");
    while (1) delay(10);
  }
  
  // Read the first byte
  uint8_t test = i2ceeprom.read8(0x0);
  Serial.print("Restarted "); Serial.print(test); Serial.println(" times");
  // Test write ++
  i2ceeprom.write8(0x0, test+1);
  
  // dump the first 256 bytes of memory
  uint8_t val;
  for (uint16_t addr = 0; addr < 256; addr++) {
    val = i2ceeprom.read8(addr);
    if ((addr % 32) == 0) {
      Serial.print("\n 0x"); Serial.print(addr, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (val < 0x10) 
      Serial.print('0');
    Serial.print(val, HEX); Serial.print(" ");
  }
}

void loop(void) {

}