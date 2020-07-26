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
#include <chrono>

using namespace std;

int pos_off=0;//Encoder Incremental

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

void Move_to_Pos(long pos){
	unsigned int pErrorCode = false;
	//VCS_ActivateProfileVelocityMode(g_pKeyHandle, g_usNodeId, &pErrorCode);//colocar só uma vez no começo
	VCS_MoveToPosition(g_pKeyHandle, g_usNodeId, pos,0,1, &pErrorCode); // Velocidade em rpm;
	//VCS_HaltVelocityMovement(g_pKeyHandle, g_usNodeId, &pErrorCode); // Stop motor

}

void PID(double kp,double kd,double ki,double to,int pos){
    double static integral;
    int static last_erro;
    int speed;
    double erro = 100;

    while(erro > 0.1 || erro < -0.1){ 
        erro = pos - (Get_Position()-pos_off); 
        integral = erro*to*ki + integral;
        speed = kp*erro + integral + (erro - last_erro)*kd/to; 
        Speed_Control(speed);   //Talvez seja necessário mandar (-speed) //Necessário calcular o valor de to
        last_erro = erro;
    }
    Speed_Control(0);
}


int main()
{
	unsigned int pErrorCode = false;
	unsigned int lErrorCode = false;
    double pos_des = 30;
	//VCS_SetDisableState(g_pKeyHandle, g_usNodeId, &lErrorCode);
	Motor_Init();
	VCS_ActivateProfilePositionMode(g_pKeyHandle, g_usNodeId, &pErrorCode);
	//VCS_ActivateProfileVelocityMode(g_pKeyHandle, g_usNodeId, &pErrorCode);
	pos_off=Get_Position();
    //double pos_des = 30;
	Move_to_Pos(pos_des);
    //PID(1,0,0.01,0.01,pos_des);
	//VCS_HaltVelocityMovement(g_pKeyHandle, g_usNodeId, &pErrorCode);
	VCS_HaltPositionMovement(g_pKeyHandle, g_usNodeId, &pErrorCode);
	VCS_SetDisableState(g_pKeyHandle, g_usNodeId, &lErrorCode);//dar disable no controle da epos
	Motor_End();
	return 0;
}
