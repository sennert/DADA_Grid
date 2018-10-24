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
#include "FastLED.h"


/*************************************************** 
  This example shows reading buttons and setting/clearing buttons in a loop
  "momentary" mode has the LED light up only when a button is pressed
  "latching" mode lets you turn the LED on/off when pressed

  Up to 12 matrices can be used but this example will show 4 or 1
 ****************************************************/

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE MOMENTARY 


DADA_GRID matrix0 = DADA_GRID();
DADA_GRID matrix1 = DADA_GRID();
DADA_GRID matrix2 = DADA_GRID();
DADA_GRID matrix3 = DADA_GRID();
DADA_GRID matrix4 = DADA_GRID();
DADA_GRID matrix5 = DADA_GRID();
DADA_GRID matrix6 = DADA_GRID();
DADA_GRID matrix7 = DADA_GRID();
//DADA_GRID matrix8 = DADA_GRID();
//DADA_GRID matrix9 = DADA_GRID();
//DADA_GRID matrix10 = DADA_GRID();
//DADA_GRID matrix11 = DADA_GRID();
//
//DADA_GRID_SET dadaGridSet =  DADA_GRID_SET(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7, &matrix8, &matrix9, &matrix10, &matrix11);
DADA_GRID_SET dadaGridSet =  DADA_GRID_SET(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7);

// set to however many you're working with here, up to 12
#define NUMTRELLIS 8

#define NUM_BUTTONS (NUMTRELLIS * 16)

//#define INTPIN A2

#define DATA_PIN    0
#define CLK_PIN     1
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define BRIGHTNESS  60

#define NUM_LEDS    192
#define NUM_LINES    12

CRGB leds[NUM_LEDS];


uint16_t addr_Array[12][16] = {
    {140,141,142,143, 156,157,158,159, 172,173,174,175, 188,189,190,191},
    {136,137,138,139, 152,153,154,155, 168,169,170,171, 184,185,186,187},
    {132,133,134,135, 148,149,150,151, 164,165,166,167, 180,181,182,183},
    {128,129,130,131, 144,145,146,147, 160,161,162,163, 176,177,178,179},

    {115,114,113,112,  99, 98, 97, 96,  83, 82, 81, 80,  79, 75, 71, 67}, 
    {119,118,117,116, 103,102,101,100,  87, 86, 85, 84,  78, 74, 70, 66},
    {123,122,121,120, 107,106,105,104,  91, 90, 89, 88,  77, 73, 69, 65},
    {127,126,125,124, 111,110,109,108,  95, 94, 93, 92,  76, 72, 68, 64},
    
    { 12, 13, 14, 15,  28, 29, 30, 31,  44, 45, 46, 47,  60, 61, 62, 63},
    {  8,  9, 10, 11,  24, 25, 26, 27,  40, 41, 42, 43,  56, 57, 58, 59},
    {  4,  5,  6,  7,  20, 21, 22, 23,  36, 37, 38, 39,  52, 53, 54, 55},
    {  0,  1,  2,  3,  16, 17, 18, 19,  32, 33, 34, 35,  48, 49, 50, 51}
};


boolean buttonState[NUM_BUTTONS];

uint8_t d = 130;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t xPos = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("DADA GRID Demo");

  
//  // INT pin requires a pullup
//  pinMode(INTPIN, INPUT);
//  digitalWrite(INTPIN, HIGH);
    dadaGridSet.begin(0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27);  // or four!


  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setTemperature(ClearBlueSky);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid (leds, NUM_LEDS, CRGB::DarkGray);
  FastLED.show();
  
  Serial.println("finished Setup");


}


void loop() {
  fill_solid (leds, NUM_LEDS, CRGB::DarkGray);

  delay(30); // 30ms delay is required, dont remove me!
  
  if (MODE == MOMENTARY) {
    // If a button was just pressed or released...
    if (dadaGridSet.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numButtons; i++) {
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
      for (uint8_t i=0; i<numButtons; i++) {
        // if it was pressed...
        if (dadaGridSet.isKeyPressed(i)) {
          Serial.print("v"); Serial.println(i);
          leds[i]= CRGB::Pink;
      }
      // tell the dadaGridSet to set the LEDs we requested

    }
  }
}

  FastLED.show();
}
