// Animatronic Guardian
// By Tedezed
// Code for motors 28BYJ-48
#include "Arduino.h"
#include "ControllSteper.h"

using namespace std;

Motors motors[2];
Motors motors_exec[1];
int pins_motor0 [4] = {9, 10, 11, 12};
int pins_motor1 [4] = {5, 6, 7, 8};

int array_steps [4][4] =
{
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1}
};
int size_array_pins = 4;

String option = "";
void setup()
{
  Serial.begin(9600);
  motors[0].pins = pins_motor0;
  motors[0].home = 0;
  motors[0].position = 0;
  motors[0].step = 0;
  conf_pin_mode(4, pins_motor0);
  
  motors[1].pins = pins_motor1;
  motors[1].home = 0;
  motors[1].position = 0;
  motors[1].step = 0;
  conf_pin_mode(4, pins_motor1);

}

// Start Guardian
void loop(){
  //motors[0].steps_to = motors[0].steps_to -1000;
  //motors[1].steps_to = motors[1].steps_to +1100;
  //exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 1700);
  //delay(3500);
  
  if (Serial.available()>0){
    option=Serial.readString();
    if(option == "home-all\n") {
      Serial.println("Go to home");
      Serial.println(motors[0].step);
      Serial.println(motors[1].step);
      motors[0].steps_to = motors[0].home;
      motors[1].steps_to = motors[1].home;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 1700, false);
      motors[0].step = motors[0].home;
      motors[1].step = motors[1].home;
    }
    else if(option == "up-all\n") {
      Serial.println("Up all");
      motors[0].steps_to = motors[0].step -1000;
      motors_exec[0] = motors[0];
      exec_steps_multiple_motors(array_steps, size_array_pins, 1, motors_exec, 2000);
      motors[0].step = motors[0].step -1000;
    }
    else if(option == "down-all\n") {
      Serial.println("Down all");
      motors[0].steps_to = motors[0].step +1000;
      motors_exec[0] = motors[0];
      exec_steps_multiple_motors(array_steps, size_array_pins, 1, motors_exec, 2000);
      motors[0].step = motors[0].step +1000;
    }
    else if(option == "left-all\n") {
      Serial.println("Left all");
      motors[1].steps_to = motors[1].step +1000;
      motors_exec[0] = motors[1];
      exec_steps_multiple_motors(array_steps, size_array_pins, 1, motors_exec, 2000);
      motors[1].step = motors[1].step +1000;
    }
    else if(option == "right-all\n") {
      Serial.println("Right all");
      motors[1].steps_to = motors[1].step -1000;
      motors_exec[0] = motors[1];
      exec_steps_multiple_motors(array_steps, size_array_pins, 1, motors_exec, 2000);
      motors[1].step = motors[1].step -1000;
    }
    else
    {
      Serial.print("Option not found: ");
      Serial.println(option);
    }
    Serial.flush(); 
  }
  delay(1);
}
