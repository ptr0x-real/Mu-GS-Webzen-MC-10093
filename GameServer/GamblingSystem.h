// GamblingSystem.h: interface for the CGamblingItemBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMBLINGSYSTEM_H__32FE5097_1ED4_421E_AD70_E598D3CD6EEE__INCLUDED_)
#define AFX_GAMBLINGSYSTEM_H__32FE5097_1ED4_421E_AD70_E598D3CD6EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemBagEx.h"

// 추가 옵션 배열 인덱스로 사용하기 위해 enum 정의
enum {
	ADD_OPTION_PLUS_4 = 0,
	ADD_OPTION_PLUS_8 = 1,
	ADD_OPTION_PLUS_12 = 2,
	ADD_OPTION_PLUS_16 = 3,

	ADD_OPTION_COUNT,
};

// 겜블링에서 구매하는 아이템의 그룹 번호 enum 정의
enum {
	GAMBILNG_ITEM_TYPE_SWORD	= 0,
	GAMBILNG_ITEM_TYPE_STAFF	= 1,
	GAMBILNG_ITEM_TYPE_BOW		= 2,
	GAMBILNG_ITEM_TYPE_SCEPTER	= 3,
	GAMBILNG_ITEM_TYPE_STICK	= 4,

	GAMBILNG_ITEM_TYPE_COUNT,
};

// 겜블링 아이템에 옵션이 여러개 붙을 확률
enum {
	ATTACH_1_OPTION_RATE = 0,		// 옵션이 1개 붙을 확률 인덱스
	ATTACH_2_OPTION_RATE = 1,		// 옵션이 2개 붙을 확률 인덱스
	ATTACH_3_OPTION_RATE = 2,		// 옵션이 3개 붙을 확률 인덱스
	ATTACH_4_OPTION_RATE = 3,		// 옵션이 4개 붙을 확률 인덱스

	ATTACH_OPTION_COUNT,
};

// 각 엑설런트 옵션이 붙을 확률 인덱스
enum {
	EX_1_OPTION_RATE = 0,		// 첫번째 엑설런트 옵션이 붙을 확률
	EX_2_OPTION_RATE = 1,		// 두번째 엑설런트 옵션이 붙을 확률
	EX_3_OPTION_RATE = 2,		// 세번째 엑설런트 옵션이 붙을 확률
	EX_4_OPTION_RATE = 3,		// 네번째 엑설런트 옵션이 붙을 확률
	EX_5_OPTION_RATE = 4,		// 다번째 엑설런트 옵션이 붙을 확률
	EX_6_OPTION_RATE = 5,		// 여섯번째 엑설런트 옵션이 붙을 확률
		
	EX_OPTION_COUNT,
};

// 스크립트에 들어오는 겜블링 아이템의 그룹 번호
enum {
	// 일반 무기 그룹
	ITEM_GROUP_NORMAL_SWORD_1	= 0,
	ITEM_GROUP_NORMAL_SWORD_2	= 1,
	ITEM_GROUP_NORMAL_SWORD_3	= 2,
	ITEM_GROUP_NORMAL_SWORD_4	= 3,
	ITEM_GROUP_NORMAL_SWORD_5	= 4,
	ITEM_GROUP_NORMAL_STAFF_1	= 5,
	ITEM_GROUP_NORMAL_STAFF_2	= 6,
	ITEM_GROUP_NORMAL_STAFF_3	= 7,
	ITEM_GROUP_NORMAL_STAFF_4	= 8,
	ITEM_GROUP_NORMAL_STAFF_5	= 9,
	ITEM_GROUP_NORMAL_BOW_1		= 10,
	ITEM_GROUP_NORMAL_BOW_2		= 11,
	ITEM_GROUP_NORMAL_BOW_3		= 12,
	ITEM_GROUP_NORMAL_BOW_4		= 13,
	ITEM_GROUP_NORMAL_BOW_5		= 14,
	ITEM_GROUP_NORMAL_SCEPTER_1	= 15,
	ITEM_GROUP_NORMAL_SCEPTER_2	= 16,
	ITEM_GROUP_NORMAL_SCEPTER_3	= 17,
	ITEM_GROUP_NORMAL_SCEPTER_4	= 18,
	ITEM_GROUP_NORMAL_SCEPTER_5	= 19,
	ITEM_GROUP_NORMAL_STICK_1	= 20,
	ITEM_GROUP_NORMAL_STICK_2	= 21,
	ITEM_GROUP_NORMAL_STICK_3	= 22,
	ITEM_GROUP_NORMAL_STICK_4	= 23,
	ITEM_GROUP_NORMAL_STICK_5	= 24,

	ITEM_GROUP_MAX,
};

typedef struct _GAMBLING_RATE
{
	int		iSkillRate;		// 각 아이템 그룹별 스킬 붙을 확률
	int		iLuckRate;		// 각 아이템 그룹별 행운 붙을 확률
	int		iAddOptionRate;	// 각 아이템 그룹별 추가 옵션 붙을 확률
	int		iExcellentRate;	// 각 아이템 그룹별 엑설런트 아이템일 확률
	int		iSelectedRate;	// 각 아이템 그룹별 선택되어질 확률

	_GAMBLING_RATE()
	{
		iSkillRate = 0;
		iLuckRate = 0;
		iAddOptionRate = 0;
		iExcellentRate = 0;
		iSelectedRate = 0;
	}
} GAMBLING_RATE, *LPGAMBLING_RATE;

class CGamblingItemBag
{
public:
	CGamblingItemBag();
	virtual ~CGamblingItemBag();

private:
	BOOL	m_bLoad;
	
	GAMBLING_RATE m_stGamblingRate[ITEM_GROUP_MAX];			// 확률 관련 구조체 변수
	int		m_iAddOption[ADD_OPTION_COUNT];					// 추가 옵션이 붙을때 각 추가 옵션별 붙을 확률
	int		m_iGamblingItemGetRate[GAMBILNG_ITEM_TYPE_COUNT];	// 각 타입별 겜블링 아이템이 나올 확률
	int		m_iGamblingOptionCountRate[ATTACH_OPTION_COUNT];	// 겜블링 아이템에 옵션이 여러개 붙을 확률
	int		m_iGamblingExOptionRate[EX_OPTION_COUNT];		// 겜블링 아이템에 각 엑설런트 옵션이 붙을 확률
	int		m_iNormalExOptionRate[EX_OPTION_COUNT];			// 보통 엑설런트 아이템에 각 엑설런트 옵션이 붙을 확률

	int		m_iBagObjectCount;							// 아이템 리스트 수
	int		m_iDropRatePerItemCount[ITEM_GROUP_MAX];	// 확률당 아이템 수
	int		m_iRateKindCount;							// 확률 종류 개수

	CItemBagAttrEx		BagObject[MAX_ITEMBAGEX];		// 아이템 리스트

private:
	int		GetBagCount(){return m_iBagObjectCount;}
	
	BYTE	GetLevel(int n);
	BYTE	IsSkill(int n);
	BYTE	IsLuck(int n);
	BYTE	IsOption(int n);

public:
	void	GamblingItemBagInit(char * name);
	void	LoadItem(char *script_file);

	BYTE	GetGamblingSystemItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0, BYTE btItemType = 0 );

	BYTE	GamblingOptionRand( int iOptionCount );
	BYTE	ExcellentOptionRand();
};

#endif // !defined(AFX_GAMBLINGSYSTEM_H__32FE5097_1ED4_421E_AD70_E598D3CD6EEE__INCLUDED_)
