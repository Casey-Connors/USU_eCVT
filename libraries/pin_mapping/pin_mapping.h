#ifndef PIN_MAPPING_h
#define PINS_MAPPING_h

#include "../avr/cores/arduino/Arduino.h"

//Analog Output Pins


//Analog Input Pins
const int throttlePositionPin = A0;  //Throttle position (potentiometer)        -- 0: NO THROTTLE   1023: FULL THROTTLE


//Digital Output Pins
const int brakePin = 5;              //Sheave actuatio motor brake              -- HIGH: BRAKE ON   LOW: BRAKE OFF
const int directionPin = 8;          //Sheave direction                         -- HIGH: Downshift  LOW: Upshift
const int controlPin = 9;            //Controls the speed of the motor. Will be pwm control. Maybe map 0-255 to 0-100 for percentage of speed.


//Digital Input Pins
const int neutralPin = 6;            //Neutral/drive selection                  -- HIGH: DRIVE      LOW: NEUTRAL
const int tachPin = 7;               //Engine tachometer (hall effect sensor)   -- HIGH: Triggered  LOW: NOT TRIGGERED
const int Upin = 2;                  //BLDC Hall effect sensor U
const int Vpin = 3;                  //BLDC Hall effect sensor V
const int Wpin = 4;                  //BLDC Hall effect sensor W

#endif  //PIN_MAPPING_h