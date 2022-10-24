#include "Adafruit_EEPROM_I2C.h"
#include "Adafruit_FRAM_I2C.h"

/* Example code for the Adafruit I2C EEPROM/FRAM breakout */

/* Connect SCL    to SCL
   Connect SDA    to SDA
   Connect VDD    to 3 - 5V DC
   Connect GROUND to common ground */
   
Adafruit_EEPROM_I2C i2ceeprom;
//Adafruit_FRAM_I2C i2ceeprom;

#define EEPROM_ADDR 0x50  // the default address!

void setup(void) {
  Serial.begin(115200);
  
  if (i2ceeprom.begin(0x50)) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C EEPROM");
  } else {
    Serial.println("I2C EEPROM not identified ... check your connections?\r\n");
    while (1) delay(10);
  }

  float f = 3.141592;
  uint8_t buffer[4];  // floats are 4 bytes!
  memcpy(buffer, (void *)&f, 4);
  
  Serial.println("Writing float to address 0x00");
  i2ceeprom.write(0x00, buffer, 4);

  i2ceeprom.read(0x00, buffer, 4);
  memcpy((void *)&f, buffer, 4);
  Serial.print("Read back float value: ");
  Serial.println(f, 8);
}

void loop(void) {

}
