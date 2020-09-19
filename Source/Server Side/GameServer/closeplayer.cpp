// closeplayer.cpp: implementation of the Ccloseplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "closeplayer.h"
#include "user.h"
#include "..\\Include\\ReadScript.h"
#include "wsGameServer.h"
#include "giocp.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CwsGameServer	wsGServer;

Ccloseplayer::Ccloseplayer()
{

}

Ccloseplayer::~Ccloseplayer()
{

}


void Ccloseplayer::Load(char *filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		LogAdd(lMsg.Get(469), filename);
		return ;
	}
	SMDToken Token;
	
	int aIndex=0;
	char closeplayerid[50];

	while( 1 )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		memset(closeplayerid, 0, 50);
		strcpy(closeplayerid, TokenString);
		if( strlen(closeplayerid) > 0 )
		{
			aIndex = gObjGetIndex(closeplayerid);
			if( aIndex >= 0 )
			{
				LogAdd(lMsg.Get(447),closeplayerid);
				//wsGServer.Close(gObj[aIndex].m_socket);
				CloseClient ( aIndex );
			}
		}
	}
	fclose(SMDFile);
}
