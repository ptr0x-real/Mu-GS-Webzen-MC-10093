// StatMng.cpp: implementation of the CStatMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatMng gStatMng;

CStatMng::CStatMng()
{
	Init();
}

CStatMng::~CStatMng()
{

}

void CStatMng::Init()
{	
	int firstState			= 2;
	int MagumfirstState		= 2;
#ifdef MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712
	int iDarkLordFirstState = 2;
#endif

	// 일반 공식: ((10~400)*2)/400+2  (레벨 9까지는 스탯 생성 불가능)
	// 마검 공식: ((10~400)*2)/700+2  (레벨 9까지는 스탯 생성 불가능)	
	// 다크로드 공식: ((10~400)*2)/500+2  (레벨 9까지는 스탯 생성 불가능)
	// #ifdef MODIFY_MINUS_FRUIT_POINT_CALC_20080627
	// 마이너스 스텟 열매 공식 변경
	// 감소 일반 공식: ((레벨)*3)/400+2 / 2 (레벨 9까지는 스탯 생성 불가능)
	// 감소 마검 공식: ((레벨)*3)/700+2 / 2 (레벨 9까지는 스탯 생성 불가능)	
	// 감소 다크로드 공식: ((레벨)*3)/500+2 / 2 (레벨 9까지는 스탯 생성 불가능)
	// #endif // MODIFY_MINUS_FRUIT_POINT_CALC_20080627
	
#if TESTSERVER == 1
//	char szTemp[255];
//	wsprintf( szTemp, "\n\n★ 열매 생성 포인트 ★\n\n");
// 	OutputDebugString(szTemp);
#endif

	for( int i = 0; i < MAX_LEVEL+100; i++ )
	{	
		if( (i+1)%10 == 0 )
		{
#ifdef MODIFY_MINUS_FRUIT_POINT_CALC_20080627
			firstState			+= ( ((i+11)*3)/400+2 );
			MagumfirstState		+= ( ((i+11)*3)/700+2 );
			iDarkLordFirstState += ( ((i+11)*3)/500+2 );
#else
			firstState			+= ((i+11)*2)/400+2;
			MagumfirstState		+= ((i+11)*2)/700+2;
	#ifdef MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712
			iDarkLordFirstState += ((i+11)*2)/500+2;
	#endif
#endif // MODIFY_MINUS_FRUIT_POINT_CALC_20080627

#if TESTSERVER == 1
//			wsprintf( szTemp, "[Lv%03d] %d\t%d\t%d\n", i+1, firstState, MagumfirstState, iDarkLordFirstState );
// 			OutputDebugString(szTemp);
#endif
		}

		gObjNormalAddStatTable[i]	= firstState;
		gObjMagumAddStatTable[i]	= MagumfirstState;	
#ifdef MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712
		gObjDarkLordAddStatTable[i] = iDarkLordFirstState;
#endif
	}

#if TESTSERVER == 1
//	wsprintf( szTemp, "\n\n");
// 	OutputDebugString(szTemp);
#endif


#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	// 감소 일반 공식: ((10~400)*2)/400+2 / 2 (레벨 9까지는 스탯 생성 불가능)
	// 감소 마검 공식: ((10~400)*2)/700+2 / 2 (레벨 9까지는 스탯 생성 불가능)	
	// 감소 다크로드 공식: ((10~400)*2)/500+2 / 2 (레벨 9까지는 스탯 생성 불가능)
	// #ifdef MODIFY_MINUS_FRUIT_POINT_CALC_20080627
	// 마이너스 스텟 열매 공식 변경
	// 감소 일반 공식: ((레벨)*3)/400+2 / 2 (레벨 9까지는 스탯 생성 불가능)
	// 감소 마검 공식: ((레벨)*3)/700+2 / 2 (레벨 9까지는 스탯 생성 불가능)	
	// 감소 다크로드 공식: ((레벨)*3)/500+2 / 2 (레벨 9까지는 스탯 생성 불가능)
	// #endif // MODIFY_MINUS_FRUIT_POINT_CALC_20080627


	// * 누적됨.

	firstState			= 2;
	MagumfirstState		= 2;
	iDarkLordFirstState = 2;

	for( int i = 0; i < MAX_LEVEL+100; i++ )
	{
		if( (i+1)%10 == 0 )
		{
#ifdef MODIFY_MINUS_FRUIT_POINT_CALC_20080627
			firstState			+= ( ((i+11)*3)/400+2 );
			MagumfirstState		+= ( ((i+11)*3)/700+2 );
			iDarkLordFirstState += ( ((i+11)*3)/500+2 );
#else
			firstState			+= ( ((i+11)*2)/400+2 );
			MagumfirstState		+= ( ((i+11)*2)/700+2 );
			iDarkLordFirstState += ( ((i+11)*2)/500+2 );
#endif // MODIFY_MINUS_FRUIT_POINT_CALC_20080627
		}

		gObjNormalMinusStatTable[i]		= firstState;
		gObjMagumMinusStatTable[i]		= MagumfirstState;	
		gObjDarkLordMinusStatTable[i]	= iDarkLordFirstState;
	}
#endif


}

int CStatMng::GetMaxStat(int level, int Class)
{
	if( level < 1 || level > g_GlobalConfig.m_iMaxLevel )
		return 0;

#ifdef DARKLORD_WORK
#ifdef MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712
	if( Class == CLASS_MAGUMSA)
#else
	if( Class == CLASS_MAGUMSA || Class == CLASS_DARKLORD )
#endif // ## MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712
#else
	if( Class == CLASS_MAGUMSA )
#endif // ## DARKLORD_WORK
	{
		return gObjMagumAddStatTable[level-1];
	}
#ifdef MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712
	else if( Class == CLASS_DARKLORD )
	{
		return gObjDarkLordAddStatTable[level-1];
	}
#endif // ## MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712

	else
	{
		return gObjNormalAddStatTable[level-1];
	}
}

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
int CStatMng::GetMaxMinusStat( int iLevel, int iClass )
{
	if( iLevel < 1 || iLevel > g_GlobalConfig.m_iMaxLevel )
		return 0;

	if( iClass == CLASS_MAGUMSA )
	{
		return gObjMagumMinusStatTable[iLevel-1];
	}
	else if( iClass == CLASS_DARKLORD )
	{
		return gObjDarkLordMinusStatTable[iLevel-1];
	}
	else
	{
		return gObjNormalMinusStatTable[iLevel-1];
	}

}
#endif