#ifndef Motor_Hall_Effect_h
#define Motor_Hall_Effect_h

#define CW 1
#define CCW -1
#define PULSES_PER_MM 10

const int Upin = 2;
const int Vpin = 3;
const int Wpin = 4;

void HallSensorU_Dispatch();
void HallSensorV_Dispatch();
void HallSensorW_Dispatch();


class Motor_Hall_Effect{
    public:
        Motor_Hall_Effect(void (*UISR)() , void (*VISR)() , void (*WISR)());
        float getPosition();
        int getDirection();

        void HallSensorU();
        void HallSensorV();
        void HallSensorW();
    private:
        

        bool HSU_Val;		        // Current U sensor state
        bool HSV_Val;		        // Current V sensor state 
        bool HSW_Val;		        // Current W sensor state
        int direct;				    // Integer variable to store BLDC rotation direction
        unsigned long pulseCount;	// Integer variable to store the pulse count (dont need to worry about overflow since uLong can store UUUGGGEE nubmers)
        float startTime;			// Float variable to store the start time of the current interrupt 
        float prevTime; 			// Float variable to store the start time of the previous interrupt 
        float pulseTimeW; 			// Float variable to store the elapsed time between interrupts for hall sensor W 
        float pulseTimeU; 			// Float variable to store the elapsed time between interrupts for hall sensor U 
        float pulseTimeV; 			// Float variable to store the elapsed time between interrupts for hall sensor V 
        float AvPulseTime; 			// Float variable to store the average elapsed time between all interrupts 
        float PPM;				    // Float variable to store calculated pulses per minute
        float RPM; 				    // Float variable to store calculated revolutions per minute
};

extern Motor_Hall_Effect motor;
#endif  //Motor_Hall_Effect_h