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

extern double kp;
extern double ki;
extern double kd;
extern const int pidSampleTime;

extern double rpm;
extern double motorOutput;
extern double RPMSetpoint;
extern float sheavePosition;
extern float motorRunTime;
extern float motorStartTime;
extern bool motorIsMoving;


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