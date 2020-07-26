// Filtrado

#include <iostream>
#include "Definitions.h"
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>

typedef void* HANDLE;
typedef int BOOL;

// variavel criada
enum EAppMode
{
	AM_UNKNOWN,
	AM_DEMO,
	AM_INTERFACE_LIST,
	AM_PROTOCOL_LIST,
	AM_VERSION_INFO
};

using namespace std;

//variaveis

void* g_pKeyHandle = 0;
unsigned short g_usNodeId = 1;
string g_deviceName;
string g_protocolStackName;
string g_interfaceName;
string g_portName;
int g_baudrate = 0;
EAppMode g_eAppMode = AM_DEMO;

const string g_programName = "HelloEposCmd";

//definiçoes
#ifndef MMC_SUCCESS
	#define MMC_SUCCESS 0
#endif

#ifndef MMC_FAILED
	#define MMC_FAILED 1
#endif

#ifndef MMC_MAX_LOG_MSG_SIZE
	#define MMC_MAX_LOG_MSG_SIZE 512
#endif

//todas as fçs
void  LogError(string functionName, int p_lResult, unsigned int p_ulErrorCode);
void  LogInfo(string message);
//void  PrintUsage();
//void  PrintHeader(); 
void  PrintSettings(); //printa id nome protocolo interface porta e baud rate
int   OpenDevice(unsigned int* p_pErrorCode);
int   CloseDevice(unsigned int* p_pErrorCode);
void  SetDefaultParameters();
//int   ParseArguments(int argc, char** argv);
int   DemoProfilePositionMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int   Demo(unsigned int* p_pErrorCode);
int   PrepareDemo(unsigned int* p_pErrorCode);
//int   PrintAvailableInterfaces();
//int	PrintAvailablePorts(char* p_pInterfaceNameSel);
//int   PrintAvailableProtocols();
//int   PrintDeviceVersion();

void SeparatorLine()
{
	const int lineLength = 65;
	for(int i=0; i<lineLength; i++)
	{
		cout << "-";
	}
	cout << endl;
}

void LogError(string functionName, int p_lResult, unsigned int p_ulErrorCode)
{
	cerr << g_programName << ": " << functionName << " failed (result=" << p_lResult << ", errorCode=0x" << std::hex << p_ulErrorCode << ")"<< endl;
}

void LogInfo(string message)
{
	cout << message << endl;
}

void PrintSettings()
{
	stringstream msg;

	msg << "default settings:" << endl;
	msg << "node id             = " << g_usNodeId << endl;
	msg << "device name         = '" << g_deviceName << "'" << endl;
	msg << "protocal stack name = '" << g_protocolStackName << "'" << endl;
	msg << "interface name      = '" << g_interfaceName << "'" << endl;
	msg << "port name           = '" << g_portName << "'"<< endl;
	msg << "baudrate            = " << g_baudrate;

	LogInfo(msg.str());

	SeparatorLine();
}

void SetDefaultParameters()
{
	//USB
	g_usNodeId = 1;
	g_deviceName = "EPOS2"; 
	g_protocolStackName = "MAXON SERIAL V2"; 
	g_interfaceName = "USB"; 
	g_portName = "USB0"; 
	g_baudrate = 1000000; 
}

int OpenDevice(unsigned int* p_pErrorCode)
{
	int lResult = MMC_FAILED;

	char* pDeviceName = new char[255];
	char* pProtocolStackName = new char[255];
	char* pInterfaceName = new char[255];
	char* pPortName = new char[255];

	strcpy(pDeviceName, g_deviceName.c_str());
	strcpy(pProtocolStackName, g_protocolStackName.c_str());
	strcpy(pInterfaceName, g_interfaceName.c_str());
	strcpy(pPortName, g_portName.c_str());

	LogInfo("Open device...");

	g_pKeyHandle = VCS_OpenDevice(pDeviceName, pProtocolStackName, pInterfaceName, pPortName, p_pErrorCode);

	if(g_pKeyHandle!=0 && *p_pErrorCode == 0)
	{
		unsigned int lBaudrate = 0;
		unsigned int lTimeout = 0;

		if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
		{
			if(VCS_SetProtocolStackSettings(g_pKeyHandle, g_baudrate, lTimeout, p_pErrorCode)!=0)
			{
				if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
				{
					if(g_baudrate==(int)lBaudrate)
					{
						lResult = MMC_SUCCESS;
					}
				}
			}
		}
	}
	else
	{
		g_pKeyHandle = 0;
	}

	delete []pDeviceName;
	delete []pProtocolStackName;
	delete []pInterfaceName;
	delete []pPortName;

	return lResult;
}

int CloseDevice(unsigned int* p_pErrorCode)
{
	int lResult = MMC_FAILED;

	*p_pErrorCode = 0;

	LogInfo("Close device");

	if(VCS_CloseDevice(g_pKeyHandle, p_pErrorCode)!=0 && *p_pErrorCode == 0)
	{
		lResult = MMC_SUCCESS;
	}

	return lResult;
}


int DemoProfilePositionMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;

	msg << "set profile position mode, node = " << p_usNodeId;
	LogInfo(msg.str());

	if(VCS_ActivateProfilePositionMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
	{
		LogError("VCS_ActivateProfilePositionMode", lResult, p_rlErrorCode);
		lResult = MMC_FAILED;
	}
	else
	{
		list<long> positionList;

		positionList.push_back(5000);
		positionList.push_back(-10000);
		positionList.push_back(5000);

		for(list<long>::iterator it = positionList.begin(); it !=positionList.end(); it++)
		{
			long targetPosition = (*it);
			stringstream msg;
			msg << "move to position = " << targetPosition << ", node = " << p_usNodeId;
			LogInfo(msg.str());

			if(VCS_MoveToPosition(p_DeviceHandle, p_usNodeId, targetPosition, 0, 1, &p_rlErrorCode) == 0)
			{
				LogError("VCS_MoveToPosition", lResult, p_rlErrorCode);
				lResult = MMC_FAILED;
				break;
			}

			sleep(1);
		}

		if(lResult == MMC_SUCCESS)
		{
			LogInfo("halt position movement");

			if(VCS_HaltPositionMovement(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
			{
				LogError("VCS_HaltPositionMovement", lResult, p_rlErrorCode);
				lResult = MMC_FAILED;
			}
		}
	}

	return lResult;
}

bool DemoProfileVelocityMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode){

	VCS_ActivateProfileVelocityMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode);
		
	long targetvelocity = 50;

	VCS_MoveWithVelocity(p_DeviceHandle, p_usNodeId, targetvelocity, &p_rlErrorCode);
	sleep(5);
	VCS_HaltVelocityMovement(p_DeviceHandle, p_usNodeId, &p_rlErrorCode);
			
	return false;
}

int PrepareDemo(unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	BOOL oIsFault = 0;

	if(VCS_GetFaultState(g_pKeyHandle, g_usNodeId, &oIsFault, p_pErrorCode ) == 0)
	{
		LogError("VCS_GetFaultState", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}

	if(lResult==0)
	{
		if(oIsFault)
		{
			stringstream msg;
			msg << "clear fault, node = '" << g_usNodeId << "'";
			LogInfo(msg.str());

			if(VCS_ClearFault(g_pKeyHandle, g_usNodeId, p_pErrorCode) == 0)
			{
				LogError("VCS_ClearFault", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}
		}

		if(lResult==0)
		{
			BOOL oIsEnabled = 0;

			if(VCS_GetEnableState(g_pKeyHandle, g_usNodeId, &oIsEnabled, p_pErrorCode) == 0)
			{
				LogError("VCS_GetEnableState", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}

			if(lResult==0)
			{
				if(!oIsEnabled)
				{
					if(VCS_SetEnableState(g_pKeyHandle, g_usNodeId, p_pErrorCode) == 0)
					{
						LogError("VCS_SetEnableState", lResult, *p_pErrorCode);
						lResult = MMC_FAILED;
					}
				}
			}
		}
	}
	return lResult;
}

int Demo(unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	unsigned int lErrorCode = 0;

	lResult = DemoProfileVelocityMode(g_pKeyHandle, g_usNodeId, lErrorCode);

	if(lResult != MMC_SUCCESS)
	{
		LogError("DemoProfileVelocityMode", lResult, lErrorCode);
	}
	else
	{
		lResult = DemoProfilePositionMode(g_pKeyHandle, g_usNodeId, lErrorCode);

		if(lResult != MMC_SUCCESS)
		{
			LogError("DemoProfilePositionMode", lResult, lErrorCode);
		}
		else
		{
			if(VCS_SetDisableState(g_pKeyHandle, g_usNodeId, &lErrorCode) == 0)
			{
				LogError("VCS_SetDisableState", lResult, lErrorCode);
				lResult = MMC_FAILED;
			}
		}
	}

	return lResult;
}
