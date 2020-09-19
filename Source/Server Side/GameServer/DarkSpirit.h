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
		PetItem_Mode_Normal,				// 기본 
		PetItem_Mode_Attack_Random,			// 랜덤하게 한마리를 공격
		PetItem_Mode_Attack_WithMaster,		// 캐릭터가 공격하는 몬스터를 같이 공격
		PetItem_Mode_Attack_Target,			// 사용자가 지정해준 몬스터를 집중 공격
	};
private:
	int		m_AttackDamageMin;	// 최소 공격력
	int		m_AttackDamageMax;	// 최대 공격력

	int		m_AttackSpeed;

	int		m_SuccessAttackRate;	// 공격 성공율		

	int		m_iMasterIndex;					// 주인의 인덱스
	int		m_iTargetIndex;					// 타켓의 인덱스

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

	// 공격 타켓 설정
	void	SetTarget(int aTargetIndex);
	void	ReSetTarget(int aTargetIndex);

	// 펫 아이템 정보 설정
	void	Set(int aIndex, CItem* pPetItem);

	//다크 스피릿 모드 설정
	void	SetMode(ePetItem_Mode mode, int iTargetindex = -1 );

	// 다크 스피릿 공격

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

	// 미스체크는 다크 스피릿용으로 따로 처리 한다
	BOOL MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int& bAllMiss );
#ifdef ADD_SHIELD_POINT_01_20060403
	BOOL MissCheckPvP(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int& bAllMiss );
#endif

	// 다크로드 명령 전환
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
