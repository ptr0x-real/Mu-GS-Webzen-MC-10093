// DarkSpirit.h: interface for the CDarkSpirit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DARKSPIRIT_H__D31A3673_75DB_4204_93C3_9C29D43C209C__INCLUDED_)
#define AFX_DARKSPIRIT_H__D31A3673_75DB_4204_93C3_9C29D43C209C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DARKLORD_WORK
#include "ObjBaseAttack.h"

#define		MAX_DARKSPIRIT_ATTACK_RANGE	5

class CDarkSpirit : public CObjBaseAttack 
{
public:
	enum ePetItem_Mode
	{
		PetItem_Mode_Normal,				// �⺻ 
		PetItem_Mode_Attack_Random,			// �����ϰ� �Ѹ����� ����
		PetItem_Mode_Attack_WithMaster,		// ĳ���Ͱ� �����ϴ� ���͸� ���� ����
		PetItem_Mode_Attack_Target,			// ����ڰ� �������� ���͸� ���� ����
	};
private:
	int		m_AttackDamageMin;	// �ּ� ���ݷ�
	int		m_AttackDamageMax;	// �ִ� ���ݷ�

	int		m_AttackSpeed;

	int		m_SuccessAttackRate;	// ���� ������		

	int		m_iMasterIndex;					// ������ �ε���
	int		m_iTargetIndex;					// Ÿ���� �ε���

	DWORD	m_dwLastAttackTime;

	ePetItem_Mode	m_ActionMode;
	CItem*			m_pPetItem;
public:
	CDarkSpirit();
	virtual ~CDarkSpirit();

	void	Init();
	
	void	Run();	

	void	ModeNormal();
	void	ModeAttackRandom();
	void	ModeAttackWithMaster();
	void	ModeAttakTarget();

	// ���� Ÿ�� ����
	void	SetTarget(int aTargetIndex);
	void	ReSetTarget(int aTargetIndex);

	// �� ������ ���� ����
	void	Set(int aIndex, CItem* pPetItem);

	//��ũ ���Ǹ� ��� ����
	void	SetMode(ePetItem_Mode mode, int iTargetindex = -1 );

	// ��ũ ���Ǹ� ����

#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
	BOOL	Attack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int criticaldamage, int iActionType = 0);
#else
	BOOL	Attack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int criticaldamage);
#endif


#ifdef DUEL_SYSTEM_20031028	
	int  GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BOOL bIsOnDuel, int criticaldamage);		
#else	
	int  GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, int criticaldamage);	
#endif

	// �̽�üũ�� ��ũ ���Ǹ������� ���� ó�� �Ѵ�
	BOOL MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int& bAllMiss );
#ifdef ADD_SHIELD_POINT_01_20060403
	BOOL MissCheckPvP(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int& bAllMiss );
#endif

	// ��ũ�ε� ��� ��ȯ
	void	ChangeCommand(int command, int targetindex);

	void	RangeAttack(int aIndex, int aTargetIndex);

#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
	void	SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage, int iActionType = 0);
#else
	void	SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage);
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	int  GetShieldDamage(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int iAttackDamage);
#endif
	
	static	void	SendLevelmsg(int aIndex, int nPos, int PetType, int InvenType);
};

extern CDarkSpirit	gDarkSpirit[MAX_OBJECT];

#endif

#endif // !defined(AFX_DARKSPIRIT_H__D31A3673_75DB_4204_93C3_9C29D43C209C__INCLUDED_)
