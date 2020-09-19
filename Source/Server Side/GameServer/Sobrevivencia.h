#pragma once


struct SOBRE
{
	DWORD ItemType;       
	DWORD ItemNr;       
	DWORD ItemLevel;
	DWORD ItemLuck;
	DWORD ItemSkill;
	DWORD ItemDur;       
	DWORD ItemOpt;       
	DWORD ItemExc;   
};

struct SobrePlayer
{
	BYTE inSobreEvent;
	bool pWaiting;
};

class Sobrevivencia
{
public:
	static bool LoadSettings();
	void ResetEvent();
	void Init(int aIndex, char* msg) ;
	void SobreStart();
	void Ir(int aIndex);
	bool Dead(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* tObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill, int iShieldDamage);

	DWORD SectionCount;

private:
	bool Activated;

	short InitTime;
	short InitSeconds;
};

extern SOBRE SobreDrop[1000];
extern Sobrevivencia Sobre;
extern SobrePlayer SbPlayer[MAX_OBJECT];