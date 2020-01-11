// Animatronic Guardian
// By Tedezed
// Code for motors 28BYJ-48
#include "Arduino.h"
#include "ControllSteper.h"

using namespace std;

Motors motors[2];
int pins_motor1 [4] = {9, 10, 11, 12};
int pins_motor2 [4] = {5, 6, 7, 8};

int array_steps [4][4] =
{
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1}
};
int size_array_pins = 4;


void setup()
{
  Serial.begin(9600);
  motors[0].pins = pins_motor1;
  motors[0].home = 0;
  motors[0].position = 0;
  motors[0].step = 0;
  conf_pin_mode(4, pins_motor1);
  
  motors[1].pins = pins_motor2;
  motors[1].home = 0;
  motors[1].position = 0;
  motors[1].step = 0;
  conf_pin_mode(4, pins_motor2);

}

// Start Guardian
void loop(){
  motors[0].steps_to = motors[0].steps_to -1000;
  motors[1].steps_to = motors[1].steps_to +1100;
  exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 1700);
  delay(3500);
}
