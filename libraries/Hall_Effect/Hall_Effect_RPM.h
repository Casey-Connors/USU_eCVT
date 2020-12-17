#ifndef Hall_Effect_RPM_h
#define Hall_Effect_RPM_h

//I stole most of this code from somewhere on the internet. Don't @ me.
void initializeRPM();
unsigned long GetRPM();

const byte gearTeeth = 1;                                       //The number of teeth on the gear which the RPM is being measured from
const unsigned long ZeroTimeout = 100000;                       //This is the amount of time that must pass between pulses for the system to decide the RPM is zero
const byte numReadings = 2;                                     //The number of readings over which the RPM averages the value. Higher value = more smoothing but more lag


# endif