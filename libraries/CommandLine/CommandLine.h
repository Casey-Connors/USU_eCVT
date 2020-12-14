#ifndef COMMANDLINE_h
#define COMMANDLINE_h

/*************************************************************************************************************
                                            Command line inputs
*************************************************************************************************************/
constexpr char CR = '\r';
constexpr char LF = '\n';
constexpr char BS = '\b';
constexpr char NULLCHAR = '\0';
constexpr char SPACE = ' ';
#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer

bool getCommandLineFromSerialPort(char * serialCommand);

void ExecuteCommand(char * exCommand);

int readInt(void);

char * readWord(void);

float readFloat(void);

void nullCommand(char * ptrToCommandName);

#endif //COMMANDLINE_H