// StatMng.h: interface for the CStatMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATMNG_H__5C97AF32_CD10_49D8_B358_1203FF92C820__INCLUDED_)
#define AFX_STATMNG_H__5C97AF32_CD10_49D8_B358_1203FF92C820__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStatMng  
{
	int		gObjNormalAddStatTable[MAX_LEVEL+100];	// 일반 스탯 상승 가능 포인트 표
	int		gObjMagumAddStatTable[MAX_LEVEL+100];	// 마검사 스탯 상승가능 포인트 표

#ifdef MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712
	int		gObjDarkLordAddStatTable[MAX_LEVEL+100];	// 다크로드 스탯 상승가능 포인트 표
#endif


#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	int		gObjNormalMinusStatTable[MAX_LEVEL+100];	// 일반 스탯 감소 가능 포인트 표
	int		gObjMagumMinusStatTable[MAX_LEVEL+100];		// 마검사 스탯 감소 가능 포인트 표
	int		gObjDarkLordMinusStatTable[MAX_LEVEL+100];	// 다크로드 스탯 감소 가능 포인트 표
#endif

public:
	CStatMng();
	virtual ~CStatMng();

	void	Init();
	int		GetMaxStat(int level, int Class);

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	int		GetMaxMinusStat( int iLevel, int iClass );
#endif

};

extern CStatMng gStatMng;

#endif // !defined(AFX_STATMNG_H__5C97AF32_CD10_49D8_B358_1203FF92C820__INCLUDED_)
