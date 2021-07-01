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
  uint8_t test = i2ceeprom.read(0x0);
  Serial.print("Restarted "); Serial.print(test); Serial.println(" times");
  // Test write ++
  test++;
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
    
  // dump the memory
  uint8_t val;
  for (uint16_t addr = 0; addr < max_addr; addr++) {
    val = i2ceeprom.read(addr);
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
