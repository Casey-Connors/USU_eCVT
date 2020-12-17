#ifndef Motor_Hall_Effect_h
#define Motor_Hall_Effect_h

#define CW 1
#define CCW -1
constexpr float PULSES_PER_MM = 1000.00;

void initialize_motor_hall_effect();
float getPosition();
int getDirection();
void HallSensorU();
void HallSensorV();
void HallSensorW();
extern volatile long pulseCount;	// Integer variable to store the pulse count (dont need to worry about overflow since uLong can store UUUGGGEE nubmers)
 
extern bool HSU_Val;		        // Current U sensor state
extern bool HSV_Val;		        // Current V sensor state 
extern bool HSW_Val;		        // Current W sensor state
extern int direct;				    // Integer variable to store BLDC rotation direction
        
#endif  //Motor_Hall_Effect_h