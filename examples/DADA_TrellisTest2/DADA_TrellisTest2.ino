/*************************************************** 
  This is a test example for the Adafruit Trellis w/HT16K33

  Designed specifically to work with the Adafruit Trellis 
  ----> https://www.adafruit.com/products/1616
  ----> https://www.adafruit.com/products/1611

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <i2c_t3.h>  // include the i2c written by Brian "nox771", github.com/nox771/i2c_t3
#include "DADA_GRID.h"

/*************************************************** 
  This example shows reading buttons and setting/clearing buttons in a loop
  "momentary" mode has the LED light up only when a button is pressed
  "latching" mode lets you turn the LED on/off when pressed

  Up to 12 matrices can be used but this example will show 4 or 1
 ****************************************************/

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE LATCHING 


DADA_GRID matrix0 = DADA_GRID();
DADA_GRID matrix1 = DADA_GRID();
DADA_GRID matrix2 = DADA_GRID();
DADA_GRID matrix3 = DADA_GRID();
DADA_GRID matrix4 = DADA_GRID();
DADA_GRID matrix5 = DADA_GRID();
DADA_GRID matrix6 = DADA_GRID();
DADA_GRID matrix7 = DADA_GRID();
DADA_GRID matrix8 = DADA_GRID();
DADA_GRID matrix9 = DADA_GRID();
DADA_GRID matrix10 = DADA_GRID();
DADA_GRID matrix11 = DADA_GRID();

DADA_GRID_SET dadaGridSet =  DADA_GRID_SET(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7, &matrix8, &matrix9, &matrix10, &matrix11);

// set to however many you're working with here, up to 12
#define NUMTRELLIS 12

#define numKeys (NUMTRELLIS * 16)
#define INTPIN A2

void setup() {
  Serial.begin(9600);
  Serial.println("DADA GRID Demo");

//  // INT pin requires a pullup
//  pinMode(INTPIN, INPUT);
//  digitalWrite(INTPIN, HIGH);
    dadaGridSet.begin(0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x70,0x71,0x72,0x73);  // or four!

}


void loop() {
  delay(30); // 30ms delay is required, dont remove me!
  
  if (MODE == MOMENTARY) {
    // If a button was just pressed or released...
    if (dadaGridSet.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
  // if it was pressed, turn it on
  if (dadaGridSet.justPressed(i)) {
    Serial.print("v"); Serial.println(i);
  } 
  // if it was released, turn it off
  if (dadaGridSet.justReleased(i)) {
    Serial.print("^"); Serial.println(i);
  }
      }
      // tell the dadaGridSet to set the LEDs we requested
      //dadaGridSet.writeDisplay();
    }
  }

  if (MODE == LATCHING) {
    // If a button was just pressed or released...
    if (dadaGridSet.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
        if (dadaGridSet.justPressed(i)) {
          Serial.print("v"); Serial.println(i);
      }
      // tell the dadaGridSet to set the LEDs we requested

    }
  }
}
