/*
DESCRIPTION:
This example code will use the the buzzzer on the Robo ESP32 to play the tones. 
The User button also used in this code. Upon startup, a short melody will be played 
and then the code will wait for the button press to play another set of short tones.
 
AUTHOR   : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/


const int BuzzerPin = 23;    // Robo ESP32 Buzzer connected to pin D23
const int UserButton1 = 34;  // User Button 1 connected to pin D34
const int UserButton2 = 35;  // User Button 2 connected to pin D35

// Notes for the "Happy Birthday" song
int melody_note[] = { 392, 392, 440, 392, 523, 494, 392, 392, 440, 392, 587, 523 };

// Short melody note durations for fast tempo (milliseconds)
int melody_duration[] = { 100, 100, 200, 200, 200, 270, 100, 100, 200, 200, 200, 270 };

void setup() {
  // Set the buzzer as an output device
  pinMode(BuzzerPin, OUTPUT);

  // Set the button as an input device with internal pull-up
  pinMode(UserButton1, INPUT_PULLUP);
  pinMode(UserButton2, INPUT_PULLUP);

  // Play the "Happy Birthday" melody during setup at fast tempo
  play_melody(BuzzerPin);
}

void loop() {
  // Check if the button is pressed (LOW means pressed because of INPUT_PULLUP)
  if (!digitalRead(UserButton1)) {
    // Play a short melody when the button is pressed
    tone(BuzzerPin, 262, 100);  // Play C note
    delay(100);
    tone(BuzzerPin, 659, 100);  // Play E note
    delay(100);
    tone(BuzzerPin, 784, 100);  // Play G note
    delay(100);
    noTone(BuzzerPin);  // Turn off the buzzer
  }
  if (!digitalRead(UserButton2)) {
    // Play a short melody when the button is pressed
    tone(BuzzerPin, 784, 100);  // Play C note
    delay(100);
    tone(BuzzerPin, 659, 100);  // Play E note
    delay(100);
    tone(BuzzerPin, 262, 100);  // Play G note
    delay(100);
    noTone(BuzzerPin);  // Turn off the buzzer
  }
}

// Function to play the "Happy Birthday" melody
void play_melody(int pin) {
  int notesCount = sizeof(melody_note) / sizeof(melody_note[0]);  // Calculate number of notes
  for (int i = 0; i < notesCount; i++) {
    if (melody_note[i] == 0) {
      noTone(pin);  // If the note is 0, turn off the buzzer
    } else {
      tone(pin, melody_note[i], melody_duration[i]);  // Play the note with the specified duration
    }
    delay(melody_duration[i] * 1.3);  // Slight delay between notes for rhythm
  }
}
