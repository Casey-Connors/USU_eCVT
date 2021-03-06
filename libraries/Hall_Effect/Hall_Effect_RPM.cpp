#include <Arduino.h>
#include "Hall_Effect_RPM.h"
#include <pin_mapping.h>

//These can be defined in the class. Would have to do the extern and ISR wrapper trick like with the motor_hall_effect
static volatile unsigned long LastTimeWeMeasured;                      // Stores the last time we measured a pulse so we can calculate the period.
static volatile unsigned long PeriodBetweenPulses;  // Stores the period between pulses in microseconds.
static unsigned int AmountOfReadings;
static unsigned int PulseCounter;                                  // Counts the amount of pulse readings we took so we can average multiple pulses before calculating the period.
static unsigned long PeriodSum;                                        // Stores the summation of all the periods to do the average.
static volatile unsigned long PeriodAverage;        // Stores the period between pulses in microseconds in total, if we are taking multiple pulses.
unsigned long FrequencyRaw;                                     // Calculated frequency, based on the period. This has a lot of extra decimals without the decimal point.
unsigned long FrequencyReal;                                    // Frequency without decimals.
unsigned long RPM;                                              // Raw RPM without any processing.
unsigned long LastTimeCycleMeasure;        // Stores the last time we measure a pulse in that cycle.
																	// We need a variable with a value that is not going to be affected by the interrupt
																	// because we are going to do math and functions that are going to mess up if the values
																	// changes in the middle of the cycle.
unsigned long CurrentMicros;                         // Stores the micros in that cycle.
																	// We need a variable with a value that is not going to be affected by the interrupt
																	// because we are going to do math and functions that are going to mess up if the values
																	// changes in the middle of the cycle.

unsigned int ZeroDebouncingExtra;  // Stores the extra value added to the ZeroTimeout to debounce it.
									// The ZeroTimeout needs debouncing so when the value is close to the threshold it
									// doesn't jump from 0 to the value. This extra value changes the threshold a little
									// when we show a 0.


unsigned long readIndex;  // The index of the current reading.
unsigned long total;  // The running total.
unsigned long average;  // The RPM value after applying the smoothing.

unsigned long readings[10];  // The input.

void RPM_Pulse_Event() {                                 
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;    
  LastTimeWeMeasured = micros();                        

  if(PulseCounter >= AmountOfReadings)                 
  {
    PeriodAverage = PeriodSum / AmountOfReadings; 
    PulseCounter = 1;  
    PeriodSum = PeriodBetweenPulses;

    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);
  
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);  
    AmountOfReadings = RemapedAmountOfReadings;  
  }
  else
  {
    PulseCounter++;
    PeriodSum = PeriodSum + PeriodBetweenPulses; 
  }
  
}

void initializeRPM(){
	PeriodBetweenPulses = ZeroTimeout+1000;
	PeriodAverage = ZeroTimeout+1000;
	PulseCounter = 1;
	LastTimeCycleMeasure = LastTimeWeMeasured;
	CurrentMicros = micros();
	AmountOfReadings = 1;

  pinMode(tachPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(tachPin), RPM_Pulse_Event, FALLING);
}

unsigned long GetRPM(){
  LastTimeCycleMeasure = LastTimeWeMeasured;  // Last cycle time
  CurrentMicros = micros();                   // Current amount of time since the program started

  if(CurrentMicros < LastTimeCycleMeasure) {  //Check to make sure that the amount of time has increased since last cycle. This just eliminates an error case.
    LastTimeCycleMeasure = CurrentMicros;
  }

  FrequencyRaw = 10000000000 / PeriodAverage;  // Calculate the frequency using the period between pulses.

  if(PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra) {  // If the pulses are too far apart that we reached the timeout for zero:
    FrequencyRaw = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;  // Change the threshold a little so it doesn't bounce.
  }
  else{
    ZeroDebouncingExtra = 0;  // Reset the threshold to the normal value so it doesn't bounce.
  }

  FrequencyReal = FrequencyRaw / 10000;  // Get frequency without decimals.
                                          // This is not used to calculate RPM but we remove the decimals just in case
                                          // you want to print it.

  RPM = FrequencyRaw / gearTeeth * 60;        //Calculates revolutions per minute
  RPM = RPM / 10000;  // Remove the decimals.

  total = total - readings[readIndex];       // Advance to the next position in the array.
  readings[readIndex] = RPM;                 // Takes the value that we are going to smooth.
  total = total + readings[readIndex];       // Add the reading to the total.
  readIndex = readIndex + 1;                 // Advance to the next position in the array.

  if (readIndex >= numReadings){             // If we're at the end of the array:
    readIndex = 0;                           // Reset array index.
  }
  
  average = total / numReadings;             // The average value is the smoothed result.

  return average;
}

