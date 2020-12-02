#include "MotorController.h"
#include "../avr/cores/arduino/Arduino.h"
#include <PID_v1.h>
#include <Hall_Effect_RPM.h>
#include "Motor_Hall_Effect.h"
#include <pin_mapping.h>

double kp = 1.2;
double ki = 0.25;
double kd = 1.5;
const int pidSampleTime = 200;

double rpm = 0;
double motorOutput = 0;
double RPMSetpoint = 0;
float sheavePosition = 0;
float motorRunTime = 0;
float motorStartTime = 0;
bool motorIsMoving = false;

PID motorPID(&rpm, &motorOutput, &RPMSetpoint, kp, ki, kd, DIRECT);    //Set variables for PID system. See documentation for PID library
Hall_Effect_RPM tach(tachPin);

/*************************************************************************************************************
                                            Monitoring Functions
*************************************************************************************************************/
int GetThrottlePosition(void){
  return map(analogRead(throttlePositionPin), 0, 1023, 0, 100);  //Maps the reading from the linear potentiometer to a percentage
}


/*************************************************************************************************************
                                            eCVT Control Functions
*************************************************************************************************************/
void EnableBrake(void){
  digitalWrite(controlPin, MIN_SPEED);
  digitalWrite(brakePin, HIGH);                      //Pull the brake pin to ground. This should enable the brake. Double check that its not backwards.
}

void DisableBrake(void){
  digitalWrite(brakePin, LOW);                     
}

void SetDirection(bool dir){
  if(dir){
    digitalWrite(directionPin, LOW);                //Pull the direction pin to ground. This will set the direction but I don't know which direction is which.
  } else{
    digitalWrite(directionPin, HIGH);               //Same concern as with the disable brake command
  }
}

double CalculatePeakPowerRPM(double throttlePosition){
  return 2500;                                    //Need to calculate the peak power rpm based on throttle position
}

void ComputeMotorOutput(void){
  RPMSetpoint = CalculatePeakPowerRPM(GetThrottlePosition());          //Calculate peak power RPM based on current throttle
  tach.GetRPM();
  motorPID.Compute();                                                  //PID compute to determine required motor output

  if(sheavePosition >= SHEAVE_LIMIT_MAX){                              //If the sheave is at its max limit                           
    if(motorOutput > 0){  
      motorOutput = 0;                                                    //Clamp motor output to only 0 or negative number
    }
  }
  else if (sheavePosition <= SHEAVE_LIMIT_MIN){                        //If the sheave is at its min limit 
    if(motorOutput < 0){
      motorOutput = 0;                                                    //Clamp motor output to only 0 or a positive number
    }
  }
}

void Shift(void){
  if(motorOutput > 0){                                       //If the motor output is greater than 0, that means the eCVT needs to upshift
      DisableBrake();
      SetDirection(UPSHIFT);                                 //Set the direction to upshift
      digitalWrite(controlPin, abs(motorOutput));            //Run the motor in the upshift direction at the speed computed by the PID system
  }
  else if(motorOutput < 0){                                  //If the motor output is less than 0, that means the eCVT needs to downshift
    DisableBrake();
    SetDirection(DOWNSHIFT);                                 //Set the direction to downshift
    digitalWrite(controlPin, abs(motorOutput));              //Run the motor in the downshift direction at the speed computed by the PID system
  }
  else{
    EnableBrake();
  }
}

void Upshift(float distance, float rate){         //distance is the distance in mm to move the sheave
  motorRunTime = distance/(MM_PER_REV);        //Computes the amount of time the motor has to run in order to move the sheave [distance] amount
  rate = constrain(rate, MIN_SPEED, MAX_SPEED);
  
  Serial.print("The sheave upshift ");
  Serial.print(distance);
  Serial.print(" mm at a rate of ");
  Serial.print(rate);
  Serial.print(" mm per minute");

  DisableBrake();                                   //Disable the motor brake
  SetDirection(UPSHIFT);                           //Set the motor direction to backward
  motorStartTime = millis();
  analogWrite(controlPin, rate);               //Set the motor
  motorIsMoving = true;
}

void Downshift(float distance, float rate){
  motorRunTime = distance/(MM_PER_REV);          //Computes the amount of time the motor has to run in order to move the sheave [distance] amount
  rate = constrain(rate, MIN_SPEED, MAX_SPEED);
 
  Serial.print("The sheave will move ");
  Serial.print(distance);
  Serial.print(" mm forward\n");

  DisableBrake();                                   //Disable the motor brake
  SetDirection(DOWNSHIFT);                            //Set the motor direction to forward
  motorStartTime = millis();
  analogWrite(controlPin, rate);
  motorIsMoving = true;              
}

/*************************************************************************************************************
                                            Initialization
*************************************************************************************************************/
void InitializeController(void) {
  pinMode(controlPin, OUTPUT);          //controlPin is an output pin
  pinMode(directionPin, OUTPUT);        //directionPin is an output pin
  pinMode(brakePin, OUTPUT);            //brakePin is an output pin
  pinMode(tachPin, INPUT);              //tachPin is an input pin
  pinMode(neutralPin, INPUT);
  pinMode(Upin, INPUT);
  pinMode(Vpin, INPUT);
  pinMode(Wpin, INPUT);
  sheavePosition = 0;

  motorPID.SetMode(AUTOMATIC);          //PID mode set to automatic. See documentation for PID library
  motorPID.SetOutputLimits(-MAX_SPEED, MAX_SPEED);   //Set output limits to be from - max speed to max speed. Later, if the pid outputs a negative value we interpret that as a different direction and change the motor accordingly
  motorPID.SetSampleTime(pidSampleTime);
  motorPID.SetControllerDirection(REVERSE);  //As motor output goes up, RPM goes down. This makes it a reverse process.
  
  delay(1000);
  sheavePosition = 0;                                                    
}