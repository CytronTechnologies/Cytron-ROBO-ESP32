/*
DESCRIPTION:
This example code will show how to use the User button on the Robo ESP32 as an Input.
In this code, the button will be used to control an on-board LED. 
If the button is pressed, the LED will light up for 0.5 second then turned off

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/


const int led1 = 21;
const int led2 = 22;
const int button1 = 34;
const int button2 = 35;

void setup() {

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
}

void loop() {

  // Check Button 1 (D34) is pressed

  if (!digitalRead(button1)) {
    // LED D21 is light up for 0.5s then turned off.
    digitalWrite(led1, HIGH);
    delay(500);
    digitalWrite(led1, LOW);
  }

  // Check Button 2 (D35) is pressed

  if (!digitalRead(button2)) {
    // LED D22 is light up for 0.5s then turned off.
    digitalWrite(led2, HIGH);
    delay(500);
    digitalWrite(led2, LOW);
  }
}
