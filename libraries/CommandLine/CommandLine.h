#ifndef COMMANDLINE_h
#define COMMANDLINE_h

/*************************************************************************************************************
                                            Command line inputs
*************************************************************************************************************/
#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '
#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer

bool getCommandLineFromSerialPort(char * serialCommand);

bool ExecuteCommand(char * exCommand);

int readInt(void);

char * readWord(void);

float readFloat(void);

void nullCommand(char * ptrToCommandName);

#endif //COMMANDLINE_H