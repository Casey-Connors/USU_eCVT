#include "Motor_Hall_Effect.h"
#include <Arduino.h>
#include "MotorController.h"
#include <pin_mapping.h>

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
    float position = float(pulseCount)/PULSES_PER_MM;
    return position;
}

int Motor_Hall_Effect::getDirection(){
    return direct;
}

void Motor_Hall_Effect::HallSensorU(){
    HSU_Val = digitalRead(Upin);
    HSW_Val = digitalRead(Wpin);					// Read the current W (or V) hall sensor value		
    direct = (HSU_Val == HSW_Val) ? CW : CCW;
    pulseCount = pulseCount + (1 * direct);
}

void Motor_Hall_Effect::HallSensorV(){
    HSV_Val = digitalRead(3);
    HSU_Val = digitalRead(2);					// Read the current U (or W) hall sensor value 
    direct = (HSV_Val == HSU_Val) ? CW : CCW;
    pulseCount = pulseCount + (1 * direct);
}

void Motor_Hall_Effect::HallSensorW(){
  HSW_Val = digitalRead(4);					// Read the current W hall sensor value
  HSV_Val = digitalRead(3);						// Read the current V (or U) hall sensor value 
  direct = (HSW_Val == HSV_Val) ? CW : CCW;			// Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * direct);				// Add 1 to the pulse count
}