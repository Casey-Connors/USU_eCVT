#ifndef MotorController_h
#define MotorController_h

#include "../avr/cores/arduino/Arduino.h"

#define MAX_SPEED 225               //This is the maximum pwm value that the motor should be set to.
#define MIN_SPEED 0                 //This is the minimum pwm value that the motor should be set to. When at this value, the motor should not be running.
#define MM_PER_REV 20               //Distance that the sheave moves (in mm) per 1 revolution of the motor output shaft. THIS VALUE WAS RANDOMLY CHOSEN. NEED TO DETERMINE THE COORECT VALUE.
#define DOWNSHIFT 1                 //Anywhere in the code that says DOWNSHIFT is replaced with zero. Allows booleans to be used for motor direction.
#define UPSHIFT 0                   //Anywhere in the code that says UPSHIFT is replaced with one. Allows booleans to be used for motor direction.
#define MAX_THROTTLE 10             //Threshold that defines what is considered maximum throttle. Used in shifting algorithm.
#define MIN_RPM 10                  //Threshold for what is considered zero RPM
#define RPM_GEAR_TEETH 1            //Number of teeth on the gear that the tachometer is measuring from.
#define SHEAVE_LIMIT_MAX 20         //Maximum distance (in mm) from home the sheave can travel (cant go further than this)
#define SHEAVE_LIMIT_MIN 0          //Minimum distance (in mm) from home the sheave can travel (cant go closer than this)

const int SAM_Hall_Pin = 3;          //This pin takes the square wave from the sheave actuation motor hall effect sensor to track sheave position 
const int directionPin = 4;          //on = downshift, off = upshift
const int brakePin = 5;              //Brake is on when its connected to ground, off otherwise
const int tachPin = 7;               //Pin that takes input from the hall effect sensor that is being used as a tachometer. This HAS to stay at pin 7 becuase its one of the few than can work for intterupts.
const int controlPin = 8;            //Controls the speed of the motor. Will be pwm control. Maybe map 0-255 to 0-100 for percentage of speed.
const int throttlePositionPin = A0;  //Pin that takes input from the throttle position sensor
const int neutralPin = 6;

extern double kp;
extern double ki;
extern double kd;
extern const int pidSampleTime;

extern double rpm;
extern double motorOutput;
extern double RPMSetpoint;
double sheavePosition = 0;

int GetThrottlePosition(void);

void EnableBrake(void);

void DisableBrake(void);

void SetDirection(bool dir);

double CalculatePeakPowerRPM(double throttlePosition);

void ComputeMotorOutput(void);

void Shift(void);

void Upshift(float, float);

void Downshift(float, float);

void InitializeController(void);

#endif  //MotorController_h