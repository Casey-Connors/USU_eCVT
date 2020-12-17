#include <TCU.h>
#include <MotorController.h>
#include <CommandLine.h>
#include <Motor_Hall_Effect.h>
#include <pin_mapping.h>

char CommandLine[COMMAND_BUFFER_LENGTH + 1];
float lastTime = 0;
float currentTime = 0;
float lastSpeed = 0;
float currentSpeed = 0;
float timeDelta = 0;
float lastPosTime = 0;
float posUpdateInterval = 1000;

Motor_Hall_Effect motor = Motor_Hall_Effect(HallSensorU_Dispatch, HallSensorV_Dispatch, HallSensorW_Dispatch);

void setup() {
  InitializeController();
}

void loop() {
  if(millis() - lastPosTime >= posUpdateInterval){
    Serial.print("U: ");
    Serial.print(digitalRead(Upin));
    Serial.print(" || V: ");
    Serial.print(digitalRead(Vpin));
    Serial.print(" || W: ");
    Serial.println(digitalRead(Wpin));
    Serial.print("   ");
    Serial.print(motor.pulseCount);
    sheavePosition = motor.getPosition();
    Serial.print("    Sheave position: ");
    Serial.println(sheavePosition);
    lastPosTime = millis();
  }
  
  //If the system is in drive (i.e. the neutral switch is in the ON or HIGH position)
  //Note that this currently relies on the transmission being in the netural position when the system turns on. This will need updated for the sheave limit sensor
  if(/*digitalRead(neutralPin)*/0){
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
    bool received = getCommandLineFromSerialPort(CommandLine);         //gets a command from the command line if their is one (and returns true in that case)                
    if(received){                                                      //If there is a command stored in the command buffer (i.e. if there was a command in the command line).
      ExecuteCommand(CommandLine);                                        //Execute command. See CommandLine.cpp for implimentation 
    }

    if(motorIsMoving){
      if(((millis()-motorStartTime)/1000) >= motorRunTime){              //This is my stupid way of checking if the motor has moved the desired amount without using delay()
        EnableBrake();                                                    //Delay() kinda has problems when interrupts are constantly being called so this was the best I could come up with to
        motorIsMoving = false;                                            //get around that problem because, unlike Rick Sanchez, I am a hack
      }                                                                 
    }
  }
}
