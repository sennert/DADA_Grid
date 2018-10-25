/*************************************************** 
  This is a library for DADA Grid 4x4 PCB  w/TCA9655 and 16x APA!02 
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

#include "DADA_GRID.h"
#include "Wire.h"
//#include <i2c_t3.h>  // include the i2c written by Brian "nox771", github.com/nox771/i2c_t3 //for teensy second i2c
//#define WIRE_PINS   	I2C_PINS_18_19		// only for Teensy
//#define WIRE1_PINS   	I2C_PINS_29_30		// only for teensy

#define NXP_INPUT      (0)  // For NXP9555
#define NXP_OUTPUT     (2)  // See data sheet
#define NXP_INVERT     (4)  // for details...
#define NXP_CONFIG     (6)


#define unit8_t byte

DADA_GRID::DADA_GRID(void) {

}

void DADA_GRID::begin(uint8_t _addr = 0x20, uint8_t _i2cp = 0) {
  i2c_addr = _addr;
  i2c_port = _i2cp;

  for(int i=0; i<16; i++){
    buttonState[i] = 0;
    buttonState_Old[i] = 0;
  }

  if(i2c_port==0 || i2c_port==1)
  {
	  	  Wire.begin();
		  Wire.beginTransmission(_addr);
		  Wire.write(NXP_CONFIG);
		  Wire.write(0xff & 0xFFFF);  // low byte
		  Wire.write(0xFFFF >> 8);    // high byte
		  Wire.endTransmission();

		  Wire.beginTransmission(_addr);
		  Wire.write(NXP_INVERT);
		  Wire.write(0xff & 0xFFFF);  // low byte
		  Wire.write(0xFFFF >> 8);    // high byte
		  Wire.endTransmission();
  } else if(i2c_port==2)
  {
/*	 	  Wire1.begin();
		  Wire1.beginTransmission(_addr);
		  Wire1.write(NXP_CONFIG);
		  Wire1.write(0xff & 0xFFFF);  // low byte
		  Wire1.write(0xFFFF >> 8);    // high byte
		  Wire1.endTransmission();

		  Wire1.beginTransmission(_addr);
		  Wire1.write(NXP_INVERT);
		  Wire1.write(0xff & 0xFFFF);  // low byte
		  Wire1.write(0xFFFF >> 8);    // high byte
		  Wire1.endTransmission();
		  */
  }
}


/* 
Helper button functions, the data is updated every readSwitches() call!
*/

bool DADA_GRID::isKeyPressed(uint8_t k) {
  	if (k > 15) return false;
	return buttonState[k];
}
bool DADA_GRID::wasKeyPressed(uint8_t k) {
  	if (k > 15) return false;
	return buttonState_Old[k];
}

bool DADA_GRID::justPressed(uint8_t k) {
  return (isKeyPressed(k) & !wasKeyPressed(k));
}
bool DADA_GRID::justReleased(uint8_t k) {
  return (!isKeyPressed(k) & wasKeyPressed(k));
}




/* 
   Gets the switch memory data and updates the last/current read
*/
bool DADA_GRID::readSwitches(void) {

  newData = 0;

  if(i2c_port<=1){ 

	  Wire.beginTransmission(i2c_addr);
	  Wire.write(NXP_INPUT);
	  Wire.endTransmission();

	  Wire.requestFrom(i2c_addr, 4);
	  if (Wire.available()) {
	    newData = Wire.read();
	  }
	  if (Wire.available()) {
	    newData |= (Wire.read() << 8);
	  }
	} else if (i2c_port==2){ 

/*	  Wire1.beginTransmission(i2c_addr);
	  Wire1.write(NXP_INPUT);
	  Wire1.endTransmission();

	  Wire1.requestFrom(i2c_addr, 4);
	  if (Wire1.available()) {
	    newData = Wire1.read();
	  }
	  if (Wire1.available()) {
	    newData |= (Wire1.read() << 8);
	  }*/
	}


	//if(newData != oldData){

	    if (newData != -1) {
	    	memcpy(buttonState_Old, buttonState, sizeof(buttonState));		// copy old State to OLDSTATE

	     	//if (newData != 0) {								// no sure if necessary
		        for (int i = 0; i < 16; i++) {
		          if (newData & (1 << i)) {
		            // Button is pressed
					buttonState[i] = true;
		          } else {
		            // Button is released
					buttonState[i] = false;
		          }
		        }
	      	//} else {
	        //Serial.println("Button released");
	     // }


    	//oldData = newData; 
    	return true;
   	    }

	//}
  	return false;  
}

/*
boolean DADA_GRID::readSwitches(void) {
  memcpy(buttonState_Old, buttonState, sizeof(buttonState));

    if(i2c_port<=1){ 


		  for (uint8_t i=0; i<16; i++) {
		    if (buttonState_Old[i] != buttonState[i]) {

		      return true;
		    }
		  }

  	} else if (i2c_port==2){


		  for (uint8_t i=0; i<6; i++) {
		    if (oldData != data) {
		       for (uint8_t j=0; j<6; j++) {
		       }
		      return true;
		    }
		  }
  	}

  	return false;  
}
*/


/*************************************************************************/

// Maximum 8 matrices (3 address pins)
DADA_GRID_SET::DADA_GRID_SET(DADA_GRID *matrix0, 
				      DADA_GRID *matrix1,
				      DADA_GRID *matrix2,
				      DADA_GRID *matrix3,
				      DADA_GRID *matrix4,
				      DADA_GRID *matrix5,
				      DADA_GRID *matrix6,
				      DADA_GRID *matrix7, 
				      DADA_GRID *matrix8,
				      DADA_GRID *matrix9,
				      DADA_GRID *matrix10,
				      DADA_GRID *matrix11) {  matrices[0] = matrix0;
  matrices[1] = matrix1;
  matrices[2] = matrix2;
  matrices[3] = matrix3;
  matrices[4] = matrix4;
  matrices[5] = matrix5;
  matrices[6] = matrix6;
  matrices[7] = matrix7;
  matrices[8] = matrix8;
  matrices[9] = matrix9;
  matrices[10] = matrix10;
  matrices[11] = matrix11;
  _nummatrix = 0;

  for (uint8_t i=0; i<12; i++) {
    if (matrices[i] != 0)
      _nummatrix= i+1;
    else break;
  }
}



void DADA_GRID_SET::begin(uint8_t addr0, uint8_t addr1,
				uint8_t addr2, uint8_t addr3,
				uint8_t addr4, uint8_t addr5,
				uint8_t addr6, uint8_t addr7,
				uint8_t addr8, uint8_t addr9,
				uint8_t addr10, uint8_t addr11) {  
	uint8_t addrs[12] = {addr0, addr1, addr2, addr3, addr4, addr5, addr6, addr7, addr8, addr9, addr10, addr11};

	if(_nummatrix<=8) {
	  for (uint8_t i=0; i<_nummatrix; i++) {
	    if (matrices[i] != 0)
	      matrices[i]->begin(addrs[i],0);

	  }
	} else {
	  for (uint8_t i=0; i<8; i++) {
	    if (matrices[i] != 0)
	      matrices[i]->begin(addrs[i],1);
	  }  
	  for (uint8_t i=8; i<_nummatrix; i++) {
	    if (matrices[i] != 0)
	      matrices[i]->begin(addrs[i],2);
	  }
	}  
}


/* 
Helper button functions, the data is updated every readSwitches() call!
*/

bool DADA_GRID_SET::isKeyPressed(uint8_t k) {
  if (k > 191) return false;
  uint8_t matrix, key;

  // determine submatrix #
  matrix = k / 16;
  key = k % 16;

  // not that many matrices!
  if (matrix >= _nummatrix) {return false;}

  return  matrices[matrix]->isKeyPressed(key);
}

bool DADA_GRID_SET::wasKeyPressed(uint8_t k) {
  if (k > 191) return false;
  uint8_t matrix, key;
  
  // determine submatrix #
  matrix = k / 16;
  key = k % 16;

  // not that many matrices!
  if (matrix >= _nummatrix) return false;

  return  matrices[matrix]->wasKeyPressed(key);
}

bool DADA_GRID_SET::justPressed(uint8_t k) {
  if (k > 191) return false;
  uint8_t matrix, key;
  
  // determine submatrix #
  matrix = k / 16;
  key = k % 16;

  // not that many matrices!
  if (matrix >= _nummatrix) return false;  
  return (matrices[matrix]->isKeyPressed(key) & !matrices[matrix]->wasKeyPressed(key));
}

bool DADA_GRID_SET::justReleased(uint8_t k) {
  if (k > 191) return false;
  uint8_t matrix, key;
  
  // determine submatrix #
  matrix = k / 16;
  key = k % 16;

  // not that many matrices!
  if (matrix >= _nummatrix) return false;  
  return (!matrices[matrix]->isKeyPressed(key) & matrices[matrix]->wasKeyPressed(key));
}


/* 
   Gets the switch memory data and updates the last/current read
*/
bool DADA_GRID_SET::readSwitches(void) {
  for (uint8_t i=0; i<_nummatrix; i++) {
    if (matrices[i] != 0)
      matrices[i]->readSwitches();
  }
  return true;
}
