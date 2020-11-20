#include "Motor_Hall_Effect.h"
#include <Arduino.h>
#include "MotorController.h"

//This whole dispatch thing is wierd but its the easiest way to be able to call the member function ISR's from attachInterrupt in the constructor

void HallSensorU_Dispatch(){
    motor.HallSensorU();
}

void HallSensorV_Dispatch(){
    motor.HallSensorV();
}

void HallSensorW_Dispatch(){
    motor.HallSensorW();
}

Motor_Hall_Effect::Motor_Hall_Effect(void (*UISR)() , void (*VISR)() , void (*WISR)()){
    pinMode(Upin, INPUT);			
    pinMode(Vpin, INPUT);			
    pinMode(Wpin, INPUT);

    attachInterrupt(digitalPinToInterrupt(Upin), HallSensorU_Dispatch, CHANGE);      
    attachInterrupt(digitalPinToInterrupt(Vpin), HallSensorV_Dispatch, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Wpin), HallSensorW_Dispatch, CHANGE);
}

float Motor_Hall_Effect::getPosition(){
    return (pulseCount * PULSES_PER_MM);
}

int Motor_Hall_Effect::getDirection(){
    return direct;
}

void Motor_Hall_Effect::HallSensorU(){
    startTime = millis();
    HSU_Val = digitalRead(Upin);
    HSW_Val = digitalRead(Wpin);					// Read the current W (or V) hall sensor value		
    direct = (HSU_Val == HSW_Val) ? CW : CCW;
    pulseCount = pulseCount + (1 * direct);
    pulseTimeU = startTime - prevTime;				
    AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);		
    PPM = (1000 / AvPulseTime) * 60;					
    RPM = PPM / 90;
    prevTime = startTime;
}

void Motor_Hall_Effect::HallSensorV(){
    startTime = millis();
    HSV_Val = digitalRead(3);
    HSU_Val = digitalRead(2);					// Read the current U (or W) hall sensor value 
    direct = (HSV_Val == HSU_Val) ? CW : CCW;
    pulseCount = pulseCount + (1 * direct);
    pulseTimeV = startTime - prevTime;				
    AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);		
    PPM = (1000 / AvPulseTime) * 60;					
    RPM = PPM / 90;
    prevTime = startTime;
}

void Motor_Hall_Effect::HallSensorW(){
  startTime = millis();						// Set startTime to current microcontroller elapsed time value
  HSW_Val = digitalRead(4);					// Read the current W hall sensor value
  HSV_Val = digitalRead(3);						// Read the current V (or U) hall sensor value 
  direct = (HSW_Val == HSV_Val) ? CW : CCW;			// Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * direct);				// Add 1 to the pulse count
  pulseTimeW = startTime - prevTime;				// Calculate the current time between pulses
  AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);	// Calculate the average time time between pulses
  PPM = (1000 / AvPulseTime) * 60;					// Calculate the pulses per min (1000 millis in 1 second)
  RPM = PPM / 90;						// Calculate revs per minute based on 90 pulses per rev
  prevTime = startTime;						// Remember the start time for the next interrupt
}