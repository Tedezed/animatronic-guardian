// Animatronic Guardian
// By: Juan Manuel Torres (Tedezed)
// Code for motors 28BYJ-48
#include "Arduino.h"
#include "ControllSteper.h"
#include "Servo.h"
using namespace std;

// Conf 74HC595
#define SER_Pin 4 //SER_IN
#define RCLK_Pin 3 //L_CLOCK
#define SRCLK_Pin 2 //CLOCK
#define NUM_REGISTERS 6 //Number of circuits 74HC595 in chain
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS);

// User vars
bool debug = false;
bool off_in_the_end = true;

Servo servoMotor;
int servo_pin = 10;

Motors motors[12];
int size_motors = 12;

int array_steps [4][4] =
{
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1}
};
int size_array_pins = 4;

int mov_motor = 500; // Steps in a unit
int delay_motor = 2300;

// Max 200 motors
int pins_motor0 [4] = {3, 2, 1, 0};
int pins_motor1 [4] = {4, 5, 6, 7};
int pins_motor2 [4] = {8, 9, 10, 11};
int pins_motor3 [4] = {12, 13, 14, 15};
int pins_motor4 [4] = {16, 17, 18, 19};
int pins_motor5 [4] = {20, 21, 22, 23};
int pins_motor6 [4] = {27, 26, 25, 24};
int pins_motor7 [4] = {28, 29, 30, 31};
int pins_motor8 [4] = {32, 33, 34, 35};
int pins_motor9 [4] = {36, 37, 38, 39};
int pins_motor10 [4] = {40, 41, 42, 43};
int pins_motor11 [4] = {44, 45, 46, 47};

String option = "";
void setup()
{
  Serial.begin(9600);
  // Servo head
  //servoMotor.attach(servo_pin);
  
  // Init motors
  for (int m = 0; m < size_motors; m++)
  {
    motors[m].home = 0;
    motors[m].position = 0;
    motors[m].step = 0;
  }
  motors[0].pins = pins_motor0;
  motors[1].pins = pins_motor1;
  motors[0].arm = 0;
  motors[0].arm_mode = 0;
  motors[1].arm = 0;
  motors[1].arm_mode = 1;
  
  motors[2].pins = pins_motor2;
  motors[3].pins = pins_motor3;
  motors[2].arm = 1;
  motors[2].arm_mode = 0;
  motors[3].arm = 1;
  motors[3].arm_mode = 1;

  motors[4].pins = pins_motor4;
  motors[5].pins = pins_motor5;
  motors[4].arm = 2;
  motors[4].arm_mode = 0;
  motors[5].arm = 2;
  motors[5].arm_mode = 1;

  motors[6].pins = pins_motor6;
  motors[7].pins = pins_motor7;
  motors[6].arm = 3;
  motors[6].arm_mode = 0;
  motors[7].arm = 3;
  motors[7].arm_mode = 1;

  motors[8].pins = pins_motor8;
  motors[9].pins = pins_motor9;
  motors[8].arm = 4;
  motors[8].arm_mode = 0;
  motors[9].arm = 4;
  motors[9].arm_mode = 1;

  motors[10].pins = pins_motor10;
  motors[11].pins = pins_motor11;
  motors[10].arm = 5;
  motors[10].arm_mode = 0;
  motors[11].arm = 5;
  motors[11].arm_mode = 1;
}

void motor_multiplexor(String option, int only_one, int mode, int mov_motor){
  // Modes: 0 home, 1 motor, 2 vertical, 3 horizontal
  Serial.print("[EXEC] ");
  Serial.println(option);
  if (only_one < 200) {
    Serial.println("[motor_multiplexor] One Motor");
    if ( mode == 0 ) {
      motors[only_one].steps_to = motors[only_one].home;
    }
    else {
      Serial.print("Motor: ");
      Serial.println(only_one);
      motors[only_one].steps_to = motors[only_one].step + mov_motor;
    }
    //exec_steps_multiple_motors(array_steps, size_array_pins, size_motors, motors, delay_motor, off_in_the_end, debug);
    exec_steps_multiple_motors(array_steps, size_array_pins, size_motors, motors, delay_motor, off_in_the_end, debug);
  }
  else 
  {
    Serial.println("[motor_multiplexor] All Motors");
    for (int m = 0; m < size_motors; m++)
      {
      if ( mode == 0 ) {
        motors[m].steps_to = motors[m].home;
      }
      else {
        if ( mode == 2 ) {
          if ( motors[m].arm_mode == 0 ) {
            motors[m].steps_to = motors[m].step + mov_motor; 
          }
        }
        else {
          if ( motors[m].arm_mode == 1 ) {
            motors[m].steps_to = motors[m].step + mov_motor; 
          }
        }
      }
    }
    exec_steps_multiple_motors(array_steps, size_array_pins, size_motors, motors, delay_motor, off_in_the_end, debug);
  }
}

int only_one = 300;
int only_one_vertical_motor = 0;
int only_one_horizontal_motor = 1;
// Start Guardian
void loop(){
  //servoMotor.detach(servo_pin);
  //servoMotor.write(70);
  //Serial.println(servoMotor.read());
  if (Serial.available()>0){
    option=Serial.readString();
    Serial.print("> ");
    Serial.print(option);
    if(option == "home\n") {
      motor_multiplexor(option, only_one, 0, 0);
    }
    else if(option == "new-home\n") {
      Serial.println("[EXEC] New home");
      for (int m = 0; m < size_motors; m++)
      {
         motors[m].home = 0;
         motors[m].step = 0;
      }
    }
    else if(option == "up\n") {
      motor_multiplexor(option, only_one_vertical_motor, 2, -mov_motor);
    }
    else if(option == "down\n") {
      motor_multiplexor(option, only_one_vertical_motor, 2, +mov_motor);
    }
    else if(option == "left\n") {
      motor_multiplexor(option, only_one_horizontal_motor, 3, -mov_motor);
    }
    else if(option == "right\n") {
      motor_multiplexor(option, only_one_horizontal_motor, 3, +mov_motor);
    }
    else if(option == "head-left\n") {
      servoMotor.attach(servo_pin);
      Serial.println("[EXEC] Head Left");
      servoMotor.write(60);
      delay(1000);
      servoMotor.detach();
    }
    else if(option == "head-right\n") {
      Serial.println("[EXEC] Head Right");
      servoMotor.attach(servo_pin);
      servoMotor.write(90);
      delay(1000);
      servoMotor.detach();
    }
    else if(option == "one\n") {
      Serial.println("[INPUT] Enter arm number... ");
      bool not_break = true;
      while (not_break) {
        if (Serial.available()>0){
          only_one=Serial.parseInt();
          only_one_vertical_motor=only_one*2;
          only_one_horizontal_motor=only_one*2+1;
          Serial.print("Select arm: ");
          Serial.println(only_one);
          Serial.print("Vertical motor: ");
          Serial.println(only_one_vertical_motor);
          Serial.print("Horizontal motor: ");
          Serial.println(only_one_horizontal_motor);
          not_break = false;
        }
      }
    }
    else if(option == "all\n") {
      Serial.println("[INFO] Select all motors...");
      only_one = 300;
      only_one_vertical_motor = 300;
      only_one_horizontal_motor = 300;
    }
    else if(option == "hi\n") {
      Serial.println("Hi!");
    }
    else
    {
      if (option != "\n") {
        Serial.print("[ERROR] Option not found: ");
        Serial.println(option);
      }
    }
    Serial.flush(); 
  }
  delay(1);
}
