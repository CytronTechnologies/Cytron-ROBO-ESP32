/*
DESCRIPTION:
This example code will use Robo ESP32 to read the analog input from Maker Soil Module
and then show the result on serial monitor. This code also applicable to any analog sensor.

CONNECTION:

-GROVE 7-      | Maker Soil
Robo ESP32     | 
---------------------------
GND            | GND
VCC            | VCC
D39            | DIS
D36            | OUT

AUTHOR   : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

const int MakerSoil = 36;
int inputvalue = 0;
float readingvalue = 0;

void setup() {

  pinMode(MakerSoil, INPUT);
  Serial.begin(115200);
}


void loop() {

  // Read the input value from Maker Soil
  inputvalue = analogRead(MakerSoil);

  //Convert input ADC value, 3.3V times 2^8 resolution
  readingvalue = (inputvalue * 3.3 / 4096);

  Serial.print("Input Value : ");
  Serial.println(inputvalue);
  Serial.print("Reading Value : ");
  Serial.println(readingvalue);
  Serial.println("---------------------------");

  delay(1000);
}