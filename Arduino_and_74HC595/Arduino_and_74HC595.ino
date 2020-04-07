// Animatronic Guardian
// By Tedezed
// Code for motors 28BYJ-48
#include "Arduino.h"
#include "ControllSteper.h"

using namespace std;

Motors motors[6];
int size_motors = 6;

int pins_motor0 [4] = {0, 1, 2, 3};
int pins_motor1 [4] = {4, 5, 6, 7};
int pins_motor2 [4] = {8, 9, 10, 11};
int pins_motor3 [4] = {12, 13, 14, 15};
int pins_motor4 [4] = {16, 17, 18, 19};
int pins_motor5 [4] = {20, 21, 22, 23};

int mov_motor = 500; // Steps in a unit
int delay_motor = 2200;
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
  for (int m = 0; m < size_motors; m++)
  {
    motors[m].home = 0;
    motors[m].position = 0;
    motors[m].step = 0;
  }
  motors[0].pins = pins_motor0;
  motors[1].pins = pins_motor1;
  motors[2].pins = pins_motor2;
  motors[3].pins = pins_motor3;
  motors[4].pins = pins_motor4;
  motors[5].pins = pins_motor5;
}

int only_one = 300;
// Start Guardian
void loop(){
  //motors[0].steps_to = motors[0].steps_to -1000;
  //motors[1].steps_to = motors[1].steps_to +1100;
  //exec_steps_multiple_motors(array_steps, size_array_pins, 2, motors, 1700);
  //delay(3500);
  
  if (Serial.available()>0){
    option=Serial.readString();
    if(option == "home\n") {
      Serial.println("Go to home");
      if (only_one < 200) {
        motors[only_one].steps_to = motors[only_one].home;
        exec_steps_multiple_motors(array_steps, size_array_pins, 1, motors, delay_motor, debug);
      }
      else 
      {
        for (int m = 0; m < size_motors; m++)
        {
           motors[m].steps_to = motors[m].home;
        }
        exec_steps_multiple_motors(array_steps, size_array_pins, size_motors, motors, delay_motor, debug);
      }
    }
    else if(option == "new-home\n") {
      Serial.println("New home");
      for (int m = 0; m < size_motors; m++)
      {
         motors[m].home = 0;
         motors[m].step = 0;
      }
    }
    else if(option == "up\n") {
      Serial.println("Up");
      if (only_one < 200) {
        motors[only_one].steps_to = motors[only_one].step - mov_motor;
        exec_steps_multiple_motors(array_steps, size_array_pins, 1, motors, delay_motor, debug);
      }
      else
      {
        for (int m = 0; m < size_motors; m++)
        {
           motors[m].steps_to = motors[m].step - mov_motor;
        }
        exec_steps_multiple_motors(array_steps, size_array_pins, size_motors, motors, delay_motor, debug);
      }
    }
    else if(option == "down\n") {
      Serial.println("Down");
      if (only_one < 200) {
        motors[only_one].steps_to = motors[only_one].step + mov_motor;
        exec_steps_multiple_motors(array_steps, size_array_pins, size_motors, motors, delay_motor, debug);
      }
      else
      {
        for (int m = 0; m < size_motors; m++)
        {
           motors[m].steps_to = motors[m].step + mov_motor;
        }
        exec_steps_multiple_motors(array_steps, size_array_pins, size_motors, motors, delay_motor, debug);
      }
    }
    else if(option == "only-one\n") {
      Serial.println("Only-one");
      bool not_break = true;
      while (not_break) {
        if (Serial.available()>0){
          only_one=Serial.parseInt();
          Serial.print("Select motor: ");
          Serial.println(only_one);
          not_break = false;
        }
      }
    }
    else if(option == "all\n") {
      Serial.println("Select all motors");
      only_one = 300;
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
