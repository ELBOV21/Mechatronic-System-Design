#include "Servo_Driver.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
void Servo_Init() {
    pwm.begin();
     pwm.setOscillatorFrequency(25000000); 
    pwm.setPWMFreq(50);  
}

void Servo_SetAngle(uint8_t servoNum, float angle) {
    // Map angle (0-180) to pulse width (500-2500 microseconds)
    uint16_t pulseWidth = map(angle, 0, 180, 500, 2500);
    pwm.writeMicroseconds(servoNum, pulseWidth);
}