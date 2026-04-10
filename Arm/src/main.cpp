#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "Servo_Driver.h" 

// Must exactly match the structure on the Controller
typedef struct ESPNOW_Message {
  int command;
  int value1;
  int value2;
  int value3;
} ESPNOW_Message;

ESPNOW_Message incomingData;

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
  if (len != sizeof(ESPNOW_Message)) {
    Serial.println("Invalid data size received.");
    return;
  }

  // Copy raw data into our struct
  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

  Serial.print("Command: ");
  Serial.println(incomingData.command);
  Serial.print("Angle Value: ");
  Serial.println(incomingData.value1);

  // Process command if it's meant for the arm
  if (incomingData.command == 1) {
    // Move servo 0 to the angle specified in value1
    Servo_SetAngle(0, incomingData.value1); 
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize your PCA9685 driver
   Servo_Init(); 

  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);
  
  Serial.println("Arm ESP32 Ready to Receive.");
}

void loop() {
  // ESP-NOW handles reception in the background via the callback
}