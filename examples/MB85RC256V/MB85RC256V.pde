#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"

/* Example code for the Adafruit I2C FRAM breakout */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5.0V DC
   Connect GROUND to common ground */
   
Adafruit_FRAM_I2C fram     = Adafruit_FRAM_I2C();
uint16_t          framAddr = 0;

void setup(void) {
  Serial.begin(9600);
  
  if (fram.begin()) {
    Serial.println("Found I2C FRAM");
  } else {
    Serial.println("No I2C FRAM found ... check your connections\r\n");
    while (1);
  }
  
  // Test write
  // fram.write8(MB85RC_ADDRESS, framAddr, 0xAB);
}

void loop(void) {
  uint8_t value = fram.read8(MB85RC_ADDRESS, framAddr);
  Serial.print("0x"); Serial.print(framAddr, HEX); Serial.print(": ");
  Serial.println(value, HEX);
  framAddr++;
  delay(1000);
}
