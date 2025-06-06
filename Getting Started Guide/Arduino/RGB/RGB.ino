/*
DESCRIPTION:
This example code will use Robo ESP32 to light up the on-board RGB leds.
The RGB LEDs will sequentially changing their colors individually. 

AUTHOR   : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io

REFERENCE:
Adafruit_NeoPixel library link:
https://github.com/adafruit/Adafruit_NeoPixel/tree/master

*/

#include <Adafruit_NeoPixel.h>

// Declare pin number of the Neopixel LED and the number of the LED
const int RGBPin = 15;    // RGB pin for Robo ESP32
const int numPixels = 2;  //Number of onboard RGB pixels

// Initialize the NeoPixel RGB LEDs on pin D15
Adafruit_NeoPixel pixels(numPixels, RGBPin, NEO_GRB + NEO_KHZ800);

void setup() {

  pixels.begin();  // Initialize NeoPixel library
  pixels.clear();  // Set all pixel colors to 'off'
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void loop() {

  /* 

   pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
   NeoPixels are numbered from 0 to (number of pixels - 1) 
   
   */

  pixels.setPixelColor(0, pixels.Color(200, 0, 0));    // Red
  pixels.setPixelColor(1, pixels.Color(200, 0, 200));  // Magenta
  pixels.show();                                       // Send the updated pixel colors to the hardware.
  delay(1000);

  pixels.setPixelColor(0, pixels.Color(0, 200, 0));    // Green
  pixels.setPixelColor(1, pixels.Color(200, 200, 0));  // Yellow
  pixels.show();                                       // Send the updated pixel colors to the hardware.
  delay(1000);

  pixels.setPixelColor(0, pixels.Color(0, 0, 200));    // Blue
  pixels.setPixelColor(1, pixels.Color(0, 200, 200));  // Cyan
  pixels.show();                                       // Send the updated pixel colors to the hardware.
  delay(1000);

  pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // RGB turned off
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));  // RGB turned off
  pixels.show();                                   // Send the updated pixel colors to the hardware.
  delay(1000);
}