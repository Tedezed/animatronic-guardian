// Animatronic Guardian
// By Tedezed
// Code for motors 28BYJ-48
#include "Arduino.h"
#include "ControllSteper.h"

using namespace std;

Motors motors[2];
int pins_motor0 [4] = {0, 1, 2, 3};
int pins_motor1 [4] = {8, 9, 10, 11};

int mov_motor = 500; // Steps in a unit
bool debug = false;

// Conf 74HC595
#define SER_Pin 4 //SER_IN
#define RCLK_Pin 3 //L_CLOCK
#define SRCLK_Pin 2 //CLOCK
#define NUM_REGISTERS 2 //Number of circuits 74HC595 in chain

//Initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS);

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
  //
  motors[1].pins = pins_motor1;
  motors[1].home = 0;
  motors[1].position = 0;
  motors[1].step = 0;
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
      motors[0].steps_to = motors[0].home;
      motors[1].steps_to = motors[1].home;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 1700, debug);
    }
    else if(option == "new-home\n") {
      Serial.println("New home");
      motors[0].home = 0;
      motors[0].step = 0;
      motors[1].home = 0;
      motors[1].step = 0;
    }
    else if(option == "up-all\n") {
      Serial.println("Up all");
      motors[0].steps_to = motors[0].step - mov_motor;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
    }
    else if(option == "down-all\n") {
      Serial.println("Down all");
      motors[0].steps_to = motors[0].step + mov_motor;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
    }
    else if(option == "left-all\n") {
      Serial.println("Left all");
      Serial.println(motors[1].steps_to);
      motors[1].steps_to = motors[1].step - mov_motor;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
      Serial.println(motors[1].step);
    }
    else if(option == "right-all\n") {
      Serial.println("Right all");
      motors[1].steps_to = motors[1].step + mov_motor;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
    }
    else if(option == "rotation\n") {
      Serial.println("Rotation");
      motors[0].steps_to = motors[0].home;
      motors[1].steps_to = motors[1].home;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
      motors[0].steps_to = motors[0].step - 1000;
      motors[1].steps_to = motors[1].step - 2000;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
      motors[0].steps_to = motors[0].step - 1000;
      motors[1].steps_to = motors[1].step + 2000;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
      motors[0].steps_to = motors[0].home;
      motors[1].steps_to = motors[1].home;
      exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 2000, debug);
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
