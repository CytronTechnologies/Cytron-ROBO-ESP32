/*
DESCRIPTION:
This example code uses the Robo ESP32 to test the functionality of the IO LEDs.
Initially, all LEDs will be off. The LEDs will then blink alternately every 0.5 seconds, and finally, all LEDs will remain on.
This test ensures the LEDs on the Robo ESP32 are functioning correctly.

AUTHOR   : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

const int led1 = 16;  //LED Connected to pin D16
const int led2 = 17;  //LED Connected to pin D17
const int led3 = 21;  //LED Connected to pin D21
const int led4 = 22;  //LED Connected to pin D22
const int led5 = 25;  //LED Connected to pin D25
const int led6 = 26;  //LED Connected to pin D26
const int led7 = 32;  //LED Connected to pin D32
const int led8 = 33;  //LED Connected to pin D33


void setup() {

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
}

void loop() {
  //Turn OFF the LEDs
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);
  delay(1500);  //Wait for 1.5 seconds

  // Alternating blinking pattern for all LEDs
  digitalWrite(led1, HIGH);
  delay(500);
  digitalWrite(led1, LOW);
  delay(500);
  digitalWrite(led2, HIGH);
  delay(500);
  digitalWrite(led2, LOW);
  delay(500);
  digitalWrite(led3, HIGH);
  delay(500);
  digitalWrite(led3, LOW);
  delay(500);
  digitalWrite(led4, HIGH);
  delay(500);
  digitalWrite(led4, LOW);
  delay(500);
  digitalWrite(led5, HIGH);
  delay(500);
  digitalWrite(led5, LOW);
  delay(500);
  digitalWrite(led6, HIGH);
  delay(500);
  digitalWrite(led6, LOW);
  delay(500);
  digitalWrite(led7, HIGH);
  delay(500);
  digitalWrite(led7, LOW);
  delay(500);
  digitalWrite(led8, HIGH);
  delay(500);
  digitalWrite(led8, LOW);

  delay(1000);  // Pause for 1 seconds before next sequence

  // Set a new blinking pattern
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
  digitalWrite(led6, HIGH);
  digitalWrite(led7, HIGH);
  digitalWrite(led8, HIGH);

  delay(500);  //Delay for 0.5 seconds

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);

  delay(500);  //Delay for 0.5 seconds

  // Repeat the pattern again
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
  digitalWrite(led6, HIGH);
  digitalWrite(led7, HIGH);
  digitalWrite(led8, HIGH);

  delay(500);  //Delay for 0.5 seconds
}
