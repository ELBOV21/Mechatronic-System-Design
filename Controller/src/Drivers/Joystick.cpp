#include "Joystick.h"
const int joyXPin = 32; 
const int joyYPin = 33; 
int getJoystickDirection( int threshold) {
  // Map to -100 to 100 percentage. 
  int rawX = analogRead(joyXPin);
  int rawY = analogRead(joyYPin);

  int x = map(rawX, 0, 4095, -100, 100); 
  int y = map(rawY, 0, 4095, 100, -100);

  // Determine which zones the stick is pushed into
  bool up = (y > threshold);
  bool down = (y < -threshold);
  bool right = (x > threshold);
  bool left = (x < -threshold);

  int direction = 0; // Default to Stop

  if (up && !right && !left) direction = 1;         // Forward
  else if (up && right) direction = 2;              // Forward-Right
  else if (!up && !down && right) direction = 3;    // Right
  else if (down && right) direction = 4;            // Backward-Right
  else if (down && !right && !left) direction = 5;  // Backward
  else if (down && left) direction = 6;             // Backward-Left
  else if (!up && !down && left) direction = 7;     // Left
  else if (up && left) direction = 8;               // Forward-Left
  return direction;
}