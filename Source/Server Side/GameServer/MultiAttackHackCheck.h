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
	short	number;				// Ÿ���� ���� ���� ��ȣ	
	BYTE	Serial[MAHC_MAX_SERIAL];		// �ø����ȣ
} MULTIATTACK_HACKCHECK;		// ���߰��� �� üũ�� ����


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
	// ���� ��ų üũ
	BOOL	CheckPenetrationSkill(int aTargetIndex, BYTE skillnum, BYTE serial);
#endif
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	// ���̾ũ�� ��ų üũ
	BOOL	CheckFireScreamSkill( int iTargetIndex, BYTE btSkillnum, BYTE btSerial );
#endif

};

extern CMultiAttackHackCheck	gMultiAttackHackCheck[MAX_OBJECT];

#endif // !defined(AFX_MULTIATTACKHACKCHECK_H__C20A4FD5_30CC_4ABF_B162_3836CDFA658E__INCLUDED_)
