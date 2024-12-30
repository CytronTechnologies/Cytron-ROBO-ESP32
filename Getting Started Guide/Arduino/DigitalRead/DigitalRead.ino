/*
DESCRIPTION:
This example code will use Robo ESP32 to read the digital input from Ultrasonic sensor
and then show the result on serial monitor. This code also applicable to any digital sensor.

CONNECTION:

-GROVE 2-      | Maker Soil
Robo ESP32     | 
---------------------------
GND            | GND
VCC            | VCC
D26            | TRIG
D25            | ECHO

AUTHOR   : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

// Defines pins numbers
const int trigPin = 26;  // Connect to pin 26 from Robo ESP32
const int echoPin = 25;  // Connect to pin 25 from Robo ESP32

// Defines variables
long duration;
int distance;


void setup() {

  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  Serial.begin(115200);      // Starts the serial communication
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
