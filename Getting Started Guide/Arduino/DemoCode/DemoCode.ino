/*
DESCRIPTION:
Demo-Code for ROBO ESP32
This demo code is written in Arduino IDE and it serves
as an easy quality check when you first receive the board.

It plays a Happy Birthday melody upon power up (slide power switch to ON)
and shows running lights (blue LEDs) at the same time.
Then the two RGB LEDs will animate the colors, while the 
program checking push buttons' state, repeatedly.

Press D34 button to play a short melody, lights up all 
blue LEDs, move servo motors to 0 degree and run DC motors
at 50% and -50% speeds.
Press D35 button to play another melody, turn off all blue 
LEDs, move servo motors to 180 degree & brake DC motors.

ROBO ESP32 also has four DC motors quick test buttons 
built-in. You may press the onboard M1A, M1B, M2A or M2B 
push buttons to run your motors without writing any code.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#include "CytronMotorDriver.h"

/*
    LEDs
*/
#define led1 16  // Pin D16 on Robo ESP32
#define led2 17  // Pin D17 on Robo ESP32
#define led3 21  // Pin D21 on Robo ESP32
#define led4 22  // Pin D22 on Robo ESP32
#define led5 25  // Pin D25 on Robo ESP32
#define led6 26  // Pin D26 on Robo ESP32
#define led7 32  // Pin D32 on Robo ESP32
#define led8 33  // Pin D33 on Robo ESP32

/*
    Buzzer and User Buttons
*/
#define Buzzer 23       // Buzzer Pin D23 on Robo ESP32
#define UserButton1 34  // User Button 1 Pin D34 on Robo ESP32
#define UserButton2 35  // User Button 2 Pin D35 on Robo ESP32

/*
    Motors
*/
#define M1A 12
#define M1B 13
#define M2A 14
#define M2B 27

CytronMD motor1(PWM_PWM, M1A, M1B);
CytronMD motor2(PWM_PWM, M2A, M2B);

/*
    Servos
*/
const int servoPin1 = 4;
const int servoPin2 = 5;
const int servoPin3 = 18;
const int servoPin4 = 19;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

const int Neopixel = 15;  // Pin connected to the NeoPixel data input
const int numPixels = 2;  // Number of pixels (2 for two NeoPixels)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, Neopixel, NEO_GRB + NEO_KHZ800);

/* Melody Notes and Durations */
int melody[] = { 392, 392, 440, 392, 523, 494, 392, 392, 440, 392, 587, 523 };

// Short melody note durations for fast tempo (milliseconds)
int duration[] = { 100, 100, 200, 200, 200, 270, 100, 100, 200, 200, 200, 270 };

void playHappyBirthday() {
  int notesCount = sizeof(melody) / sizeof(melody[0]);  // Calculate number of notes
  for (int i = 0; i < notesCount; i++) {
    if (melody[i] == 0) {
      noTone(Buzzer);  // If the note is 0, turn off the buzzer
    } else {
      tone(Buzzer, melody[i], duration[i]);  // Play the note with the specified duration
    }
    delay(duration[i] * 1.3);  // Slight delay between notes for rhythm
  }
}

void playSound1() {
  // Play a short melody when the User Button 34 is pressed
  tone(Buzzer, 262, 100);  // Play C note
  delay(100);
  tone(Buzzer, 659, 100);  // Play E note
  delay(100);
  tone(Buzzer, 784, 100);  // Play G note
  delay(100);
  noTone(Buzzer);  // Turn off the buzzer
}

void playSound2() {
  // Play a short melody when the User Button 35 is pressed
  tone(Buzzer, 784, 100);  // Play C note
  delay(100);
  tone(Buzzer, 659, 100);  // Play E note
  delay(100);
  tone(Buzzer, 262, 100);  // Play G note
  delay(100);
  noTone(Buzzer);  // Turn off the buzzer
}

void lightBlueLEDs(bool state) {
  int leds[] = { led1, led2, led3, led4, led5, led6, led7, led8 };
  for (int i = 0; i < 8; i++) {
    digitalWrite(leds[i], state ? HIGH : LOW);
  }
}

void turnOffNeoPixels() {
  strip.fill(0x000000);  // Black color (all LEDs off)
  strip.show();          // Send the updated color to the strip
}

void setup() {
  Serial.begin(115200);

  // LED setup
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);

  // Buzzer and buttons
  pinMode(Buzzer, OUTPUT);
  pinMode(UserButton1, INPUT_PULLUP);
  pinMode(UserButton2, INPUT_PULLUP);

  // Servos
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);

  strip.begin();
  strip.show();

  // Power-on animation
  playHappyBirthday();
  delay(100);
  motor2.setSpeed(-100);
  delay(100);
  motor2.setSpeed(100);
  delay(100);
  motor2.setSpeed(0);
  motor1.setSpeed(-100);
  delay(100);
  motor1.setSpeed(100);
  delay(100);
  motor1.setSpeed(0);
  motor2.setSpeed(0);

  int leds[] = { 16, 17, 21, 22, 25, 26, 32, 33 };  // LED pins

  // Set all LED pins as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Turn on LEDs one by one
  for (int i = 0; i < 8; i++) {
    digitalWrite(leds[i], HIGH);
    delay(100);
    digitalWrite(leds[i], LOW);
  }
  delay(100);
}

void animateNeoPixels() {
  static int state = 0;
  static uint8_t red = 255, green = 0, blue = 0;  // Start with red color
  static unsigned long lastUpdate = 0;            // Last time LEDs were updated
  const unsigned long interval = 10;              // Interval in milliseconds
  const float brightnessFactor = 0.2;             // Brightness factor (0.0 to 1.0)

  // Only update if the interval has passed
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();  // Update the time

    switch (state) {
      case 0:  // Red to Yellow (increase Green)
        if (green < 255) green++;
        else state++;  // Move to next state
        break;

      case 1:  // Yellow to Green (decrease Red)
        if (red > 0) red--;
        else state++;  // Move to next state
        break;

      case 2:  // Green to Cyan (increase Blue)
        if (blue < 255) blue++;
        else state++;  // Move to next state
        break;

      case 3:  // Cyan to Blue (decrease Green)
        if (green > 0) green--;
        else state++;  // Move to next state
        break;

      case 4:  // Blue to Magenta (increase Red)
        if (red < 255) red++;
        else state++;  // Move to next state
        break;

      case 5:  // Magenta to Red (decrease Blue)
        if (blue > 0) blue--;
        else state = 0;  // Reset to initial state
        break;
    }

    // Scale RGB values to control brightness
    uint8_t scaledRed = red * brightnessFactor;
    uint8_t scaledGreen = green * brightnessFactor;
    uint8_t scaledBlue = blue * brightnessFactor;

    // Combine scaled RGB values and display
    uint32_t color = (scaledRed << 16) | (scaledGreen << 8) | scaledBlue;
    strip.fill(color);
    strip.show();  // Display RGB
  }
}

bool neoPixelsOff = false;

void loop() {
  // NeoPixel animation
  if (!neoPixelsOff) {
    animateNeoPixels();
  }

  // Button D34
  if (digitalRead(UserButton1) == LOW) {
    playSound1();
    lightBlueLEDs(true);  // Light up all GPIO LEDs

    servo1.write(0);  // Servo pin D4 in 0 degree position
    servo2.write(0);  // Servo pin D5 in 0 degree position
    servo3.write(0);  // Servo pin D18 in 0 degree position
    servo4.write(0);  // Servo pin D19 in 0 degree position

    motor1.setSpeed(100);   // Run motor1 at 50% speed
    motor2.setSpeed(-100);  // Run motor2 at -50% speed
    delay(200);
    motor1.setSpeed(-100);  // Run motor1 at 50% speed
    motor2.setSpeed(100);   // Run motor2 at -50% speed
    delay(200);
    motor1.setSpeed(100);   // Run motor1 at 50% speed
    motor2.setSpeed(-100);  // Run motor2 at -50% speed
    delay(200);
    motor1.setSpeed(-100);  // Run motor1 at 50% speed
    motor2.setSpeed(100);   // Run motor2 at -50% speed
    delay(200);
    motor1.setSpeed(100);   // Run motor1 at 50% speed
    motor2.setSpeed(-100);  // Run motor2 at -50% speed
    delay(200);
    motor1.setSpeed(-100);  // Run motor1 at 50% speed
    motor2.setSpeed(100);   // Run motor2 at -50% speed
    delay(200);
    motor1.setSpeed(100);   // Run motor1 at 50% speed
    motor2.setSpeed(-100);  // Run motor2 at -50% speed
    delay(200);

    neoPixelsOff = false;  // Turn NeoPixels back on
  }

  // Button D35
  if (digitalRead(UserButton2) == LOW) {
    playSound2();
    lightBlueLEDs(false);

    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    servo4.write(180);

    motor1.setSpeed(0);  // Brake motor1
    motor2.setSpeed(0);  // Brake motor2
    turnOffNeoPixels();
    neoPixelsOff = true;
    delay(1000);
  }
}