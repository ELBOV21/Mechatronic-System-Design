#include <Wire.h>
#include "tof.h"
#include "HiwonderMotorDriver.h"
#include <esp_now.h>
#include <WiFi.h>
//#include "Control/kinematics.h"
int8_t forward[4]={-30,-30,-30,-30};                  // (forward)
int8_t backward[4]={30,30,30,30};                     // (backward)

int8_t pure_left[4]={30,-30,-30,30};         // (move horizontally to the left)
int8_t pure_right[4]={-30,30,30,-30};            // (move horizontally to the right)

int8_t forward_left[4]={0,-30,-30,0};                // forward left
int8_t forward_right[4]={-30,0,0,-30};               // forward right

int8_t backward_left[4]={30,0,0,30};                // backward left
int8_t backward_right[4]={0,30,30,0};               // backward right


int8_t car_stop[4]={0,0,0,0}; 

typedef struct ESPNOW_Message {
  int command;
  int value1;
  int value2;
  int value3;
} ESPNOW_Message;

ESPNOW_Message incomingData;

#define I2C_SDA 21
#define I2C_SCL 22
TOFSensor tofSensor(I2C_SDA, I2C_SCL); // Use custom I2C pins

const float CUSTOM_MOTOR_PPR = 11.0;     // Pulses per revolution of the bare motor shaft
const float CUSTOM_GEAR_RATIO = 34.0;    // Gear Ratio
const float TICKS_PER_REV = CUSTOM_MOTOR_PPR * CUSTOM_GEAR_RATIO;

// Instantiate the driver using the default Wire object
HiwonderMotorDriver motors(Wire);

unsigned long lastTime = 0;
int32_t lastEnc1 = 0;
int targetSpeed = 0;


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
  if (len != sizeof(ESPNOW_Message)) return;
  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

  if (incomingData.command == 2) {
    int direction = incomingData.value1;

    // Switch statement to handle the 8 discrete directions
    switch (direction) {
      case 0:
        Serial.println("STOP");
        // Motor logic: All motors speed 04
         motors.setSpeed(car_stop);
        break;
      case 1:
        Serial.println("FORWARD");
        // Motor logic: All motors forward
        motors.setSpeed(forward);
        break;
      case 2:
        Serial.println("FORWARD-RIGHT (Diagonal)");
        // Motor logic: Front-Left & Back-Right forward, others stop
        
        break;
      case 3:
        Serial.println("RIGHT (Strafe)");
        // Motor logic: FL & BR forward, FR & BL backward
        motors.setSpeed(pure_right);
        break;
      case 4:
        Serial.println("BACKWARD-RIGHT (Diagonal)");
        // Motor logic: Front-Right & Back-Left backward, others stop
        break;
      case 5:
        Serial.println("BACKWARD");
        // Motor logic: All motors backward
         motors.setSpeed(backward);
        break;
      case 6:
        Serial.println("BACKWARD-LEFT (Diagonal)");
        // Motor logic: Front-Left & Back-Right backward, others stop
        break;
      case 7:
        Serial.println("LEFT (Strafe)");
        // Motor logic: FL & BR backward, FR & BL forward
        motors.setSpeed(pure_left);
        break;
      case 8:
        Serial.println("FORWARD-LEFT (Diagonal)");
        // Motor logic: Front-Right & Back-Left forward, others stop
        break;
    }
  }
}
void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Car ESP32 Ready - Waiting for Direction Commands.");
    Wire.begin(I2C_SDA, I2C_SCL);
    delay(500);
   // tofSensor.begin();
    
    Serial.println("System Initializing...");

    // Initialize with specific motor type
    if (!motors.begin(HiwonderMotorDriver::JGB37_520, 0)) {
        Serial.println("Warning: Motor driver initialization failed (I2C error).");
    }
}

void loop() {
// uint16_t d = tofSensor.readDistance();
//     if (d == 0xFFFF)
//         Serial.println("Error");
//     else
//         Serial.println(d);
    //delay(500);
    // Serial.printf("Distance: %d mm\n", tofSensor.readRangeContinuousMillimeters());
    // // 1. Check Battery
    // uint16_t voltage = motors.getBatteryVoltage();
    // Serial.printf("Battery Voltage: %d mV\n", voltage);

    // // 2. Move Forward using closed-loop speed
    // Serial.println("Moving Forward...");
    // motors.setSpeed(50, 50, 50, 50);
    // delay(3000);

    // // 3. Move Backward
    // Serial.println("Moving Backward...");
    // motors.setSpeed(-20, -20, -20, -20);
    // delay(3000);

    // // 4. Read Encoders
    // int32_t e1, e2, e3, e4;
    // if (motors.readEncoders(e1, e2, e3, e4)) {
    //     Serial.printf("Encoders -> M1: %ld | M2: %ld | M3: %ld | M4: %ld\n", e1, e2, e3, e4);
    // } else {
    //     Serial.println("Failed to read encoders.");
    // }

    // // 5. Stop and Reset
    // motors.stopAll();
    // delay(2000);
    // motors.resetEncoders();
    // Serial.println("Encoders reset.");

    // if (Serial.available() > 0) {
    //     // Read the incoming string until a newline character is detected
    //     String input = Serial.readStringUntil('\n');
    //     input.trim(); 
        
    //     if (input.length() > 0) {
    //         targetSpeed = input.toInt();
    //         Serial.printf(">>> Target Speed updated to: %d <<<\n", targetSpeed);
    //     }
    // }

  //motors.setSpeed(0, 0, targetSpeed, targetSpeed);
    //  motors.setSpeed(0, 0, 50, -50);
    //  delay(1000);
    // motors.setSpeed(0, 0, 0, 0);
    //  delay(1000);
    // motors.setSpeed(0, 0, -50, 50);
    //  delay(1000);
    // Command the motor with open-loop PWM
   //motors.setSpeed(50, 0, 0, 0); // Set all motors to 50% speed
   // motors.setPWM(100, 0, 0, 0); // Set all motors to 50% PWM (open-loop control)
    // unsigned long currentTime = millis();
    // // // Calculate RPM every 100ms
    // if (currentTime - lastTime >= 100) {
        
    //     int32_t e1, e2, e3, e4;
        
    //     if (motors.readEncoders(e1, e2, e3, e4)) {
    //        // Serial.printf("Encoders -> M1: %ld | M2: %ld | M3: %ld | M4: %ld\n", e1, e2, e3, e4);
    //         // Calculate delta ticks
    //         int32_t deltaTicks = e1 - lastEnc1;
    //         lastEnc1 = e1;
            
    //         // Calculate RPM: (Delta Ticks / Ticks Per Rev) * (60 seconds / Delta Time in seconds)
    //         float rpm = ((float)deltaTicks / TICKS_PER_REV) * (60.0 / 0.1)/4.0;
            
    //         Serial.printf("Motor 1 RPM: %.2f\n", rpm);
    //     }
    //     lastTime = currentTime;
    // }
}

