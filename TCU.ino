#include <TCU.h>
#include <MotorController.h>
#include <CommandLine.h>

char CommandLine[COMMAND_BUFFER_LENGTH + 1];

void setup() {
  InitializeController();
}

void loop() {
  bool received = getCommandLineFromSerialPort(CommandLine);         //gets a command from the command line if their is one. Should only run this when debugging. Maybe do a macro cut thing.                  
  
  if(received){                                                      //If there is a command stored in the command buffer.
    ExecuteCommand(CommandLine);                                        //Execute command
  }

}
