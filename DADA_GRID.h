/*************************************************** 
  This is a library for DADA Grid 4x4 PCB  w/TCA9555 and 16x APA102 

  These displays use I2C to communicate, 2 pins are required to  
  interface
  
  The Code is based on the Library for Adafruit Trellis.
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
  
  rewritten by Steffen Sennert for DADA 
 ****************************************************/

#ifndef _DADA_GRID_H_
#define _DADA_GRID_H_
#include "Wire.h"
//#include <i2c_t3.h>  // include the i2c written by Brian "nox771", github.com/nox771/i2c_t3 //for teensy second i2c
//#define WIRE_PINS   	I2C_PINS_18_19		// only for Teensy
//#define WIRE1_PINS   	I2C_PINS_29_30		// only for teensy

// Set _BV if not already set (eg. Arudiono DUE, Arduino Zero Pro)
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

//#include <i2c_t3.h>  // include the i2c written by Brian "nox771", github.com/nox771/i2c_t3 //for teensy second i2c


#define NXP_INPUT      (0)  // For NXP9555
#define NXP_OUTPUT     (2)  // See data sheet
#define NXP_INVERT     (4)  // for details...
#define NXP_CONFIG     (6)

// this is the raw HT16K33 controller
class DADA_GRID {
 public:
  DADA_GRID(void);
  void begin(uint8_t _addr, uint8_t _i2cp);
  bool readSwitches(void);

  bool isKeyPressed(uint8_t k);
  bool wasKeyPressed(uint8_t k);

  bool justPressed(uint8_t k);
  bool justReleased(uint8_t k);

 private:
  uint16_t newData, oldData;

  bool buttonState[16];
  bool buttonState_Old[16];

  uint8_t i2c_addr;
  uint8_t i2c_port;
};


// control a large # at a time!
class DADA_GRID_SET {
 public:
  DADA_GRID_SET(DADA_GRID *matrix0, 
		      DADA_GRID *matrix1=0,
		      DADA_GRID *matrix2=0,
		      DADA_GRID *matrix3=0,
		      DADA_GRID *matrix4=0,
		      DADA_GRID *matrix5=0,
		      DADA_GRID *matrix6=0,
		      DADA_GRID *matrix7=0,
		      DADA_GRID *matrix8=0,
		      DADA_GRID *matrix9=0,
		      DADA_GRID *matrix10=0,
		      DADA_GRID *matrix11=0);  
void begin(uint8_t _addr0 = 0x20, uint8_t _addr1 = 0x21,
	     uint8_t _addr2 = 0x22, uint8_t _addr3 = 0x23,
	     uint8_t _addr4 = 0x24, uint8_t _addr5 = 0x25,
	     uint8_t _addr6 = 0x26, uint8_t _addr7 = 0x27,
		 uint8_t _addr8 = 0x20, uint8_t _addr9 = 0x21,
	     uint8_t _addr10 = 0x22, uint8_t _addr11 = 0x23);


  bool isKeyPressed(uint8_t k);
  bool wasKeyPressed(uint8_t k);

  bool readSwitches(void);
  bool justPressed(uint8_t k);
  bool justReleased(uint8_t k);

 private:
  DADA_GRID *matrices[12];
  uint8_t _nummatrix;
};

#endif // _DADA_GRID_H_
