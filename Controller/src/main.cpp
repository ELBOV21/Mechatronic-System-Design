#include <esp_now.h>
#include <WiFi.h>
#include "Joystick.h"
// MAC Addresses
uint8_t armAddress[] = {0xC8, 0xF0, 0x9E, 0xB9, 0x94, 0x0C};
uint8_t carAddress[] = {0x80, 0xF3, 0xDA, 0x54, 0xAC, 0x58};

typedef struct ESPNOW_Message {
  int command;
  int value1;
  int value2;
  int value3;
} ESPNOW_Message;

ESPNOW_Message myData;
esp_now_peer_info_t peerInfo;

const int potPin = 34; 

// A threshold to decide when the joystick is pushed far enough
// 40 out of 100 means you have to push it 40% of the way to trigger a direction
const int threshold = 40; 

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  
  // Register Arm
  memcpy(peerInfo.peer_addr, armAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  // Register Car
  memcpy(peerInfo.peer_addr, carAddress, 6);
  esp_now_add_peer(&peerInfo);
}

void loop() {
  // --- 1. HANDLE ARM (Potentiometer) ---
  int potValue = analogRead(potPin);
  myData.command = 1;
  myData.value1 = map(potValue, 0, 4095, 0, 180); 
  myData.value2 = 0;
  myData.value3 = 0;
  esp_now_send(armAddress, (uint8_t *) &myData, sizeof(myData));
  
  delay(10); 

  // --- 2. HANDLE CAR (Joystick 8-Way Logic) ---
 int direction = getJoystickDirection(threshold);
  // Forward-Left
Serial.print("Joystick Direction: ");
switch(direction) {
  case 0: Serial.println("Stop"); break;
  case 1: Serial.println("Forward"); break;
  case 2: Serial.println("Forward-Right"); break;
  case 3: Serial.println("Right"); break;
  case 4: Serial.println("Backward-Right"); break;
  case 5: Serial.println("Backward"); break;
  case 6: Serial.println("Backward-Left"); break;
  case 7: Serial.println("Left"); break;
  case 8: Serial.println("Forward-Left"); break;
}
myData.command = 2; // Car command
myData.value1 = direction; // Send ONLY the integer 0-8
myData.value2 = 0; // Unused
myData.value3 = 0;
  
  esp_now_send(carAddress, (uint8_t *) &myData, sizeof(myData));

  delay(40); 
}