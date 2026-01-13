#include <Arduino.h>
#include <Bluepad32.h>

#define FWD_LEFT   12
#define REV_LEFT   13
#define FWD_RIGHT  14
#define REV_RIGHT  27

#define CH_FWD_LEFT   0
#define CH_REV_LEFT   1
#define CH_FWD_RIGHT  2
#define CH_REV_RIGHT  3

const int freq = 5000;       
const int resolution = 8;    

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

int speedMode = 1;        // Default gear
int maxSpeeds[5] = {0, 100, 150, 200, 255};  

// For debounce (non-blocking)
unsigned long lastGearChange = 0;
const unsigned long gearDelay = 200; // ms

// === Function Prototypes ===
void controlMotors(int16_t y, int16_t x);
void controlMotor(int channelFwd, int channelRev, int speed);
void processControllers();
void processGamepad(ControllerPtr ctl);

// ===========================
// Setup
// ===========================
void setup() {
    Serial.begin(115200);
    Serial.println("Robo ESP32 PS4 control start");

    ledcSetup(CH_FWD_LEFT, freq, resolution);
    ledcSetup(CH_REV_LEFT, freq, resolution);
    ledcSetup(CH_FWD_RIGHT, freq, resolution);
    ledcSetup(CH_REV_RIGHT, freq, resolution);

    ledcAttachPin(FWD_LEFT, CH_FWD_LEFT);
    ledcAttachPin(REV_LEFT, CH_REV_LEFT);
    ledcAttachPin(FWD_RIGHT, CH_FWD_RIGHT);
    ledcAttachPin(REV_RIGHT, CH_REV_RIGHT);

    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();
}

void loop() {
    BP32.update();
    processControllers();

    delay(1); 
}

// ===========================
// Bluepad32 Events
// ===========================
void onConnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            myControllers[i] = ctl;
            Serial.printf("Controller connected at index %d\n", i);

            ctl->playDualRumble(0, 150, 0x40, 0x80);
            delay(500);
            ctl->playDualRumble(0, 150, 0x40, 0x80);
            delay(500);
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

            ctl->playDualRumble(0, 120, 0x50, 0x50);
            return;
        }
    }
    Serial.println("Controller disconnected, but not found.");
}


// ===========================
// Processing
// ===========================
void processControllers() {
    for (auto ctl : myControllers) {
        if (ctl && ctl->isConnected()) {
            processGamepad(ctl);
        }
    }
}

void processGamepad(ControllerPtr ctl) {
    unsigned long now = millis();

    // === Speed control with R2 / L2 ===
// === Speed control with R2 / L2 ===
if (ctl->throttle() > 200 && now - lastGearChange > gearDelay) {  // R2
    if (speedMode < 4) {
        speedMode++;
        Serial.printf("Increased speed mode: %d (maxSpeed=%d)\n", speedMode, maxSpeeds[speedMode]);
    } else {
        Serial.println("Already at maximum speed mode!");
   
        ctl->playDualRumble(0, 150, 0x30, 0x40);
    }
    lastGearChange = now;
}

if (ctl->brake() > 200 && now - lastGearChange > gearDelay) {     // L2
    if (speedMode > 1) {
        speedMode--;
        Serial.printf("Decreased speed mode: %d (maxSpeed=%d)\n", speedMode, maxSpeeds[speedMode]);
    } else {
        Serial.println("Already at minimum speed mode!");
     
        ctl->playDualRumble(0, 150, 0x30, 0x40);
    }
    lastGearChange = now;
}


    // === Button-based override control ===
    if (ctl->l1()) {
        int turnSpeed = maxSpeeds[speedMode];
        controlMotor(CH_FWD_LEFT, CH_REV_LEFT, -turnSpeed);
        controlMotor(CH_FWD_RIGHT, CH_REV_RIGHT, turnSpeed);
        Serial.printf("L1 pressed → move LEFT (speed=%d)\n", turnSpeed);
        return;
    }
    else if (ctl->r1()) {
        int turnSpeed = maxSpeeds[speedMode];
        controlMotor(CH_FWD_LEFT, CH_REV_LEFT, turnSpeed);
        controlMotor(CH_FWD_RIGHT, CH_REV_RIGHT, -turnSpeed);
        Serial.printf("R1 pressed → move RIGHT (speed=%d)\n", turnSpeed);
        return;
    }

    // === Joystick control ===
    int16_t axisY = ctl->axisY();
    int16_t axisX = ctl->axisX();
    controlMotors(axisY, axisX);

    // Debug print
    Serial.printf("Mode=%d, MaxSpeed=%d\n", speedMode, maxSpeeds[speedMode]);
}

// ===========================
// Motor Control
// ===========================
void controlMotors(int16_t y, int16_t x) {
    int deadzone = 30;
    int speedLeft = 0;
    int speedRight = 0;

    if (abs(y) > deadzone || abs(x) > deadzone) {
        int maxSpeed = maxSpeeds[speedMode];

        // Calculate motor speeds based on joystick
        speedLeft  = (int)((float)(y + x) / 512.0 * maxSpeed);
        speedRight = (int)((float)(y - x) / 512.0 * maxSpeed);

        // Clamp values
        speedLeft  = constrain(speedLeft,  -maxSpeed, maxSpeed);
        speedRight = constrain(speedRight, -maxSpeed, maxSpeed);
    }

    controlMotor(CH_FWD_LEFT, CH_REV_LEFT, speedLeft);
    controlMotor(CH_FWD_RIGHT, CH_REV_RIGHT, speedRight);
}

void controlMotor(int channelFwd, int channelRev, int speed) {
    if (speed > 0) {
        ledcWrite(channelFwd, speed);
        ledcWrite(channelRev, 0);
    } else {
        ledcWrite(channelFwd, 0);
        ledcWrite(channelRev, -speed);
    }
}
