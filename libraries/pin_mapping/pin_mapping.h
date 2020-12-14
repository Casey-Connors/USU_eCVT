#ifndef PIN_MAPPING_h
#define PINS_MAPPING_h

#include <Arduino.h>


//Analog Input Pins
const int throttlePositionPin = 10;  //PIN A10. ONLY USE IN ANALOG. Throttle position (potentiometer)        -- 0: NO THROTTLE   1023: FULL THROTTLE


//Digital Output Pins
const int controlPin        = 2;            //Controls the speed of the motor. Will be pwm control. Maybe map 0-255 to 0-100 for percentage of speed.
const int directionPin      = 3;          //Sheave direction                         -- HIGH: Downshift  LOW: Upshift
const int enablePin         = 4;
const int brakePin          = 5;              //Sheave actuatio motor brake              -- HIGH: BRAKE ON   LOW: BRAKE OFF


//Digital Input Pins
const int alarmPin          = 6;
const int Upin              = 7;                  //BLDC Hall effect sensor U
const int Vpin              = 8;                  //BLDC Hall effect sensor V
const int Wpin              = 9;                  //BLDC Hall effect sensor W
const int neutralPin        = 10;            //Neutral/drive selection                  -- HIGH: DRIVE      LOW: NEUTRAL
const int tachPin           = 11;               //Engine tachometer (hall effect sensor)   -- HIGH: Triggered  LOW: NOT TRIGGERED
#endif  //PIN_MAPPING_h