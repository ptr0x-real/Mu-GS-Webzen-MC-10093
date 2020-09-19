#include "StdAfx.h"
#include "Licencia.h"
#include "GameServer.h"

LICENSE pLicense;

void ReadyLicense()
{
	MACProtection();
}

bool CheckingMAC(LPCSTR MacID)
{
	char ClientsMacs[13][18] =
	{
		"70-71-BC-7B-DE-A0",
		"FE-7E-5F-74-31-B1",
		"00-50-56-0A-50-D4",
		"00-50-56-06-48-01",
	};

	for(int x=0; x<13; x++)
	{
		if(!strcmp(MacID,ClientsMacs[x]))
		{
			return true;
		}
	}
	return false;
}

bool MACProtection()
{
	unsigned char MACData[6];

	UUID uuid;
	UuidCreateSequential( &uuid );

	for (int i=2; i<8; i++)
	{
		MACData[i - 2] = uuid.Data4[i];
	}

	char MACADDR[18];
	sprintf(MACADDR, "%02X-%02X-%02X-%02X-%02X-%02X", MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);

	if(CheckingMAC(MACADDR))
	{
		return true;
	}

	else
	{
		char Nose[255];
		sprintf(Nose, "MC LICENCIA INVALIDA , enviar esta serie: (%02X-%02X-%02X-%02X-%02X-%02X)", MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
		MessageBoxA(NULL, Nose, "LICENSE MIRAGE CONTINENT V9.0",MB_OK);
	    ExitProcess(0);
	}
	return false;
}

