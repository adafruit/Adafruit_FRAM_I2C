
/* Example code to interrogate Adafruit SPI/I2C FRAM breakout for address size and storage capacity */

/* NOTE: This sketch will overwrite data already on the FRAM breakout */

#define USE_I2C	// Comment this out to use SPI
#ifndef USE_I2C
#define USE_SPI
#endif


#ifdef USE_I2C
#include "Adafruit_FRAM_I2C.h"
/* Example code for the Adafruit I2C FRAM breakout */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5.0V DC
   Connect GROUND to common ground */
   
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
uint16_t          framAddr = 0;
#define writeEnable(B)

#else // USE_SPI

#include <SPI.h>
#include "Adafruit_FRAM_SPI.h"
/* Example code for the Adafruit SPI FRAM breakout */

uint8_t FRAM_CS = 10;
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI();  // use hardware SPI
uint8_t FRAM_SCK = 13;
uint8_t FRAM_MISO = 12;
uint8_t FRAM_MOSI = 11;
//Or use software SPI, any pins!
//Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_SCK, FRAM_MISO, FRAM_MOSI, FRAM_CS);
#define writeEnable(B) fram.writeEnable(B);

#endif

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif


bool DEBUG_DATA = false;
uint8_t  addrSizeInBytes = 2; //Default to address size of two bytes
uint32_t memSize;
#define BUFFER_SIZE 256
uint32_t writeOffset = 0;
uint8_t writeBuffer[BUFFER_SIZE];
uint8_t readBuffer[BUFFER_SIZE];
int errorCount = 0;


int32_t readBack(uint32_t addr, int32_t data) {
  int32_t check = !data;
  int32_t wrapCheck, backup;
  fram.read(addr, (uint8_t*)&backup, sizeof(int32_t));
  writeEnable(true);
  fram.write(addr, (uint8_t*)&data, sizeof(int32_t));
  writeEnable(false);
  fram.read(addr, (uint8_t*)&check, sizeof(int32_t));
  fram.read(0, (uint8_t*)&wrapCheck, sizeof(int32_t));
  writeEnable(true);
  fram.write(addr, (uint8_t*)&backup, sizeof(int32_t));
  writeEnable(false);
  // Check for warparound, address 0 will work anyway
  if (wrapCheck==check)
    check = 0;
  return check;
}


bool testAddrSize(uint8_t addrSize) {
  fram.setAddressSize(addrSize);
  if (readBack(4, 0xbeefbead) == 0xbeefbead)
    return true;
  return false;
}


void setup(void) {
  #ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif

  Serial.begin(115200);
  addrSizeInBytes = 3;
#ifdef USE_I2C
  if (fram.begin(MB85RC_DEFAULT_ADDRESS, addrSizeInBytes)) {
#else // USE_SPI
  if (fram.begin(FRAM_CS, addrSizeInBytes)) {
#endif
    Serial.println("Found FRAM");
  } else {
    Serial.println("No FRAM found ... check your connections\r\n");
    while (1);
  }

/*
  if (testAddrSize(2))
    addrSizeInBytes = 2;
  else if (testAddrSize(3))
    addrSizeInBytes = 3;
  else if (testAddrSize(4))
    addrSizeInBytes = 4;
  else {
    Serial.println("FRAM can not be read/written with any address size\r\n");
    while (1);
  }
*/  
  memSize = 0;
  while (readBack(memSize, memSize) == memSize) {
    memSize += 256;
    //Serial.print("Block: #"); Serial.println(memSize/256);
  }
  
  Serial.print("FRAM address size is ");
  Serial.print(addrSizeInBytes);
  Serial.println(" bytes.");
  Serial.println("FRAM capacity appears to be..");
  Serial.print(memSize); Serial.println(" bytes");
  Serial.print(memSize/0x400); Serial.println(" kilobytes");
  Serial.print((memSize*8)/0x400); Serial.println(" kilobits");
  if (memSize >= (0x100000/8)) {
    Serial.print((memSize*8)/0x100000); Serial.println(" megabits");
  }

  for (int i=0; i<BUFFER_SIZE; i++)
    writeBuffer[i] = i;

  // Random seed, if A1 is not connected. 
  randomSeed(analogRead(A1)); 
}

void loop(void) {
  if (0 && writeOffset < memSize)
  {
    int i;
    uint32_t len = 1 + (random()%(BUFFER_SIZE-1));
    len = (writeOffset+len <= memSize) ? len : memSize-writeOffset;
    Serial.print("Write to address "); Serial.print(writeOffset);
    Serial.print(" with "); Serial.print(len); Serial.print(" bytes.. ");
    writeEnable(true);
    fram.write(writeOffset, (uint8_t*)writeBuffer, len);
    writeEnable(false);
    // Mark the read buffer with something we will recognise
    for (i=0; i<BUFFER_SIZE; i++)
      readBuffer[i] = 0xcd;
    fram.read(writeOffset, (uint8_t*)readBuffer, len);
    int oldErrorCount = errorCount;
    for (i=0; i<len; i++)
      if (readBuffer[i] != writeBuffer[i])
        errorCount++;
    Serial.print("..read-back ");
    Serial.println((oldErrorCount==errorCount) ? "OK" : "FAIL");
    writeOffset += len;
    
    if (writeOffset==memSize)
    {
      Serial.println("");
      Serial.print("Done with "); Serial.print(errorCount); Serial.println(" errors.");
    }
    else if (DEBUG_DATA)
    {
      uint8_t x = 0;
      Serial.print("Read "); Serial.print(len); Serial.println(" bytes.. ");
      for (i=0; i<len; i++)
      {
         Serial.print(readBuffer[i], HEX);
         if (++x==30)
         {
           Serial.println("");
           x = 0;
         }
         else
           Serial.print(",");
         
      }
      Serial.println("");
      delay(1000);
    }
  }
}