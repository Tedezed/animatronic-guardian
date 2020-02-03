# 74HC595 IC 8 Bits Guardian

### Sources
- http://www.haberocean.com/2019/05/shift-register-module-working-using-arduino-uno/
- http://www.haberocean.com/2019/05/cascading-two-shift-register-modules-using-arduino-uno/
- http://www.haberocean.com/2019/05/7-digit-seven-segment-display-module-from-arduino-uno-through-shift-register-module/
- https://www.bidorbuy.co.za/item/364825764/74HC595_8_Bit_Breakout_Shift_Register_Module_for_Arduino.html
- https://bildr.org/2011/08/74hc595-breakout-arduino/
- https://github.com/catchdave/arduino-gb/blob/master/libraries/Shifter/Shifter.h

### Info

- Necessary 6 circuits of 8 bits, 48 bits of communication in total.
- 8 digital outputs each, 48 total outputs.
- Each motor needs 4 outputs, we can control a total of 12 motors.

### Example code

```
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
```

```
#include <Shifter.h>

#define SER_Pin 4 //SER_IN - DATA
#define RCLK_Pin 3 //L_CLOCK
#define SRCLK_Pin 2 //CLOCK

#define NUM_REGISTERS 2 //how many registers are in the chain


//initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS);

void setup(){

}

void loop(){
 shifter.clear(); //set all pins on the shift register chain to LOW
 shifter.write(); //send changes to the chain and display them
 
 delay(1000);

 for (int n = 0; n < 159 ; n++)
 {
 shifter.setPin(n, HIGH); //set pin 1 in the chain(second pin) HIGH
 }
 shifter.write(); //send changes to the chain and display them
 
 delay(1000);
 
 shifter.setAll(HIGH); //Set all pins on the chain high
 shifter.write(); //send changes to the chain and display them
 
 delay(1000);
 }
```

```
int dataPin = 4;
int latchPin = 3;
int clockPin = 2;

byte output = 0;
 
void setup() 
{
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}
 
void loop() 
{
  Serial.println("Off all");
  updateShiftRegister(0x0000);
  delay(800);
  Serial.println("On all");
  updateShiftRegister(0xFFFF);
  delay(800);
}
 
void updateShiftRegister(byte data)
{
   Serial.println(data);
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, data);
   digitalWrite(latchPin, HIGH);
}
```


```
int dataPin = 4;
int latchPin = 3;
int clockPin = 2;

byte output = 0;
 
void setup() 
{
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}
 
void loop() 
{
  led_example();
}
 
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, output);
   digitalWrite(latchPin, HIGH);
}

void led_example() 
{
  output = 0;
  Serial.println("off-all");
  updateShiftRegister();
  delay(500);
  bitSet(output, 0);
  bitSet(output, 7);
  updateShiftRegister();
  delay(500);
}
```

```
/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * 74HC595 shift register
 * LEDs attached to each of the outputs of the shift register

 */
//Pin connected to ST_CP (12) of 74HC595
int latchPin = 12;
//Pin connected to SH_CP (11) of 74HC595
int clockPin = 11;
////Pin connected to DS (14) of 74HC595
int dataPin = 10;

//holders for information you're going to pass to shifting function
byte data;
byte dataArray[10];

void customDelay() {
  delayMicroseconds(1000);
}

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);

  //Binary notation as comment
  dataArray[0] = 0xFF; //0b11111111
  dataArray[1] = 0xFE; //0b11111110
  dataArray[2] = 0xFC; //0b11111100
  dataArray[3] = 0xF8; //0b11111000
  dataArray[4] = 0xF0; //0b11110000
  dataArray[5] = 0xE0; //0b11100000
  dataArray[6] = 0xC0; //0b11000000
  dataArray[7] = 0x80; //0b10000000
  dataArray[8] = 0x00; //0b00000000
  dataArray[9] = 0xE0; //0b11100000
}

void loop() {
  for (int j = 0; j < 9; j++) {
    //load the light sequence you want from array
    data = dataArray[j];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    customDelay();
    //move 'em out
    shiftOut(dataPin, clockPin, data);
    shiftOut(dataPin, clockPin, data);
    shiftOut(dataPin, clockPin, data);
    shiftOut(dataPin, clockPin, data);
        
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    customDelay();
    delay(50);
  }
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  customDelay();
  digitalWrite(myClockPin, 0);
  customDelay();

  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);
    customDelay();

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else { 
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    customDelay();
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    customDelay();
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
    customDelay();
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
  customDelay();
}
```

```
int latchPin = 3;
int clockPin = 2;
int dataPin = 4;

//holders for information you're going to pass to shifting function
byte data;
byte dataArray[10];

void customDelay() {
  delayMicroseconds(1000);
}

void setup() {
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);

  //Binary notation as comment, animation
  dataArray[0] = 0xFF; //0b11111111
  dataArray[1] = 0xFE; //0b11111110
  dataArray[2] = 0xFC; //0b11111100
  dataArray[3] = 0xF8; //0b11111000
  dataArray[4] = 0xF0; //0b11110000
  dataArray[5] = 0xE0; //0b11100000
  dataArray[6] = 0xC0; //0b11000000
  dataArray[7] = 0x80; //0b10000000
  // 0x8080; //0b1000000010000000
  dataArray[8] = 0x00; //0b00000000
  dataArray[9] = 0xE0; //0b11100000
}

void loop() {
  for (int j = 0; j < 9; j++) {
    //load the light sequence you want from array
    data = dataArray[j];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    customDelay();
    //move 'em out
    Custom_shiftOut(dataPin, clockPin, data);
    Custom_shiftOut(dataPin, clockPin, data);
    Custom_shiftOut(dataPin, clockPin, data);
    Custom_shiftOut(dataPin, clockPin, data);
        
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    customDelay();
    delay(50);
  }
}

// the heart of the program
void Custom_shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  customDelay();
  digitalWrite(myClockPin, 0);
  customDelay();

  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);
    customDelay();

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else { 
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    customDelay();
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    customDelay();
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
    customDelay();
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
  customDelay();
}
```

```
//Pin connected to ST_CP (12) of 74HC595
int latchPin = 12;
//Pin connected to SH_CP (11) of 74HC595
int clockPin = 11;
////Pin connected to DS (14) of 74HC595
int dataPin = 10;

int numDigitArray[7]  = {0, 0, 0, 0, 0, 0, 0};
int dotPointArray[7]  = {0, 0, 0, 0, 0, 0, 0};

void displayValueAndDigit(byte value, byte digit, byte dp, byte NDY) {
    digitalWrite(latchPin, 0);
    customDelay();
    shiftOut(dataPin, clockPin, value);
    shiftOut(dataPin, clockPin, digit);
    shiftOut(dataPin, clockPin, dp);
    shiftOut(dataPin, clockPin, 0x00);
    digitalWrite(latchPin, 1);
    customDelay();
}

void displayNumberInDigit(int numberToDisplayInDigit, int digitNumber, byte dotPointHex) {
  byte numberHex, digitNumberHex;
  
  switch(numberToDisplayInDigit)
  {
    case 0:
      numberHex = 0x00; 
      break;
    case 1:
      numberHex = 0x01;
      break;
    case 2:
      numberHex = 0x02;
      break;
    case 3:
      numberHex = 0x03;
      break;
    case 4:
      numberHex = 0x04; 
      break;
    case 5:
      numberHex = 0x05;
      break;
    case 6:
      numberHex = 0x06;
      break;
    case 7:
      numberHex = 0x07;
      break;
    case 8:
      numberHex = 0x08;
      break;
    case 9:
      numberHex = 0x09;
      break;
    case 15:
      numberHex = 0x0F;
      break;
    default:
      numberHex = 0x00;
      break;
  }

  switch(digitNumber) {
    case 0:
      digitNumberHex = 0xBF;
      break;
    case 1:
      digitNumberHex = 0xDF;
      break;
    case 2:
      digitNumberHex = 0xEF;
      break;
    case 3:
      digitNumberHex = 0xF7;
      break;
    case 4:
      digitNumberHex = 0xFB;
      break;
    case 5:
      digitNumberHex = 0xFD;
      break;
    case 6:
      digitNumberHex = 0xFE;
      break;
    default:
      digitNumberHex = 0x00;
      break;
  }

  displayValueAndDigit(numberHex, digitNumberHex, dotPointHex, 0x00);
  displayValueAndDigit(numberHex, 0xFF, dotPointHex, 0x00);
}

int decPartProcess(double decPart) {
  
  int i = 0;
  double decPartMult10, decPartMult100, tempFractionalPart;
  int dotPointPosition;
  int decPartArray[] = {0, 0};
  int decPartToInt = int(decPart * 100);

  //  decPart      decPartToInt
  //  .0           0
  //  .05          5
  //  .5           50
  //  .58          58

  decPartMult10 = decPart * 10;
  decPartArray[0] = (int) decPartMult10;
  tempFractionalPart = decPartMult10 - decPartArray[0]; 
  
  decPartMult100 = tempFractionalPart * 10;
  decPartArray[1] = (int) decPartMult100;
  tempFractionalPart = decPartMult100 - decPartArray[1];

  //  decPartArray[0]     decPartArray[1]   
  //  0                   0
  //  0                   5
  //  5                   0
  //  5                   5

  if(decPartArray[0] == 0 && decPartArray[1] == 0) {
    dotPointArray[0] = 0;
    dotPointArray[1] = 0;
    dotPointArray[2] = 0;
    dotPointArray[3] = 0;
    dotPointArray[4] = 0;
    dotPointArray[5] = 0;
    dotPointArray[6] = 0; 
    
    dotPointPosition = 6;
    
  } else if(decPartArray[0] == 0 && decPartArray[1] > 0) {
    dotPointArray[0] = 0;
    dotPointArray[1] = 0;
    dotPointArray[2] = 0;
    dotPointArray[3] = 0;
    dotPointArray[4] = 1;
    dotPointArray[5] = 0;
    dotPointArray[6] = 0;    

    numDigitArray[5] = decPartArray[0];
    numDigitArray[6] = decPartArray[1];
    dotPointPosition = 4;
         
  } else if(decPartArray[0] > 0 && decPartArray[1] == 0) {
    dotPointArray[0] = 0;
    dotPointArray[1] = 0;
    dotPointArray[2] = 0;
    dotPointArray[3] = 0;
    dotPointArray[4] = 0;
    dotPointArray[5] = 1;
    dotPointArray[6] = 0;   

    numDigitArray[6] = decPartArray[0];
    dotPointPosition = 5;
    
  } else if(decPartArray[0] > 0 && decPartArray[1] > 0) {
    dotPointArray[0] = 0;
    dotPointArray[1] = 0;
    dotPointArray[2] = 0;
    dotPointArray[3] = 0;
    dotPointArray[4] = 1;
    dotPointArray[5] = 0;
    dotPointArray[6] = 0;    

    numDigitArray[5] = decPartArray[0];
    numDigitArray[6] = decPartArray[1];
    dotPointPosition = 4;
  }

  for(i = 0; i< 7;i++) {
    Serial.print(numDigitArray[i]);
  }

  Serial.print(" ");
  Serial.print(dotPointPosition);
  
  Serial.println("");

  return(dotPointPosition);
}

void intPartProcess(long int intPart, int dotPointPosition, byte sign) {
  //  intPart     dotPointPosition
  //  846         4
  int i = 0;  
  int quotient;
  int tempDotPointPosition = dotPointPosition;
  long int tempIntPart = intPart;

  for(i=0;i<=dotPointPosition;i++) {
    numDigitArray[i] = 15;
  }

  if(tempIntPart == 0) {
    numDigitArray[tempDotPointPosition] = 0;
  }
  
  while(tempIntPart > 0) {
    numDigitArray[tempDotPointPosition] = tempIntPart % 10;
    tempIntPart /= 10;
    tempDotPointPosition--;  
    if(tempDotPointPosition < 0) {
      break;
    }
  }

  if(sign == 0x80) {
    numDigitArray[0] = 15;
  }
}

void customDelay() {
  delayMicroseconds(1);
}

void displayNumber(int dotPointPosition, byte sign) {
  int i;
  byte dotPointHex, dotAndSign;
  
  switch(dotPointPosition) {
    case 0:
      dotPointHex = 0x40;
      break;
    case 1:
      dotPointHex = 0x20;
      break;
    case 2:
      dotPointHex = 0x10;
      break;
    case 3:
      dotPointHex = 0x08;
      break;
    case 4:
      dotPointHex = 0x04;
      break;
    case 5:
      dotPointHex = 0x02;
      break;
    case 6:
      dotPointHex = 0x00;
      break;
    default:
      dotPointHex = 0x00;         
      break;
  }

  dotAndSign = dotPointHex | sign;
  
  for(i=0;i<7;i++) {
    displayNumberInDigit(numDigitArray[i], i, dotAndSign);
  }
}

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  byte sign = 0x00;

  // Upper limit +32767
  // Lower limit -32767
  //long double numberToDisplay = -2689.6;
  long double numberToDisplay = random(-32767, 32767) / 15.0;
  
  if(numberToDisplay < 0.0) {
    numberToDisplay*=-1;
    sign = 0x80;
  }
  
  long int intPart = (int) numberToDisplay;
  float decPart = numberToDisplay - intPart;
  int dotPointPosition, i;

  dotPointPosition = decPartProcess(decPart);
  intPartProcess(intPart, dotPointPosition, sign);

  displayNumber(dotPointPosition, sign);

  delay(300);
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  customDelay();
  digitalWrite(myClockPin, 0);
  customDelay();

  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);
    customDelay();
    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else { 
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    customDelay();
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    customDelay();
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
    customDelay();
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
  customDelay();
}
```