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
  uint16_t num;
  
  Serial.begin(115200);
  
  if (i2ceeprom.begin(0x50)) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C EEPROM");
  } else {
    Serial.println("I2C EEPROM not identified ... check your connections?\r\n");
    while (1) delay(10);
  }

  String s = "Hello world!";
  num = i2ceeprom.writeObject(0x00, s);
  Serial.print("Wrote a string with ");
  Serial.print(num);
  Serial.println(" bytes");

  String s2;
  i2ceeprom.readObject(0x00, s2);
  Serial.print("Read back string value: ");
  Serial.print(s2);
}

void loop(void) {

}
