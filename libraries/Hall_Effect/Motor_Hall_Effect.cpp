#include "Motor_Hall_Effect.h"
#include <Arduino.h>
#include "MotorController.h"
#include <pin_mapping.h>

long pulseCount;	        // Integer variable to store the pulse count (dont need to worry about overflow since uLong can store UUUGGGEE nubmers)
 
bool HSU_Val;		        // Current U sensor state
bool HSV_Val;		        // Current V sensor state 
bool HSW_Val;		        // Current W sensor state
int direct;				    // Integer variable to store BLDC rotation direction

void initialize_motor_hall_effect() {
    pinMode(Upin, INPUT);
    pinMode(Vpin, INPUT);
    pinMode(Wpin, INPUT);

    attachInterrupt(digitalPinToInterrupt(Upin), HallSensorU, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Vpin), HallSensorV, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Wpin), HallSensorW, CHANGE);
}

float getPosition(){
    float position = float(pulseCount)/PULSES_PER_MM;
    return position;
}

void HallSensorU(){
    HSU_Val = digitalRead(Upin);
    HSW_Val = digitalRead(Wpin);					// Read the current W (or V) hall sensor value		
    direct = (HSU_Val == HSW_Val) ? CW : CCW;
    pulseCount = pulseCount + (1 * direct);
}

void HallSensorV(){
    HSV_Val = digitalRead(3);
    HSU_Val = digitalRead(2);					// Read the current U (or W) hall sensor value 
    direct = (HSV_Val == HSU_Val) ? CW : CCW;
    pulseCount = pulseCount + (1 * direct);
}

void HallSensorW(){
  HSW_Val = digitalRead(4);					// Read the current W hall sensor value
  HSV_Val = digitalRead(3);						// Read the current V (or U) hall sensor value 
  direct = (HSW_Val == HSV_Val) ? CW : CCW;			// Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * direct);				// Add 1 to the pulse count
}