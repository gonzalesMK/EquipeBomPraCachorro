#include <iostream>
#include <chrono>
#include "Definitions.h"
#include "EPOS_Control.h"
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>

const WORD maxStrSize = 100;
char* deviceName = "EPOS2";
char* protocol = "CANopen";
char* namesel[100];
bool endofsel;
int errorcode = 0;

if(VCS_GetInterfaceNameSelection(deviceName,protocol,true,namesel,100,&endofsel,&errorcode)){
    while(!endofsel){
        VCS_GetInterfaceNameSelection(deviceName,protocol,false,namesel,100,&endofsel,&errorcode);
    }
}
