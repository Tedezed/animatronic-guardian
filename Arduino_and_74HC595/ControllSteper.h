#include "Arduino.h"

#ifndef CONTROL_STEPER_H
#define CONTROL_STEPER_H

#include <Shifter.h>

int* array_motors;
int* current_pins;

struct Motors {
  int* pins;
  int home;
  int position;
  int step;
  int steps_to;
  // ID for leg, two motors in one leg
  int arm;
  // Mode leg: 0 up-down, 1 left-right
  int arm_mode;
};

extern Motors motors[12];
extern Shifter shifter;

void conf_pin_mode(int size_pins,int* input)
{
  for (int i = 0; i < size_pins; i++)
  {
    pinMode(input[i], OUTPUT);
    Serial.print("Config pin:");
    Serial.println(input[i]);
  }
}

int to_positive(int input)
{
    if (input < 0)
    {
      input = input * -1;
    }
    return input;
}

int to_negative(int input)
{
    if (input > 0)
    {
      input = input * -1;
    }
    return input;
}

int distance(int init_step, int end_step)
{
    if (init_step < end_step)
    {
      return to_positive(init_step - end_step);
    }
    return to_negative(end_step - init_step);
}

int max_motor_distance(Motors* motors, int size_motors, bool debug=false)
{
  int max_distance = 0;
  for (int m = 0; m < size_motors; m++)
  {
    int distance_motor = to_positive(distance(motors[m].step, motors[m].steps_to));
    if (debug)
    {
      Serial.print("[INTERNAL] Motor: ");
      Serial.print(m);
      Serial.print(" Actual step: ");
      Serial.print(motors[m].step);
      Serial.print(" Step to: ");
      Serial.print(motors[m].steps_to);
      Serial.print(" Distance: ");
      Serial.println(distance_motor);
    }
    if (distance_motor > max_distance)
    {
      max_distance = distance_motor;
    }
  }
  return max_distance;
}

Motors* exec_steps_multiple_motors(
  int array_steps[][4],
  int size_array_pins,
  int size_motors,
  Motors* motors,
  int delay_step=2000,
  bool debug=false
  )
{
  for (int m = 0; m < size_motors; m++)
  {
     Serial.print("[INTERNAL] Motor ");
     Serial.print(m);
     Serial.print(" init step: ");
     Serial.println(motors[m].step);
  }
  // State 0 = stop
  // State 1 = positive rotation
  // State -1 = negative rotation
  int state_motor = 0;
  int max_step = max_motor_distance(motors, size_motors, debug);
  Serial.print("[INTERNAL] Exec steps: ");
  Serial.println(max_step);
  for (int s = 0; s < max_step; s++)
  {
    for (int m = 0; m < size_motors; m++)
    {
      // Position control
      int next_position = motors[m].position + 1;
      if (next_position >= size_array_pins)
      {
        next_position = 0;
        motors[m].position = next_position;
      }
      else
      {
        motors[m].position = next_position;
      }
      
      for (int i = 0; i < size_array_pins; i++)
      {
        // Reverse control
        int distance_motor = distance(motors[m].step, motors[m].steps_to);
        if (debug)
        {
          Serial.print("[INTERNAL] Pin: ");
          Serial.print(motors[m].pins[i]);
          Serial.print(" Position: ");
          Serial.print(array_steps[next_position][i]);
          Serial.print(" Next_position: ");
          Serial.print(next_position);
          Serial.print(" Distance: ");
          Serial.println(distance_motor);
        }
        if (distance_motor < 0)
        {
          shifter.setPin(motors[m].pins[i], array_steps[next_position][(i-(size_array_pins-1))*-1]);
          state_motor = -1;
        }
        else if (distance_motor == 0)
        {
          state_motor = 0;
        }
        else
        {
          shifter.setPin(motors[m].pins[i], array_steps[next_position][i]);
          state_motor = 1;
        }
      }
      shifter.write();
      //Serial.println("-----");
      motors[m].step = motors[m].step + state_motor;
      //Serial.println(state_motor);
      //Serial.println(motors[m].step);
    }
    delayMicroseconds(delay_step);
  }
  for (int m = 0; m < size_motors; m++)
  {
     Serial.print("[INTERNAL] Motor ");
     Serial.print(m);
     Serial.print(" end step: ");
     Serial.println(motors[m].step);
  }
  return motors;
}

#endif
