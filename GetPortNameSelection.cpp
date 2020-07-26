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

const int maxsize = 100;
char* devicename = "EPOS2";
char* protocol = "CANopen";
char* interface = "";//n sei
char* port[100];
bool endsel = false;
int error = 0;

if(VCS_GetPortNameSelection(devicename,protocol,interface,true,port,100,&endsel,&error)){

    while(!endsel){

        VCS_GetPortNameSelection(devicename,protocol,interface,false,port,100,&endsel,&error);
    }
}