#include <iostream>
#include "Definitions.h"
#include "EPOS_Control.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>

using namespace std;

void Motor_Init(){
	int lResult = MMC_FAILED;
	unsigned int ulErrorCode = 0;

	SetDefaultParameters();

	PrintSettings();
	
	if((lResult = OpenDevice(&ulErrorCode))!=MMC_SUCCESS){
		LogError("OpenDevice", lResult, ulErrorCode);
		//return lResult;
	}
	
	if((lResult = PrepareDemo(&ulErrorCode))!=MMC_SUCCESS){
		LogError("PrepareDemo", lResult, ulErrorCode);
		//return lResult;
	}
	
}

int Get_Position(){
	unsigned int pErrorCode = false;
	unsigned int lErrorCode = false;
	int posEpos = 0;
	
	VCS_GetPositionIs(g_pKeyHandle, g_usNodeId, &posEpos, &pErrorCode);
        return posEpos;	
	
}

void Motor_End(){
	int lResult = MMC_FAILED;
	unsigned int ulErrorCode = 0;
	unsigned int lErrorCode = false;
	
	if((lResult = CloseDevice(&ulErrorCode))!=MMC_SUCCESS){
		LogError("CloseDevice", lResult, ulErrorCode);
		//return lResult;
	}	
	VCS_SetDisableState(g_pKeyHandle, g_usNodeId, &lErrorCode);//desativar controle
}

void Speed_Control(int speed){
	unsigned int pErrorCode = false;
	//VCS_ActivateProfileVelocityMode(g_pKeyHandle, g_usNodeId, &pErrorCode);//colocar só uma vez no começo
	VCS_MoveWithVelocity(g_pKeyHandle, g_usNodeId, -speed, &pErrorCode); // Velocidade em rpm;
	//VCS_HaltVelocityMovement(g_pKeyHandle, g_usNodeId, &pErrorCode); // Stop motor

}
bool ChecarSensor(){
    // Checar se uma bola foi inserida
    return true;
}

int main()
{
	unsigned int pErrorCode = false;
	unsigned int lErrorCode = false;
	
	// Inicializar Motor
	Motor_Init();
	VCS_ActivateProfileVelocityMode(g_pKeyHandle, g_usNodeId, &pErrorCode);
	
	while(TRUE){
	    
	    // Caso detectar uma bola
	    if ( ChecarSensor() ){
	        // Lança a bola
        	Speed_Control(round(10));
        	sleep(3);
        	
        	// Parar o motor
        	VCS_HaltVelocityMovement(g_pKeyHandle, g_usNodeId, &pErrorCode);
    	}
	}

	VCS_HaltVelocityMovement(g_pKeyHandle, g_usNodeId, &pErrorCode);
	VCS_SetDisableState(g_pKeyHandle, g_usNodeId, &lErrorCode);//dar disable no controle da epos
	Motor_End();
	return 0;
}
