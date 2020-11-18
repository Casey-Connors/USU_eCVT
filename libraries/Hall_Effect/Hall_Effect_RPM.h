#ifndef Hall_Effect_RPM_h
#define Hall_Effect_RPM_h

//I stole most of this code from somewhere on the internet. Don't @ me.
class Hall_Effect_RPM{
	public:
		Hall_Effect_RPM(int pin);
		int getPin();
		void setPin();
		unsigned long GetRPM();
	
	private:
		const byte gearTeeth = 1;                                       //The number of teeth on the gear which the RPM is being measured from
		const unsigned long ZeroTimeout = 100000;                       //This is the amount of time that must pass between pulses for the system to decide the RPM is zero
		const byte numReadings = 2;                                     //The number of readings over which the RPM averages the value. Higher value = more smoothing but more lag
		
																			// It has a big number so it doesn't start with 0 which would be interpreted as a high frequency.
																			// It has a big number so it doesn't start with 0 which would be interpreted as a high frequency.
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

		unsigned long readings[];  // The input.
};

# endif