#include "CommandLine.h"
#include <MotorController.h>
#include <string.h>
#include <stdlib.h>

const char *DownshiftCommandToken         = "downshift";  
const char *UpshiftCommandToken           = "upshift";
const char *enableBrakeCommandToken       = "enableBrake";
const char *disableBrakeCommandToken      = "disableBrake";
const char *setGearRatioCommandToken      = "gearRatio";
const char *setDirectionCommandToken      = "direction";
const char *automaticControlCommandToken  = "automaticControl";
const char *manualControlCommandToken     = "manualControl";

const char *delimiters            = ", \n";                    //commands can be separated by return, space or comma

bool automaticControl = false;         //This defaults automatic control to off. This is just the default parameter. Dont change it here to enable automatic control, do that from the command line.

/*************************************************************************************************************
                                      Obtain command from command line
*************************************************************************************************************/
bool getCommandLineFromSerialPort(char * serialCommand){
  static uint8_t charsRead = 0;                      
  
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:      //likely have full command in buffer now, commands are terminated by CR and/or LS
      case LF:
        serialCommand[charsRead] = NULLCHAR;       //null terminate our command char array
        if (charsRead > 0)  {
          charsRead = 0;                           //charsRead is static, so have to reset
          Serial.println(serialCommand);
          return true;
        }
        break;
      case BS:                                    // handle backspace in input: put a space in last char
        if (charsRead > 0) {                        //and adjust commandLine and charsRead
          serialCommand[--charsRead] = NULLCHAR;
          Serial << byte(BS) << byte(SPACE) << byte(BS); 
        }
        break;
      default:
        // c = tolower(c);
        if (charsRead < COMMAND_BUFFER_LENGTH) {
          serialCommand[charsRead++] = c;
        }
        serialCommand[charsRead] = NULLCHAR;     //just in case
        break;
    }
  }
  return false;
}

/*************************************************************************************************************
                                      Execute Commands
*************************************************************************************************************/
bool ExecuteCommand(char * exCommand) {
    char * ptrToCommandName = strtok(exCommand, delimiters);                    //Get an array of characters up to one of the delimiters specified in CommandLine.h. This is a command   
    
    /*REPLACE THESE WITH CASE STATEMENTS SO ITS NOT SO ANNOYING TO MAINTAIN*/
    if(!automaticControl){
      if(strcmp(ptrToCommandName, DownshiftCommandToken) == 0){             //If the command is equal to the move sheave forward command
        float distance = readFloat();                                                 //Read in the number that is expected to be after the command. May want to add in some error handling in case there isnt a number after the command.
        float rate = readFloat();
        Downshift(distance, rate);                                           //Move sheave forward the distance that was read in from the command line on the previous line of code
      } else if(strcmp(ptrToCommandName, UpshiftCommandToken) == 0){     //If the command is equal to the move sheave backward command
          float distance = readFloat();                                               //Read in the number that is expected to be after the command. May want to add in some error handling in case there isnt a number after the command.
          float rate = readFloat();
          Upshift(distance, rate);                                        //Move sheave backward the distance that was read in from the command line on the previous line of code
      } else if(strcmp(ptrToCommandName, enableBrakeCommandToken) == 0){
          EnableBrake();
          Serial.println("Sheave brake enabled");
      } else if(strcmp(ptrToCommandName, disableBrakeCommandToken) == 0){
          DisableBrake();
          Serial.println("Sheave brake disabled");
      } else if(strcmp(ptrToCommandName, setDirectionCommandToken) == 0){
          bool direction = readInt();
          SetDirection(direction);
          Serial.println("Direction reversed");
      }else {
            nullCommand(ptrToCommandName);
      }
    } 
    
    else {  
      if(strcmp(ptrToCommandName, automaticControlCommandToken) == 0){
          automaticControl = true;
          Serial.println("Automatic control enabled. Manual control will not work until it is re-enabled.");
      } else if(strcmp(ptrToCommandName, manualControlCommandToken) == 0){
          automaticControl = false;
          Serial.println("Automatic control enabled. Manual control will not work until it is re-enabled.");
      }else {
            nullCommand(ptrToCommandName);
      }
    }
  }

/*************************************************************************************************************
                                       Read functions for the command line
*************************************************************************************************************/
int readInt(void) {
  char * numTextPtr = strtok(NULL, delimiters);         
  return atoi(numTextPtr);                              
}

char * readWord(void) {
  char * word = strtok(NULL, delimiters);               
  return word;
}

float readFloat(void){
  char * floatTextPtr = strtok(NULL, delimiters);
  return atof(floatTextPtr);
}

void nullCommand(char * ptrToCommandName) {
  Serial.print("Command not found: ");     
  Serial.print(ptrToCommandName);
}
