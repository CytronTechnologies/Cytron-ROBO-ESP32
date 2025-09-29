#include <Arduino.h>
#include <Bluepad32.h>

// Motor control pins
#define FWD_LEFT 12  // D27 Forward Left Motor
#define REV_LEFT 13   // D14 Reverse Left Motor
#define FWD_RIGHT 14  // D13 Forward Right Motor
#define REV_RIGHT 27  // D12 Reverse Right Motor

// PWM configuration
#define PWM_FREQUENCY 1000
#define PWM_RESOLUTION 8
#define FWD_LEFT_CHANNEL 0
#define REV_LEFT_CHANNEL 1
#define FWD_RIGHT_CHANNEL 2
#define REV_RIGHT_CHANNEL 3

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void setup() {
    Serial.begin(115200);
    Serial.println("Robo ESP32 PS4 control start");

    // Set motor pins as output
    pinMode(FWD_LEFT, OUTPUT);
    pinMode(REV_LEFT, OUTPUT);
    pinMode(FWD_RIGHT, OUTPUT);
    pinMode(REV_RIGHT, OUTPUT);

    // Setup PWM channels
    ledcSetup(FWD_LEFT_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(REV_LEFT_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(FWD_RIGHT_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(REV_RIGHT_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);

    // Attach PWM channels to motor pins
    ledcAttachPin(FWD_LEFT, FWD_LEFT_CHANNEL);
    ledcAttachPin(REV_LEFT, REV_LEFT_CHANNEL);
    ledcAttachPin(FWD_RIGHT, FWD_RIGHT_CHANNEL);
    ledcAttachPin(REV_RIGHT, REV_RIGHT_CHANNEL);

    // Initialize Bluepad32
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();  // Optional: clear previously paired devices
}

void loop() {
    BP32.update();  
    processControllers();
    vTaskDelay(1);  
}

// Called when a new controller connects
void onConnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            myControllers[i] = ctl;
            Serial.printf("Controller connected at index %d\n", i);
            return;
        }
    }
    Serial.println("Controller connected, but no available slot.");
}

void onDisconnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            myControllers[i] = nullptr;
            Serial.printf("Controller disconnected from index %d\n", i);
            return;
        }
    }
    Serial.println("Controller disconnected, but not found.");
}

// Check and process connected controllers
void processControllers() {
    for (auto ctl : myControllers) {
        if (ctl && ctl->isConnected()) {
            processGamepad(ctl);
        }
    }
}

// Read joystick values and control motors
void processGamepad(ControllerPtr ctl) {
    int16_t axisY = ctl->axisY();  // Forward/Reverse
    int16_t axisX = ctl->axisX();  // Left/Right

    controlMotors(axisY, axisX);
}

// Translate joystick values to motor PWM signals
void controlMotors(int16_t y, int16_t x) {
    int threshold = 50;  // Dead zone
    int speedLeft = 0;
    int speedRight = 0;

    if (abs(y) > threshold || abs(x) > threshold) {
        speedLeft = constrain(y + x, -255, 255);
        speedRight = constrain(y - x, -255, 255);
    }

    // Left motor
    if (speedLeft > 0) {
        ledcWrite(FWD_LEFT_CHANNEL, speedLeft);
        ledcWrite(REV_LEFT_CHANNEL, 0);
    } else {
        ledcWrite(FWD_LEFT_CHANNEL, 0);
        ledcWrite(REV_LEFT_CHANNEL, -speedLeft);
    }

    // Right motor
    if (speedRight > 0) {
        ledcWrite(FWD_RIGHT_CHANNEL, speedRight);
        ledcWrite(REV_RIGHT_CHANNEL, 0);
    } else {
        ledcWrite(FWD_RIGHT_CHANNEL, 0);
        ledcWrite(REV_RIGHT_CHANNEL, -speedRight);
    }
}