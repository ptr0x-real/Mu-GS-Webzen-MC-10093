//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # DARK PLUGIN - POWERED BY FIRE TEAM
// # GAME SERVER: 99.60T (C) WEBZEN.
// # VERSÃO: 1.0.0.0
// # Autor: Maykon
// # Skype: Maykon.ale
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # O Senhor é meu pastor e nada me faltará!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

class Functions
{
public:

	bool VerificarGM(int aIndex);
	int Get_PlayerIndex(char *Name);
	void FireWork(int aIndex);
	static BOOL HookgObjAttack(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo);
	static void gObjSecondProcEx();
	static void gObjCloseSetEx(int aIndex, int flag);
	void OutWhiteMSG(int aIndex, const char* Format, ...);
	void GreenChatSend(LPCSTR Nome, LPSTR Mensagem, DWORD aIndex);
};

void BuxConvert(char* buf, int size);

extern Functions func;