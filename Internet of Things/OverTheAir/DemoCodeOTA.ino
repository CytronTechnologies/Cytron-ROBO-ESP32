#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#include "CytronMotorDriver.h"

// WiFi Credentials
const char* host = "esp32";
const char* ssid = "CytronVeryFastWifi";
const char* password = "CytronVerySecurePassword";


WebServer server(80);

const char* loginIndex = R"rawliteral(
<form name='loginForm'>
  <table width='20%' bgcolor='A09F9F' align='center'>
    <tr><td colspan=2><center><font size=4><b>ESP32 Login Page</b></font></center><br></td></tr>
    <tr><td>Username:</td><td><input type='text' size=25 name='userid'><br></td></tr>
    <tr><td>Password:</td><td><input type='password' size=25 name='pwd'><br></td></tr>
    <tr><td><input type='submit' onclick='check(this.form)' value='Login'></td></tr>
  </table>
</form>
<script>
function check(form){
  if(form.userid.value=='admin' && form.pwd.value=='admin'){
    window.open('/serverIndex')
  } else {
    alert('Error Password or Username')
  }
}
</script>
)rawliteral";

const char* serverIndex = R"rawliteral(
<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>
<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
  <input type='file' name='update'>
  <input type='submit' value='Update'>
</form>
<div id='prg'>progress: 0%</div>
<script>
  $('form').submit(function(e){
    e.preventDefault();
    var form = $('#upload_form')[0];
    var data = new FormData(form);
    $.ajax({
      url: '/update',
      type: 'POST',
      data: data,
      contentType: false,
      processData: false,
      xhr: function(){
        var xhr = new window.XMLHttpRequest();
        xhr.upload.addEventListener('progress', function(evt){
          if(evt.lengthComputable){
            var per = evt.loaded / evt.total;
            $('#prg').html('progress: ' + Math.round(per*100) + '%');
          }
        }, false);
        return xhr;
      },
      success:function(d,s){ console.log('success!') },
      error: function(a,b,c){}
    });
  });
</script>
)rawliteral";

// Hardware setup
#define led1 16
#define led2 17
#define led3 21
#define led4 22
#define led5 25
#define led6 26
#define led7 32
#define led8 33
#define Buzzer 23
#define UserButton1 34
#define UserButton2 35
#define M1A 12
#define M1B 13
#define M2A 14
#define M2B 27
CytronMD motor1(PWM_PWM, M1A, M1B);
CytronMD motor2(PWM_PWM, M2A, M2B);

const int servoPin1 = 4;
const int servoPin2 = 5;
const int servoPin3 = 18;
const int servoPin4 = 19;
Servo servo1, servo2, servo3, servo4;

const int Neopixel = 15;
const int numPixels = 2;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, Neopixel, NEO_GRB + NEO_KHZ800);

int melody[] = { 392, 392, 440, 392, 523, 494, 392, 392, 440, 392, 587, 523 };
int duration[] = { 100, 100, 200, 200, 200, 270, 100, 100, 200, 200, 200, 270 };

bool neoPixelsOff = false;

void playHappyBirthday() {
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    tone(Buzzer, melody[i], duration[i]);
    delay(duration[i] * 1.3);
  }
  noTone(Buzzer);
}

void playSound1() {
  tone(Buzzer, 262, 100);
  delay(100);
  tone(Buzzer, 659, 100);
  delay(100);
  tone(Buzzer, 784, 100);
  delay(100);
  noTone(Buzzer);
}

void playSound2() {
  tone(Buzzer, 784, 100);
  delay(100);
  tone(Buzzer, 659, 100);
  delay(100);
  tone(Buzzer, 262, 100);
  delay(100);
  noTone(Buzzer);
}

void lightBlueLEDs(bool state) {
  int leds[] = { led1, led2, led3, led4, led5, led6, led7, led8 };
  for (int i = 0; i < 8; i++) digitalWrite(leds[i], state ? HIGH : LOW);
}

void turnOffNeoPixels() {
  strip.fill(0x000000);
  strip.show();
}

void animateNeoPixels() {
  static int state = 0;
  static uint8_t red = 255, green = 0, blue = 0;
  static unsigned long lastUpdate = 0;
  const unsigned long interval = 10;
  const float brightnessFactor = 0.2;

  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    switch (state) {
      case 0:
        if (green < 255) green++;
        else state++;
        break;
      case 1:
        if (red > 0) red--;
        else state++;
        break;
      case 2:
        if (blue < 255) blue++;
        else state++;
        break;
      case 3:
        if (green > 0) green--;
        else state++;
        break;
      case 4:
        if (red < 255) red++;
        else state++;
        break;
      case 5:
        if (blue > 0) blue--;
        else state = 0;
        break;
    }
    uint8_t scaledRed = red * brightnessFactor;
    uint8_t scaledGreen = green * brightnessFactor;
    uint8_t scaledBlue = blue * brightnessFactor;
    uint32_t color = (scaledRed << 16) | (scaledGreen << 8) | scaledBlue;
    strip.fill(color);
    strip.show();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(Buzzer, OUTPUT);
  pinMode(UserButton1, INPUT_PULLUP);
  pinMode(UserButton2, INPUT_PULLUP);

  int leds[] = { led1, led2, led3, led4, led5, led6, led7, led8 };
  for (int i = 0; i < 8; i++) pinMode(leds[i], OUTPUT);

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
  playHappyBirthday();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(host))
    while (1) delay(1000);

  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  server.on(
    "/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", Update.hasError() ? "FAIL" : "OK");
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) Update.printError(Serial);
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
          Update.printError(Serial);
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("Update Success: %u bytes\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
  server.begin();
}

void loop() {
  server.handleClient();
  if (!neoPixelsOff) animateNeoPixels();

  if (digitalRead(UserButton1) == LOW) {
    playSound1();
    lightBlueLEDs(true);
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);
    for (int i = 0; i < 3; i++) {
      motor1.setSpeed(100);
      motor2.setSpeed(-100);
      delay(200);
      motor1.setSpeed(-100);
      motor2.setSpeed(100);
      delay(200);
    }
    neoPixelsOff = false;
  }

  if (digitalRead(UserButton2) == LOW) {
    playSound2();
    lightBlueLEDs(false);
    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    servo4.write(180);
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    turnOffNeoPixels();
    neoPixelsOff = true;
    delay(1000);
  }
}