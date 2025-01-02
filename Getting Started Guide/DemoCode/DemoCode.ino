/*
DESCRIPTION:
Demo-Code for ROBO ESP32
This demo code is written in Arduino IDE and it serves
as an easy quality check when you first receive the board.

It plays a melody upon power up (slide power switch to ON)
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
#define led1 16  // Pin D16 from Robo ESP32
#define led2 17  // Pin D17 from Robo ESP32
#define led3 21  // Pin D21 from Robo ESP32
#define led4 22  // Pin D22 from Robo ESP32
#define led5 25  // Pin D25 from Robo ESP32
#define led6 26  // Pin D26 from Robo ESP32
#define led7 32  // Pin D32 from Robo ESP32
#define led8 33  // Pin D33 from Robo ESP32

/*
    Buzzer and User Buttons
*/
#define Buzzer 23
#define UserButton1 34
#define UserButton2 35

/*
    Motors
*/
#define M1A 12
#define M1B 13
#define M2A 14
#define M2B 27

CytronMD motor1(PWM_DIR, M1A, M1B);
CytronMD motor2(PWM_DIR, M2A, M2B);

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
int melody[] = { 659, 659, 0, 659, 0, 523, 659, 0, 784 };
int duration[] = { 150, 150, 150, 150, 150, 150, 150, 150, 200 };

void playMelody1() {
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    if (melody[i] == 0) {
      delay(duration[i]);  // Rest for the duration
    } else {
      tone(Buzzer, melody[i], duration[i]);
      delay(duration[i]);
      noTone(Buzzer);
    }
  }
}

void playMelody2() {
  for (int i = sizeof(melody) / sizeof(melody[0]) - 1; i >= 0; i--) {
    if (melody[i] == 0) {
      delay(duration[i]);  // Rest for the duration
    } else {
      tone(Buzzer, melody[i], duration[i]);
      delay(duration[i]);
      noTone(Buzzer);
    }
  }
}

void lightBlueLEDs(bool state) {
  int leds[] = { led1, led2, led3, led4, led5, led6, led7, led8 };
  for (int i = 0; i < 8; i++) {
    digitalWrite(leds[i], state ? HIGH : LOW);
  }
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
  playMelody1();
  for (int i = 0; i < 8; i++) {
    digitalWrite(led1 + i, HIGH);
    delay(100);
  }
  for (int i = 0; i < 8; i++) {
    digitalWrite(led1 + i, LOW);
  }
}

void animateNeoPixels() {
  static int state = 0;
  static uint32_t color = 0;

  switch (state) {
    case 0:
      if (color < 0x101010) color += 0x010101;
      else state++;
      break;
    case 1:
      if ((color & 0x00FF00) > 0) color -= 0x000100;
      else state++;
      break;
    case 2:
      if ((color & 0xFF0000) > 0) color -= 0x010000;
      else state++;
      break;
    case 3:
      if ((color & 0x00FF00) < 0x1000) color += 0x000100;
      else state++;
      break;
    case 4:
      if ((color & 0x0000FF) > 0) color -= 1;
      else state++;
      break;
    case 5:
      if ((color & 0xFF0000) < 0x100000) color += 0x010000;
      else state = 0;
      break;
  }
  strip.fill(color);
  strip.show();
}

void loop() {
  // NeoPixel animation
  animateNeoPixels();

  // Button D34
  if (digitalRead(UserButton1) == LOW) {
    playMelody1();
    lightBlueLEDs(true);

    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);

    motor1.setSpeed(50);   // Run motor1 at 50% speed
    motor2.setSpeed(-50);  // Run motor2 at -50% speed

    delay(1000);
  }

  // Button D35
  if (digitalRead(UserButton2) == LOW) {
    playMelody2();
    lightBlueLEDs(false);

    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    servo4.write(180);

    motor1.setSpeed(0);  // Brake motor1
    motor2.setSpeed(0);  // Brake motor2

    delay(1000);
  }
}
