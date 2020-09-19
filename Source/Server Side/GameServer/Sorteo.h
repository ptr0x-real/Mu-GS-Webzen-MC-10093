#pragma once
struct SORTEO
{
int Received[20]; 

};

struct SORTE_PREMIO
	{
	DWORD Type;
	DWORD Index;
	DWORD ItemLevel;
	DWORD ItemDur;
	DWORD ItemSkill;
	DWORD ItemLuck;
	DWORD ItemOpt;
	DWORD ItemExc;
	};

class SORTEO_CUSTOM
{
public:
	
	static void Sorteo(void * lpParam);
	int Received[20]; 

	int elSORTEO;
	int Sorteo_Start_Time;

	DWORD SectionCount;
};

extern SORTEO_CUSTOM eSORTEO;
extern SORTE_PREMIO aSORTEO[1000];