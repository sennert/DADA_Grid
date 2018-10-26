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
 
//#include <i2c_t3.h>  // include the i2c written by Brian "nox771", github.com/nox771/i2c_t3
#include "DADA_GRID.h"
#include <Wire.h>

/* By default, the APA102 library uses pinMode and digitalWrite
 * to write to the LEDs, which works on all Arduino-compatible
 * boards but might be slow.  If you have a board supported by
 * the FastGPIO library and want faster LED updates, then install
 * the FastGPIO library and uncomment the next two lines: */
// #include <FastGPIO.h>
// #define APA102_USE_FAST_GPIO

#include <APA102.h>

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE LATCHING 


//***** DADA GRID INIT ****
DADA_GRID     matrix[8];    // up to 12 with 2 i2c busses
DADA_GRID_SET dadaGridSet =  DADA_GRID_SET(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7);
//DADA_GRID_SET dadaGridSet =  DADA_GRID_SET(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7, &matrix8, &matrix9, &matrix10, &matrix11); //for 12 PCBS


//***** DADA GRID INIT ****
DADA_GRID     matrix[12];


// set to however many you're working with here, up to 12
#define NUMTRELLIS 8

#define NUM_BUTTONS (NUMTRELLIS * 16)

//#define INTPIN A2

#define DATA_PIN    6
#define CLK_PIN     7
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define BRIGHTNESS  4 // Set the brightness to use (the maximum is 31).


// Create an object for writing to the LED strip.
APA102<DATA_PIN, CLK_PIN> ledStrip;

#define NUM_LEDS    192
#define NUM_LINES   8               // 4,8,12 possible horizontal lines

rgb_color leds[NUM_LEDS];


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


/* Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return rgb_color(r, g, b);
}


void setup() {
  Serial.begin(115200);
  Serial.println("DADA GRID Demo");

  
//  // INT pin requires a pullup
//  pinMode(INTPIN, INPUT);
//  digitalWrite(INTPIN, HIGH);

    dadaGridSet.begin(0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27);  // or four!

  for(int i=0; i<(NUM_BUTTONS-1); i++){
    buttonState[i] = 0;
  }

  for(uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = hsvToRgb(20+(i*2), 255, 255);
  }
  ledStrip.write(leds, NUM_LEDS, BRIGHTNESS);

  
  Serial.println("finished Setup");
}


void loop() {
//    fill_solid (leds, NUM_LEDS, CRGB::DarkGray);

    // If a button was just pressed or released...
    if (dadaGridSet.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<(NUM_BUTTONS-1); i++) {
        // if it was pressed, turn it on
        if (dadaGridSet.justPressed(i)) {
          Serial.print("v"); Serial.println(i);
          if (buttonState[i])  {
            buttonState[i] = false;
//            leds[i] = CRGB::DarkGrey;}
          }
          else {
            buttonState[i] = true; 
//            leds[i] = CRGB::Blue;
          }
        }
        // if it was released, turn it off
        if (dadaGridSet.justReleased(i)) {
          Serial.print("^"); Serial.println(i);
          //buttonState[i] = false;    
        }
      }
    }
  
    for(int y=0; y<12; y++){
      for(int x=0; x<16; x++){
        if (buttonState[(iForXY(x,y))]) {
          leds[(iForXY(x,y))] =  hsvToRgb(((y*6)-20), 255, 255);
        } else {
          leds[(iForXY(x,y))] =  hsvToRgb(150, 10, 100);
        }
      }
    } 

  showYBar(xPos);
    ledStrip.write(leds, NUM_LEDS, BRIGHTNESS);

}

uint8_t iForXY(uint16_t _x,uint16_t _y){
  return addr_Array[_y][_x];
}

void rainbowLineMatrix(){
   for(int y=0; y<12; y++){
    for(int x=0; x<16; x++){
        int io = iForXY(x,y);
//      leds[io] = hsvToRgb( ((y*5)+gHue), 255, 255);
    }
  } 
}

void showYBar(uint8_t _xpos){
  for(int y = 0; y < (NUM_LINES); y++){
      leds[(iForXY(_xpos,y))] = hsvToRgb(0, 0, 250);
    }  
}
