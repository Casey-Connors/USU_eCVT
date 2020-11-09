#include "TCU.h"
#include <MotorController.h>

void ShiftUpdate(){
    ComputeMotorOutput();
    Shift();
}

