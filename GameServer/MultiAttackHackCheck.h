// MultiAttackHackCheck.h: interface for the CMultiAttackHackCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIATTACKHACKCHECK_H__C20A4FD5_30CC_4ABF_B162_3836CDFA658E__INCLUDED_)
#define AFX_MULTIATTACKHACKCHECK_H__C20A4FD5_30CC_4ABF_B162_3836CDFA658E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAHC_MAX_SERIAL			255
#define MAHC_MAX_CHECKTABLE		40

typedef struct
{
	short	number;				// 타격을 받은 유저 번호	
	BYTE	Serial[MAHC_MAX_SERIAL];		// 시리얼번호
} MULTIATTACK_HACKCHECK;		// 다중공격 핵 체크를 위해


class CMultiAttackHackCheck  
{
	int						m_iCount;
	MULTIATTACK_HACKCHECK	m_Table[MAHC_MAX_CHECKTABLE];
public:
	CMultiAttackHackCheck();
	virtual ~CMultiAttackHackCheck();

	void	Init();

	BOOL	Insert(int aTargetIndex, BYTE skillnum, BYTE serial);

#ifdef CHECK_PENETRATION_SKILL
	// 관통 스킬 체크
	BOOL	CheckPenetrationSkill(int aTargetIndex, BYTE skillnum, BYTE serial);
#endif
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	// 파이어스크림 스킬 체크
	BOOL	CheckFireScreamSkill( int iTargetIndex, BYTE btSkillnum, BYTE btSerial );
#endif

};

extern CMultiAttackHackCheck	gMultiAttackHackCheck[MAX_OBJECT];

#endif // !defined(AFX_MULTIATTACKHACKCHECK_H__C20A4FD5_30CC_4ABF_B162_3836CDFA658E__INCLUDED_)
