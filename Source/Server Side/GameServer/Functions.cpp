//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # DARK PLUGIN - POWERED BY FIRE TEAM
// # GAME SERVER: 99.60T (C) WEBZEN.
// # VERSÃO: 1.0.0.0
// # Autor: Maykon
// # Skype: Maykon.ale
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # O Senhor é meu pastor e nada me faltará!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "StdAfx.h"

Functions func;

/*
void Functions::StringSendAll(char*Text, int type)
{
	for(int i=OBJECT_MIN;i<OBJECT_MAX;i++)
	{
		if(gObj[i].Connected == 3)
		{
			GCServerMsgStringSend(Text,i,type);
		}
	}
}

void Functions::AllServerAnnounce(char *message, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, message);
	vsprintf(&szBuffer[0], message, pArguments);
	va_end(pArguments);

	for(int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if(gObj[i].Connected == 3)
		{
			GCServerMsgStringSend(&szBuffer[0], i, 0);
		}
	}
}

void Functions::MsgOutputAll(char *message, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, message);
	vsprintf(&szBuffer[0], message, pArguments);
	va_end(pArguments);

	for(int i = OBJECT_MIN; i < OBJECT_MAX; i++)
	{
		if(gObj[i].Connected == 3)
		{
			GCServerMsgStringSend(&szBuffer[0], i, 1);
		}
	}
}
*/
void Functions::GreenChatSend(LPCSTR Nome, LPSTR Mensagem, DWORD aIndex)
{
	BYTE Buffer[0xFF] = {0};
	BYTE Len=strlen(Mensagem);
	Buffer[0] = 0xC1;
	Buffer[1] = Len + 15;
	Buffer[2] = 0x00;
	Buffer[13] = 0x40;
	strncpy((char*)&Buffer[3],Nome,10);
	strncpy((char*)&Buffer[14],Mensagem,Len);

	for (int i = OBJ_STARTUSERINDEX; i < MAX_OBJECT; ++i)
	{
		if (gObj[i].Connected == PLAYER_PLAYING)
		{
			DataSend(i, (unsigned char*)Buffer, Buffer[1]);
		}
	} 
}
void Functions::OutWhiteMSG(int aIndex, const char* Format, ...) 
{
		char Message[1024];
	va_list pArguments;
	va_start(pArguments, Format);
	vsprintf(Message, Format, pArguments);
	va_end(pArguments);
	BYTE *Packet;
	Packet = (BYTE*) malloc(200);
	memset(Packet, 0x00, 200);
	*Packet = 0xC1;
	*(Packet+2) = 0x00;
	memcpy((Packet+13), Message, strlen(Message));
	int Len = (strlen(Message) + 0x13);
	*(Packet+1) = Len;
	DataSend(aIndex, Packet, Len);
	free (Packet);
}

int Functions::Get_PlayerIndex(char *Name)
{    
	for(int i=OBJECT_MIN;i<MAX_OBJECT;i++)
	{     

		if (gObj[i].Connected > 2)
		{
			if (!strcmp(gObj[i].Name, Name))
			{
				return i;
			}
		}
	}
	return -1;
}


bool Functions::VerificarGM(int aIndex)
{
	unsigned char x = gObj[aIndex].Authority;
	if(x == 8 || x == 10 || x == 24 || x == 32)
		return TRUE;
	else
		return FALSE;
}

BOOL Functions::HookgObjAttack(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo)
{
	if(lpTargetObj->Authority > 1)
	{
		return FALSE;
	}

	if(lpTargetObj->MapNumber == 1 && gs_cmd.Sobreblock == 1)
	{
		return FALSE;
	}
				
	return gObjAttack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo);
}



void Functions::gObjCloseSetEx(int aIndex, int flag)
{
	switch(flag)
	{
		 case 1: 
		  {
			 /* if(Mata[aIndex].EnterMataMata == true)
			  {
				   Mata[aIndex].EnterMataMata = false;
			  }*/

			  if(SbPlayer[aIndex].inSobreEvent == 1)
			  {
				  gs_cmd.Contplayer--;
				  SbPlayer[aIndex].inSobreEvent = 0;
				  SbPlayer[aIndex].pWaiting = false;
			  }
		  }
		  break;
	}
	gObjCloseSet(aIndex, flag);  
}
