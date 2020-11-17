#include <TCU.h>
#include <MotorController.h>
#include <CommandLine.h>

char CommandLine[COMMAND_BUFFER_LENGTH + 1];
float lastTime = 0;
float currentTime = 0;
float lastSpeed = 0;
float currentSpeed = 0;
float timeDelta = 0;

void setup() {
  InitializeController();
}

void loop() {
  //If the system is in drive (i.e. the neutral switch is in the ON or HIGH position)
  //Note that this currently relies on the transmission being in the netural position when the system turns on. This will need updated for the sheave limit sensor
  if(digitalRead(neutralPin)){
    currentSpeed = map(motorOutput, -MAX_SPEED, MAX_SPEED, -20, 20);

    if(currentSpeed != lastSpeed){
      currentTime = millis();
      timeDelta = currentTime - lastTime;
      sheavePosition += lastSpeed * timeDelta;
      lastTime = currentTime;
      lastSpeed = currentSpeed;
    }

    ShiftUpdate();
  }
  //Else if the system is in neutral (i.e the netural switch is in the OFF or LOW position) then use manual commands. Note that manual commands can ONLY be used when the system is set to neutral.
  else{
    bool received = getCommandLineFromSerialPort(CommandLine);         //gets a command from the command line if their is one. Should only run this when debugging. Maybe do a macro cut thing.                  
    if(received){                                                      //If there is a command stored in the command buffer.
      ExecuteCommand(CommandLine);                                        //Execute command
    }
  }
}
