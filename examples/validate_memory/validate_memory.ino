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
  
  // Write the first byte to 0xAF
  uint8_t test = 0xAF;
  i2ceeprom.write(0x0, test);

  // Try to determine the size by writing a value and seeing if it changes the first byte
  Serial.println("Testing size!");

  uint32_t max_addr;
  for (max_addr = 1; max_addr < 0xFFFF; max_addr++) {
    if (i2ceeprom.read(max_addr) != test)
      continue; // def didnt wrap around yet

    // maybe wraped? try writing the inverse
    if (! i2ceeprom.write(max_addr, (byte)~test)) {
        Serial.print("Failed to write address 0x");
        Serial.println(max_addr, HEX);
    }

    // read address 0x0 again
    uint8_t val0 = i2ceeprom.read(0);

    // re-write the old value
    if (! i2ceeprom.write(max_addr, test)) {
        Serial.print("Failed to re-write address 0x");
        Serial.println(max_addr, HEX);
    }    

    // check if addr 0 was changed
    if (val0 == (byte)~test) {
      Serial.println("Found max address");
      break;
    }
  }
  Serial.print("This EEPROM can store ");
  Serial.print(max_addr);
  Serial.println(" bytes");
    
  // validate the memory
  Serial.println("Validating every address in memory");
  uint8_t val;
  for (uint16_t addr = 0; addr < max_addr; addr++) {
    if (addr % 32 == 0) {
      Serial.print("Testing address ");
      Serial.print(addr);
      Serial.print(" thru ");
      Serial.println(addr+31);
    }
    val = i2ceeprom.read(addr);
    
    i2ceeprom.write(addr, 0x55);
    if (i2ceeprom.read(addr) != 0x55) {
      Serial.print("Failed to write 0x55 to address 0x");
      Serial.println(addr, HEX);
    }
    i2ceeprom.write(addr, 0xAA);
    if (i2ceeprom.read(addr) != 0xAA) {
      Serial.print("Failed to write 0xAA to address 0x");
      Serial.println(addr, HEX);
    }
    
    i2ceeprom.write(addr, val);
    if (i2ceeprom.read(addr) != val) {
      Serial.print("Failed to write original value to address 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("Validated!");
}

void loop(void) {

}
