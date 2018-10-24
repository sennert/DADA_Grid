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
#define BRIGHTNESS  50

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
  Serial.begin(115200);
  Serial.println("DADA GRID Demo");

  
//  // INT pin requires a pullup
//  pinMode(INTPIN, INPUT);
//  digitalWrite(INTPIN, HIGH);
    dadaGridSet.begin(0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27);  // or four!

  for(int i=0; i<(NUM_BUTTONS-1); i++){
    buttonState[i] = 0;
  }
  randomSeed(42);
  
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setTemperature(ClearBlueSky);
  FastLED.setBrightness(BRIGHTNESS);
  
  for(int i = 240; i>=2; (i-=4)){
    gHue = i;
    rainbowLineMatrix();
    drawLogo3();
    FastLED.show();
    delay(15);
  }
  
  fill_solid (leds, NUM_LEDS, CRGB::DarkGray);
  FastLED.show();
  delay(500);
  
  for(int i = 500; i>=0; (i-=3)){
    gHue = i;
    drawLogoRainbow();
    FastLED.show();
    delay(15);
  }

  
  gHue = 0;
  
  rainbowLineMatrix();
  FastLED.show();
  
  generateRandomStep();
  generateRandomStep();
  generateRandomStep();
  generateRandomStep();
  fill_solid (leds, NUM_LEDS, CRGB::DimGray);
  showYBar(xPos);
  FastLED.show();
  
  Serial.println("finished Setup");
}


void loop() {
    fill_solid (leds, NUM_LEDS, CRGB::DarkGray);

    // If a button was just pressed or released...
    if (dadaGridSet.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<(NUM_BUTTONS-1); i++) {
        // if it was pressed, turn it on
        if (dadaGridSet.justPressed(i)) {
          Serial.print("v"); Serial.println(i);
          if (buttonState[i])  {buttonState[i] = false;leds[i] = CRGB::DarkGrey;}
          else {buttonState[i] = true; leds[i] = CRGB::Blue;}
        }
        // if it was released, turn it off
        if (dadaGridSet.justReleased(i)) {
          Serial.print("^"); Serial.println(i);
          //buttonState[i] = false;    
        }
      }
    }
  
//    for(int y=0; y<12; y++){
//      for(int x=0; x<16; x++){
//        if (buttonState[(iForXY(x,y))]) {
//          leds[(iForXY(x,y))] = CHSV( ((y*6)-20), 255, 255);
//        }
//      }
//    } 

  showYBar(xPos);
  FastLED.show();
  
  EVERY_N_MILLISECONDS(120) {xPos++; if(xPos>=16) xPos = 0;}
  EVERY_N_SECONDS(2) {generateRandomStep();generateRandomStep();}
}

uint8_t iForXY(uint16_t _x,uint16_t _y){
  return addr_Array[_y][_x];
}

void rainbowLineMatrix(){
   for(int y=0; y<12; y++){
    for(int x=0; x<16; x++){
        int io = iForXY(x,y);
      leds[io] = CHSV( ((y*5)+gHue), 255, 255);
    }
  } 
}

void drawLogo3(){
  leds[(iForXY(1,8))] = CRGB::Black;
  leds[(iForXY(2,8))] = CRGB::Black;
  leds[(iForXY(6,8))] = CRGB::Black;
  leds[(iForXY(8,8))] = CRGB::Black;
  leds[(iForXY(9,8))] = CRGB::Black;
  leds[(iForXY(13,8))] = CRGB::Black;
//
  leds[(iForXY(1,7))] = CRGB::Black;
  leds[(iForXY(3,7))] = CRGB::Black;
  leds[(iForXY(5,7))] = CRGB::Black;
  leds[(iForXY(6,7))] = CRGB::Black;
  leds[(iForXY(8,7))] = CRGB::Black;
  leds[(iForXY(10,7))] = CRGB::Black;
  leds[(iForXY(12,7))] = CRGB::Black;
  leds[(iForXY(13,7))] = CRGB::Black;
//
  leds[(iForXY(1,6))] = CRGB::Black;
  leds[(iForXY(3,6))] = CRGB::Black;
  leds[(iForXY(5,6))] = CRGB::Black;
  leds[(iForXY(7,6))] = CRGB::Black;
  leds[(iForXY(8,6))] = CRGB::Black;
  leds[(iForXY(10,6))] = CRGB::Black;
  leds[(iForXY(12,6))] = CRGB::Black;
  leds[(iForXY(14,6))] = CRGB::Black;
//
  leds[(iForXY(1,5))] = CRGB::Black;
  leds[(iForXY(3,5))] = CRGB::Black;
  leds[(iForXY(5,5))] = CRGB::Black;
  leds[(iForXY(7,5))] = CRGB::Black;
  leds[(iForXY(8,5))] = CRGB::Black;
  leds[(iForXY(10,5))] = CRGB::Black;
  leds[(iForXY(12,5))] = CRGB::Black;
  leds[(iForXY(14,5))] = CRGB::Black;
//
  leds[(iForXY(1,4))] = CRGB::Black;
  leds[(iForXY(3,4))] = CRGB::Black;
  leds[(iForXY(4,4))] = CRGB::Black;
  leds[(iForXY(5,4))] = CRGB::Black;
  leds[(iForXY(6,4))] = CRGB::Black;
  leds[(iForXY(7,4))] = CRGB::Black;
  leds[(iForXY(8,4))] = CRGB::Black;
  leds[(iForXY(10,4))] = CRGB::Black;
  leds[(iForXY(11,4))] = CRGB::Black;
  leds[(iForXY(12,4))] = CRGB::Black;
  leds[(iForXY(13,4))] = CRGB::Black;
  leds[(iForXY(14,4))] = CRGB::Black;
//
  leds[(iForXY(1,3))] = CRGB::Black;
  leds[(iForXY(2,3))] = CRGB::Black;
  leds[(iForXY(4,3))] = CRGB::Black;
  leds[(iForXY(7,3))] = CRGB::Black;
  leds[(iForXY(8,3))] = CRGB::Black;
  leds[(iForXY(9,3))] = CRGB::Black;
  leds[(iForXY(11,3))] = CRGB::Black;
  leds[(iForXY(14,3))] = CRGB::Black;
}

void drawLogoRainbow(){
  leds[(iForXY(1,8))] = CHSV(((8*5)+gHue), 255, 255);
  leds[(iForXY(2,8))] = CHSV(((8*5)+gHue), 255, 255);
  leds[(iForXY(6,8))] = CHSV(((8*5)+gHue), 255, 255);
  leds[(iForXY(8,8))] = CHSV(((8*5)+gHue), 255, 255);
  leds[(iForXY(9,8))] = CHSV(((8*5)+gHue), 255, 255);
  leds[(iForXY(13,8))] = CHSV(((8*5)+gHue), 255, 255);
//
  leds[(iForXY(1,7))] = CHSV(((7*5)+gHue), 255, 255);
  leds[(iForXY(3,7))] = CHSV(((7*5)+gHue), 255, 255);
  leds[(iForXY(5,7))] = CHSV(((7*5)+gHue), 255, 255);
  leds[(iForXY(6,7))] = CHSV(((7*5)+gHue), 255, 255);
  leds[(iForXY(8,7))] = CHSV(((7*5)+gHue), 255, 255);
  leds[(iForXY(10,7))] = CHSV(((7*5)+gHue), 255, 255);
  leds[(iForXY(12,7))] = CHSV(((7*5)+gHue), 255, 255);
  leds[(iForXY(13,7))] = CHSV(((7*5)+gHue), 255, 255);
//
  leds[(iForXY(1,6))] = CHSV(((6*5)+gHue), 255, 255);
  leds[(iForXY(3,6))] = CHSV(((6*5)+gHue), 255, 255);
  leds[(iForXY(5,6))] = CHSV(((6*5)+gHue), 255, 255);
  leds[(iForXY(7,6))] = CHSV(((6*5)+gHue), 255, 255);
  leds[(iForXY(8,6))] = CHSV(((6*5)+gHue), 255, 255);
  leds[(iForXY(10,6))] = CHSV(((6*5)+gHue), 255, 255);
  leds[(iForXY(12,6))] = CHSV(((6*5)+gHue), 255, 255);
  leds[(iForXY(14,6))] = CHSV(((6*5)+gHue), 255, 255);
//
  leds[(iForXY(1,5))] = CHSV(((5*5)+gHue), 255, 255);
  leds[(iForXY(3,5))] = CHSV(((5*5)+gHue), 255, 255);
  leds[(iForXY(5,5))] = CHSV(((5*5)+gHue), 255, 255);
  leds[(iForXY(7,5))] = CHSV(((5*5)+gHue), 255, 255);
  leds[(iForXY(8,5))] = CHSV(((5*5)+gHue), 255, 255);
  leds[(iForXY(10,5))] = CHSV(((5*5)+gHue), 255, 255);
  leds[(iForXY(12,5))] = CHSV(((5*5)+gHue), 255, 255);
  leds[(iForXY(14,5))] = CHSV(((5*5)+gHue), 255, 255);
//
  leds[(iForXY(1,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(3,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(4,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(5,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(6,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(7,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(8,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(10,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(11,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(12,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(13,4))] = CHSV(((4*5)+gHue), 255, 255);
  leds[(iForXY(14,4))] = CHSV(((4*5)+gHue), 255, 255);
//
  leds[(iForXY(1,3))] = CHSV(((3*5)+gHue), 255, 255);
  leds[(iForXY(2,3))] = CHSV(((3*5)+gHue), 255, 255);
  leds[(iForXY(4,3))] = CHSV(((3*5)+gHue), 255, 255);
  leds[(iForXY(7,3))] = CHSV(((3*5)+gHue), 255, 255);
  leds[(iForXY(8,3))] = CHSV(((3*5)+gHue), 255, 255);
  leds[(iForXY(9,3))] = CHSV(((3*5)+gHue), 255, 255);
  leds[(iForXY(11,3))] = CHSV(((3*5)+gHue), 255, 255);
  leds[(iForXY(14,3))] = CHSV(((3*5)+gHue), 255, 255);
}

void showYBar(uint8_t _xpos){
  for(int y = 0; y < (NUM_LINES); y++){
      leds[(iForXY(_xpos,y))] = CRGB::White;
    }  
}

void generateRandomStep(){
    int uX = random(0,15);
    int uY = random(0,11);
    
    int _i = iForXY(uX,uY);
    
    if (buttonState[_i])  buttonState[_i] = false;
    else buttonState[_i] = true;  
}
