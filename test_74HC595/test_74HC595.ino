#include <Shifter.h>

#define SER_Pin 4 //SER_IN
#define RCLK_Pin 3 //L_CLOCK
#define SRCLK_Pin 2 //CLOCK

#define NUM_REGISTERS 2 //how many registers are in the chain


//initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS);

void setup(){

}

void loop(){
 for (int n = 0; n < 16 ; n++)
 {
 shifter.clear();
 shifter.write();
 //
 shifter.setPin(n, HIGH); //set pin 1 in the chain(second pin) HIGH
 shifter.write(); //send changes to the chain and display them
 delay(100);
 }
 
 }
