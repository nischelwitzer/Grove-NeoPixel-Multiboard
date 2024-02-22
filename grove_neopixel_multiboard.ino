
//
// Grove NeoPixel Board
// 4 different NeoPixel Pinouts
// 
// author: Alexander Nischelwitzer, FHJ IMA/AIM DAT DMT
// last update: 27.03.2023
// 
// Button with NeoPixel 
// OneButton Lib https://github.com/mathertel/OneButton
// https://www.adafruit.com/product/3869 needs 12V
// 

#include "rgb_lcd.h"
rgb_lcd mylcd;
#include <Adafruit_NeoPixel.h>

// ---------------------------------------------------------------------------------

const int ledCtrlPin    = 2;  // connector on D2
const int buttonCtrlPin = 3; // button PIN
int  buttonCtrlState = 0;
int  buttonCtrlPrev  = 0;

const byte button1Pin = 8; // Button with NeoPixel
bool  button1State = 0;
bool  button1Prev  = 0;

const byte button0Pin = A2; // Animation ON/OFF analog Schalter
bool  button0State = 0;
bool  button0Prev  = 0;

// ##### init ######################################################################
// ---------------------------------------------------------------------------------
 
const int colorR = 255;
const int colorG = 255;
const int colorB = 0;

// ################################################################################
// Prototypes with default parameters/arguments 

void rainbowCycleAll(uint8_t wait, int repeating = 3);

// ################################################################################

byte effect = -1;

// https://www.adafruit.com/product/3630
// Adafruit NeoPixel LED Dots Strand - 20 LEDs at 2" Pitch

#define NEOPIN0              4
#define NEOPIN1              5
#define NEOPIN2              6
#define NEOPIN3              7

#define NUM_NEO_PIXEL0       10
#define NUM_NEO_PIXEL1       34
#define NUM_NEO_PIXEL2       20
#define NUM_NEO_PIXEL3        1

#define MAXEFFECT            14

// http://wiki.seeedstudio.com/Grove-LED_ring/
// C:\Users\nisch\OneDrive\Dokumente\Arduino\libraries 
// WS2813 Digital RGB LED Ring. 

// Adafruit_NeoPixel stripNeoPixel = Adafruit_NeoPixel(NUM_NEO_PIXEL, NEOPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripNeoPixel0 = Adafruit_NeoPixel(NUM_NEO_PIXEL0, NEOPIN0, NEO_RGB  + NEO_KHZ800); // 10er
Adafruit_NeoPixel stripNeoPixel1 = Adafruit_NeoPixel(NUM_NEO_PIXEL1, NEOPIN1, NEO_RGB  + NEO_KHZ800); // stripe
Adafruit_NeoPixel stripNeoPixel2 = Adafruit_NeoPixel(NUM_NEO_PIXEL2, NEOPIN2, NEO_RGB  + NEO_KHZ800); // flower - 20er Ring
Adafruit_NeoPixel stripNeoPixel3 = Adafruit_NeoPixel(NUM_NEO_PIXEL3, NEOPIN3, NEO_RGB  + NEO_KHZ800); // button strand single

Adafruit_NeoPixel stripArray[] = {stripNeoPixel0, stripNeoPixel1, stripNeoPixel2, stripNeoPixel3};
#define NUMSTRIPS (sizeof(stripArray)/sizeof(stripArray[0]))

long myCount = 0;

const uint32_t cBLACK    = 0x00000000;
const uint32_t cWHITE    = 0x00FFFFFF;

const uint32_t cRED      = 0x0000FF00; // W GRB
const uint32_t cGREEN    = 0x00FF0000;
const uint32_t cBLUE     = 0x000000FF;

const uint32_t cYELLOW   = 0x00FFFF00;
const uint32_t cCYAN     = 0x00FF00FF;
const uint32_t cMAGENTA  = 0x0000FFFF;

const uint32_t cSTART    = 0x02000000;
const uint32_t cDARKBLUE = 0x00000066;

const uint32_t cSELECTED = 0x0000FF00; // Red
const uint32_t cOFF      = 0x00000000; // White
const uint32_t cON       = 0x00ffffff; // Red

String dataSend;
char charSend[30];

// Timer Functions

int mySecPuls = 0;
int oldSecPuls = 1;
int timeout = 0;

#define seconds() (millis()/1000)
#define minutes() (millis()/60000)

// ##### init ######################################################################

void setup() 
{
  Serial.begin(115200); // 9600 115200
  Serial.println("<Arduino Grove NeoPixel starting UP>");

  Serial.println("");
  Serial.print(F("Starting UP File: "));
  Serial.println(__FILE__);
  Serial.print(F("Combiled by AK Nischelwitzer"));
  Serial.println(": " __DATE__ " @ " __TIME__);  // predefined macors
  Serial.println(F("--------------------------------------------------------------------------------"));
 
  // ....................................................... 
  // *******************************************************
    
  pinMode(ledCtrlPin,    OUTPUT);
  pinMode(buttonCtrlPin, INPUT);   

  pinMode(button0Pin, INPUT);
  pinMode(button1Pin, INPUT_PULLUP);
  
  mylcd.begin(16, 2);
  mylcd.setRGB(colorR, colorG, colorB);
  mylcd.setCursor(0, 0); 
  mylcd.print("NeoPxWood E:## D");
  mylcd.setCursor(0, 1); 
  mylcd.print("S#             x");
  
  // ## NEOPIXEL INIT
  pinMode(NEOPIN0, INPUT_PULLUP);
  pinMode(NEOPIN1, INPUT_PULLUP);
  pinMode(NEOPIN2, INPUT_PULLUP);
  pinMode(NEOPIN3, INPUT_PULLUP);

  stripNeoPixel0.setBrightness(128);
  stripNeoPixel0.begin(); 
  twoColor(&stripNeoPixel0, cRED, cBLUE, 0, 1); // 10er
  stripNeoPixel0.show(); // to off

  stripNeoPixel1.setBrightness(255);
  stripNeoPixel1.begin(); 
  twoColor(&stripNeoPixel1, cRED, cBLUE, 0, 1); // long
  stripNeoPixel1.show(); // to off
  
  stripNeoPixel2.setBrightness(128);  // flower
  stripNeoPixel2.begin(); 
  twoColor(&stripNeoPixel2, cRED, cBLUE, 0, 1); // flower
  stripNeoPixel2.show(); // to off
  
  stripNeoPixel3.setBrightness(128); // single button
  stripNeoPixel3.begin(); 
  twoColor(&stripNeoPixel3, cRED, cBLUE, 0, 1); // button
  stripNeoPixel3.show(); // to off

  // colorWipe(cBLUE, 250, 1000);
  // colorWipe(cGREEN, 20, 10);

 
  sprintf(charSend,"allNeoPixel: %03d", neoPixelAll()); 
  Serial.println(charSend);

  mylcd.setCursor(3, 1);
  //              ###########
  mylcd.print(   "Starting-UP");

  rainbowCycleAll( 0, 1);
  twoColorAll(cBLUE, cRED, 100, 50);
  rainbowCycleAll( 0, 2);
  // theaterChaseAll(cBLUE, 50, 30, 4); 
  colorWipeAll(cYELLOW, 0, 1000); 
}

// ################################################################################
// ################################################################################

 
void loop() 
{
    
  // ###################################################
  // LED Test-BUTTON (optional: with LED)
      
  buttonCtrlState = !digitalRead(buttonCtrlPin);
  if (buttonCtrlState != buttonCtrlPrev) 
  {
    dataSend = "S"+String(buttonCtrlState); // Button S Start 
    Serial.println(dataSend);  // send to pc/unity
    digitalWrite(ledCtrlPin, buttonCtrlState); 
    buttonCtrlPrev = buttonCtrlState;
    
    mylcd.setCursor(1, 1);
    mylcd.print(buttonCtrlState);

    if (buttonCtrlState) // only effect if pressed
    {
      effect++;
      if (effect> MAXEFFECT) effect=0;
      doEffect(effect);
    }
  }

  button1State = !digitalRead(button1Pin); // NeoButton "Herberstein"
  if (button1State != button1Prev) 
  {
    dataSend = "NeoButton"+String(button1State);
    Serial.println(dataSend);  
    button1Prev = button1State;
    if (button1State) // only effect if pressed
    {
      mylcd.setCursor(3, 1);
      //              ###########
      mylcd.print(   "ButtonNeoPx");
      rainbowCycleAll( 0, 1);

      // showPixelCol(&stripNeoPixel3, 0, cYELLOW);
      // effect++;
      // if (effect> MAXEFFECT) effect=0;
      // doEffect(effect);
    }
  }

  button0State = (analogRead(button0Pin)>512?1:0);
  if (button0State) {
    if (button0Prev != button0State)  // only show changes
    {
      dataSend = "auto"+String(button0State);
      Serial.println(dataSend);  
      button0Prev = button0State;
    }

    effect++;
    if (effect> MAXEFFECT) effect=0;
    doEffect(effect);
    delay(1000);
  }


  // ###################################################
  // ## pulsing state on LCD display
  // ###################################################
       
  int mySec = seconds() % 60;
  int myMin = minutes();

  mySecPuls = seconds() % 2;
  mylcd.setCursor(15, 1);
      
  if (mySecPuls != oldSecPuls)
  {    
    oldSecPuls = mySecPuls;

    if (mySecPuls == 1) 
    {
      mylcd.print("x");
      digitalWrite(ledCtrlPin, true);
      showPixelCol(&stripNeoPixel3, 0, cRED);
      colorWipe(&stripNeoPixel2, cYELLOW,   0, 0);
      timeout++;
    }
    else
    {
      mylcd.print("o");  
      digitalWrite(ledCtrlPin, false);
      showPixelCol(&stripNeoPixel3, 0, cBLUE);   
      colorWipe(&stripNeoPixel2, cGREEN,   0, 0);
      timeout++;
    }  
  }

  // Timeout - no interaction 
  if (timeout > 45)
  {
    sprintf(charSend,"timeout"); 
    Serial.println(charSend);
    showPixelCol(&stripNeoPixel3, 0, cWHITE);   
    timeout = 0;
  }

  delay(100);
}

// ################################################################################
// ################################################################################
// ################################################################################


void brain() // pos from 0..1023
{
  char   dataInfo[7];
    
  int brainNr = -99;
  int brainValue = -1;
  char *arg = NULL;

  arg = uduino.next();
  if (arg != NULL)
    brainNr = atoi(arg);
 
  arg = uduino.next();
  if (arg != NULL)
    brainValue = atoi(arg);
 
 
}

// ################################################################################


void doEffect(byte which)
{

   char   dataInfo[7];
   sprintf(dataInfo,"E:%02d", which);
   mylcd.setCursor(10, 0);
   mylcd.print(dataInfo);

  /*
  ** neuen kommands 3, 7, 8, 9, 10
  * 
  *  0..
  *  
  *  1..black
     2..grün (Standard) – so sollte es ja jetzt laufen
     3..rot (ungültige karte, nicht im 30er set)
     4..blau – not used
     5..gelb – not used
     6..weiss – not used
 
     Spezialkarten
     7..grün blinken (startup)
     8..rot blinken (shutdown)
     9..blau blicken (reboot) 
     10..gelb blinken (king card)
     11..weiss blinken – not used 
  **
  */

  mylcd.setCursor(3, 1);

  switch(which){    
    case 0: //              xxxxxxxxxxx
            mylcd.print(   "ColorWipe-G");
            colorWipeAll(cGREEN,   50, 300);
            break;
    case 1: //              xxxxxxxxxxx
            mylcd.print(   "ColorWipe-B");
            colorWipeCC(&stripNeoPixel3, cRED,   10, 100);
            colorWipeCC(&stripNeoPixel2, cGREEN,   10, 100);
            colorWipeCC(&stripNeoPixel1, cBLUE,   10, 100);
            colorWipeCC(&stripNeoPixel0, cYELLOW,   10, 100);
            break;
    case 2: //              xxxxxxxxxxx
            mylcd.print(   "ColorPart  ");
            colorPart(&stripNeoPixel0,  0,  5, cRED,    100); // 10 15 9
            colorPart(&stripNeoPixel0,  5,  5, cYELLOW, 100); // 10 15 9

            colorPart(&stripNeoPixel1,  0, 10, cRED,    100); // 10 15 9
            colorPart(&stripNeoPixel1, 10, 15, cYELLOW, 100);
            colorPart(&stripNeoPixel1, 25,  9, cRED,    100);

            // flower
            // colorPart(&stripNeoPixel2,  0,  7, cYELLOW, 100);
            // colorPart(&stripNeoPixel2,  7,  2, cRED,    100);

            colorPart(&stripNeoPixel2,  0,  5, cYELLOW, 100);
            colorPart(&stripNeoPixel2,  5,  5, cRED,    100);
            colorPart(&stripNeoPixel2, 10,  5, cBLUE, 100);
            colorPart(&stripNeoPixel2, 15,  5, cMAGENTA,    100);

            colorPart(&stripNeoPixel3,  0,  1, cRED,    100);
            break;             
    case 3: //              xxxxxxxxxxx
            mylcd.print(   "ColorWipe  ");
            colorWipe(&stripNeoPixel0, cMAGENTA,   50, 300);  // 
            colorWipe(&stripNeoPixel1, cMAGENTA,   50, 300);  // 
            colorWipe(&stripNeoPixel2, cMAGENTA,   50, 300);  // 
            colorWipe(&stripNeoPixel3, cMAGENTA,   50, 300);  // 
            break;  
    case 4: //              xxxxxxxxxxx
            mylcd.print(   "ColorWipeCC");
            colorWipeCC(&stripNeoPixel3, cWHITE,   10, 10);  // white
            colorWipeCC(&stripNeoPixel2, cWHITE,   10, 10);  // white
            colorWipeCC(&stripNeoPixel1, cWHITE,   10, 10);  // white
            colorWipeCC(&stripNeoPixel0, cWHITE,   10, 10);  // white
            break;  
    case 5: //              xxxxxxxxxxx
            mylcd.print(   "TheaterC--B");
            // theaterChase(&stripNeoPixel1, cBLUE, 50, 30, 4); // blue - blinken, wait and repeat
            theaterChaseAll(cYELLOW, 30, 30, 4); // blue - blinken, wait and repeat
            break;
    case 6: 
            mylcd.print(   "TheaterCRG-");
            theaterChaseAll(cMAGENTA, 30, 30, 4); // yellow 
            break;
    case 7: //              xxxxxxxxxxx    
            mylcd.print(   "Rainbow--40");
            rainbowAll(20);
            rainbowAll(0);
            break;
    case 8: //              xxxxxxxxxxx     
            mylcd.print(   "RainbowCycl");
            // rainbowCycle(&stripNeoPixel1, 10);  // for KING CARD - nice color change
            rainbowCycleAll(10, 1);  // very nice color change
            rainbowCycleAll( 0);  
            break;
            
    case 9: //              xxxxxxxxxxx     
            mylcd.print(   "theaChasRai");
            theaterChaseRainbowAll(30);
            // theaterChaseRainbow(&stripNeoPixel1, 20);
            break;
    case 10://              xxxxxxxxxxx     
            mylcd.print(   "theaChasLon"); 
            theaterChaseLong(&stripNeoPixel1, cYELLOW, 100, 25); // Green,  wait, cycle (rotate)
            break;
    case 11://              xxxxxxxxxxx     
            mylcd.print(   "theaChasLon"); 
            theaterChaseLong(&stripNeoPixel1, cRED, 50, 25);
            break;
    case 12://              xxxxxxxxxxx     
            mylcd.print(   "twoColor   "); 
            twoColor(&stripNeoPixel0, cWHITE, cRED, 100, 40); // red white
            twoColor(&stripNeoPixel1, cWHITE, cRED, 100, 30); // red white
            twoColor(&stripNeoPixel2, cWHITE, cRED, 100, 50); // red white
            twoColor(&stripNeoPixel3, cWHITE, cRED, 100, 60); // red white
            break;
    case 13: 
            mylcd.print(   "twoColor   "); 
            twoColorAll(cMAGENTA, cCYAN, 100,  30); 
            break;
    case 14://              xxxxxxxxxxx     
            mylcd.print(   "colorPart   "); 
            colorPart(&stripNeoPixel1, 0,  10, cYELLOW, 100); // 10 15 9
            colorPart(&stripNeoPixel1, 10, 15, cBLUE,  100);
            colorPart(&stripNeoPixel1, 25,  9, cYELLOW,  100);
            break;


  }
}

// ################################################################################
// ################################################################################
// ################################################################################
// 
// MASTER NEOPIXEL MULTI Code
// 29.03.2023
//
// ################################################################################
// ################################################################################
// ################################################################################

void colorWipe(Adafruit_NeoPixel *strip, uint32_t c, uint8_t wait, int waiting) {
  for(uint16_t i=0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
    strip->show();
    delay(wait);
  }
  delay(waiting);
}

// counter clockwise
void colorWipeCC(Adafruit_NeoPixel *strip, uint32_t c, uint8_t wait, int waiting) {
  for(uint16_t i=0; i < strip->numPixels(); i++) {
    strip->setPixelColor(strip->numPixels()-i-1, c);
    strip->show();
    delay(wait);
  }
  delay(waiting);
}

void twoColor(Adafruit_NeoPixel *strip, uint32_t c1, uint32_t c2, long wait, int repeating) 
{
  bool toggle = true;
  // all OFF
  for(uint16_t i=0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, strip->Color(0, 0, 0)); 
  }
  strip->show();

  for(uint16_t i=0; i < repeating; i++) {
    for(uint16_t i=0; i < strip->numPixels(); i++) {
      strip->setPixelColor(i, (((i+toggle)%2)?c1:c2)); 
    }
    toggle = !toggle;
    strip->show();
    delay(wait);
  }
}

// ################################################################################

void rainbow(Adafruit_NeoPixel *strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(strip, (i+j) & 255));
    }
    strip->show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel *strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(strip, ((i * 256 / strip->numPixels()) + j) & 255));
    }
    strip->show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel *strip, uint32_t c, int wait, int repeating, int steps) {
  for (int j=0; j<repeating; j++) { //do X cycles of chasing
    for (int q=0; q < steps; q++) {  
      
      for (int i=0; i < strip->numPixels(); i+=steps) {
        strip->setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip->show();

      delay(wait);

      for (int i=0; i < strip->numPixels(); i=i+steps) {
        strip->setPixelColor(i+q, 0);                      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel *strip, uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, Wheel(strip,  (i+j) % 255));    //turn every third pixel on
      }
      strip->show();
      delay(wait);
      for (int i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights.
void theaterChaseLong(Adafruit_NeoPixel *strip, uint32_t c, uint8_t wait, uint8_t rotate) {
  for (int j=0; j<rotate; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip->show();

      delay(wait);

      for (int i=0; i < strip->numPixels(); i=i+3) {
        strip->setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


// Fill the dots one after the other with a color
void statusColor(Adafruit_NeoPixel *strip, uint32_t c, long wait) {
  for(uint16_t i=0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
  }
  strip->show();
  delay(wait);
}

void colorPart(Adafruit_NeoPixel *strip, int led, int num, uint32_t c, long wait) 
{  
  for(uint16_t i=led; i < led+num; i++) {
    int pixel = i;
    if (pixel >= strip->numPixels()) pixel -= strip->numPixels();
    strip->setPixelColor(pixel, c);
    strip->show();
    delay(wait);
  }
  delay(10);
}


void showPixel(Adafruit_NeoPixel *strip, int led, uint8_t r, uint8_t g, uint8_t b)
{
  strip->setPixelColor(led, strip->Color(r, g, b));
  strip->show(); 
}

void showPixelCol(Adafruit_NeoPixel *strip, int led, uint32_t col)
{
  strip->setPixelColor(led, col);
  strip->show();
}

// ################################################################################
// ################################################################################
// NeoPixel Helper

uint32_t myColor(Adafruit_NeoPixel *strip, int myPos) { 
  return strip->Color(myPos * 3, 255 - myPos * 3, 0);
}

byte numPixel(Adafruit_NeoPixel *strip) 
{  
  return strip->numPixels();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

uint32_t Wheel(Adafruit_NeoPixel *strip, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


// ################################################################################
// ################################################################################
// NeoPixel Array Tools

int neoPixelAll() {
  int counter = 0;
  for(int i=0; i<NUMSTRIPS; i++)
    counter += stripArray[i].numPixels(); 
  return counter;
}

void colorWipeAll(uint32_t c, uint8_t wait, int waiting) 
{
  for(int run=0; run<NUMSTRIPS; run++)
  {
    for(uint16_t i=0; i < stripArray[run].numPixels(); i++) 
    {
      stripArray[run].setPixelColor(i, c);
      stripArray[run].show();
      delay(wait);
    }
  }
  delay(waiting);
}

void twoColorAll(uint32_t c1, uint32_t c2, long wait, int repeating) 
{
  bool toggle = true;
  for(uint16_t j=0; j < repeating; j++) 
  {
    for(int run=0; run<NUMSTRIPS; run++)
    {
      for(uint16_t i=0; i < stripArray[run].numPixels(); i++) 
      {
        stripArray[run].setPixelColor(i, (((i+toggle)%2)?c1:c2)); 
      }
      stripArray[run].show();
    }
    delay(wait);
    toggle = !toggle;
  }
}


void rainbowAll(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    
    for(int run=0; run<NUMSTRIPS; run++)
    {
      for(i=0; i<stripArray[run].numPixels(); i++) {
        stripArray[run].setPixelColor(i, Wheel(&stripArray[run], (i+j) & 255));
      }
      stripArray[run].show();
    }
    
    delay(wait);
  }
}

void rainbowCycleAll(uint8_t wait, int repeating) {
  uint16_t i, j, r;

  for (int r=0; r<repeating; r++) 
  {
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
      for(int run=0; run<NUMSTRIPS; run++)
      {
        for(i=0; i< stripArray[run].numPixels(); i++) {
          stripArray[run].setPixelColor(i, Wheel(&stripArray[run], ((i * 256 / stripArray[run].numPixels()) + j) & 255));
        }
        stripArray[run].show();
      }
      delay(wait);
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbowAll(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for(int run=0; run<NUMSTRIPS; run++)
      {
        for (int i=0; i < stripArray[run].numPixels(); i=i+3) 
        {
          stripArray[run].setPixelColor(i+q, Wheel(&stripArray[run],  (i+j) % 255));    //turn every third pixel on
        }
        stripArray[run].show();
        delay(wait);
        for (int i=0; i < stripArray[run].numPixels(); i=i+3) 
        {
          stripArray[run].setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
    }
  }
}

//Theatre-style crawling lights.
void theaterChaseAll(uint32_t c, int wait, int repeating, int steps) {
  for (int j=0; j<repeating; j++) { //do X cycles of chasing
    for (int q=0; q < steps; q++) {  
      for(int run=0; run<NUMSTRIPS; run++)
      {
        for (int i=0; i < stripArray[run].numPixels(); i+=steps) {
          stripArray[run].setPixelColor(i+q, c);    //turn every third pixel on
        }
        stripArray[run].show();
        delay(wait);
        for (int i=0; i < stripArray[run].numPixels(); i=i+steps) {
          stripArray[run].setPixelColor(i+q, 0);                      //turn every third pixel off
        }
      }
    }
  }
}

// ################################################################################
// ################################################################################


