#ifndef Motor_Hall_Effect_h
#define Motor_Hall_Effect_h

#define CW 1
#define CCW -1
constexpr float PULSES_PER_MM = 100.00;

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
        long pulseCount;	        // Integer variable to store the pulse count (dont need to worry about overflow since uLong can store UUUGGGEE nubmers)
    private:
        bool HSU_Val;		        // Current U sensor state
        bool HSV_Val;		        // Current V sensor state 
        bool HSW_Val;		        // Current W sensor state
        int direct;				    // Integer variable to store BLDC rotation direction
        
};

extern Motor_Hall_Effect motor;
#endif  //Motor_Hall_Effect_h