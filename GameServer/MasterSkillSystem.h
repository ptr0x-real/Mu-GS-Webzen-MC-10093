// MasterSkillSystem.h: interface for the CMasterSkillSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERSKILLSYSTEM_H__D1E7613A_BBDC_4296_8186_0043960ABF4B__INCLUDED_)
#define AFX_MASTERSKILLSYSTEM_H__D1E7613A_BBDC_4296_8186_0043960ABF4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122

#include "protocol.h"

// 결과값
enum _RESULT_MSG
{
	RM_ERROR				= 0,	// 에러
	RM_OK					= 1,	// 오~케이!
	RM_INVALID_USER			= 2,	// 이상한 유저
	RM_INVALID_SKILL_NUM	= 3,	// 이상한 스킬 번호	
	RM_INVALID_CONDITION	= 4,	// 스킬을 습득하기 위한 요구조건에 맞지 않음
	RM_ADD_MAGIC_FAIL		= 5,	// 마법 추가 실패
};

const INT	MAX_SKILL_TABLE_DATA	= 21;	// 1~20번 사용

const BYTE	MAX_TABLE_ROW			= 8;
const BYTE	MAX_TABLE_COLUMN		= 4;

const BYTE	MAX_RANK				= 9;	// 1~9
const BYTE	MAX_RANK_LEVEL			= 5;	// Rank1/1~4
const BYTE	BASE_RANK				= 1;	// 랭크는 1부터 시작
const BYTE	BASE_RANK_LEVEL			= 1;	// 랭크 레벨도 1부터 시작
const BYTE	BASE_ML_SKILL_LEVEL		= 1;	// 마스터레벨 스킬 레벨은 1부터 시작
const BYTE	BASE_ML_SKILL_LEVEL_GAP	= 1;	// 같은 랭크의 마스터레벨 스킬 레벨은 1씩 차이난다.

typedef struct _ML_SKILL_TABLE
{
	// 데이터를 미리 계산해 둔다.
	BYTE	btClass;		// 캐릭터 클래스
	BYTE	btType;			// 4가지 중 어떤 타입인가.

	// 실제 데이터
	BYTE	btWidth;									// 몇 줄짜리?
	INT		iAbility[MAX_TABLE_ROW][MAX_TABLE_COLUMN];	// 스킬테이블 정보
} ML_SKILL_TABLE, *LPML_SKILL_TABLE;


typedef struct  _MASTERLEVEL_PASSIVE_SKILL_OPTION
{
	INT		iMpsAttackSuccessRate;			// 공격성공률 상승

	INT		iMpsPVPAttackDmgRate;			// 대인공격성공률 상승
	INT		iMpsPVPBlockingRate;			// 대인방어성공률 증가

	INT		iMpsDownDur1;					// 내구 감소1
	BYTE	btMpsDownDur1Level;				// 내구 감소1 레벨	( **모든 옵션의 레벨을 저장하지 않고 필요할 때만 추가한다. )
	INT		iMpsDownDur2;					// 내구 감소2	: 날개, 반지/목걸이

	INT		iMpsResistancePoison;			// 독 저항력 상승
	INT		iMpsResistanceThunder;			// 번개 저항력 상승
	INT		iMpsResistanceIce;				// 얼음 저항력 상승

	INT		iMpsAutoRecoverLife;			// 생명력 자동회복 강화

	INT		iMpsPlusZen;					// 젠 증가
	
#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	// 패시브스킬 2차 추가 데이터
	INT		iMpsDefence;					// 방어력 증가

	INT		iMpsMaxHP;						// 최대HP 증가
	INT		iMpsMaxBP;						// 최대AG(BP) 증가

	INT		iMpsMonsterDieGetMana;			// 몬스터 킬 시 마나회복
	INT		iMpsMonsterDieGetLife;			// 몬스터 킬 시 생명력회복
	INT		iMpsMonsterDieGetSD;			// 몬스터 킬 시 SD회복

	INT		iMpsPlusExp;					// 경험치 상승
#endif	// ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 패시브스킬 4랭크 추가
	INT		iMpsMaxSD;						// 최대 SD 증가
	INT		iMpsSDSpeed;					// SD 회복 속도 상승
	INT		iMpsMaxAttackDamage;			// 최대 공격력 상승
	INT		iMpsMinAttackDamage;			// 최소 공격력 상승
	INT		iMpsDecreaseMana;				// 마나 감소 강화
	INT		iMpsMaxMagicDamage;				// 최대 마력 상승
	INT		iMpsMinMagicDamage;				// 최소 마력 상승
	INT		iMpsPetDurDownSpeed;			// 펫 내구 소모 속도 감소	
	INT		iMpsMaxAttackMagicDamage;		// 최대 공격력/마력 상승
	INT		iMpsMinAttackMagicDamage;		// 최소 공격력/마력 상승
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

} MASTERLEVEL_PASSIVE_SKILL_OPTION, *LPMASTERLEVEL_PASSIVE_SKILL_OPTION;



class CMasterSkillSystem  
{
private:
	INT				m_iRankTable[MAX_RANK][MAX_RANK_LEVEL];		// 스킬 단계별 투자 요구 포인트	정보
	INT				m_iRankPointSum[MAX_RANK];					// 랭크별로 요구되는 포인트(다음 랭크로 넘어가기 위한 포인트)
	ML_SKILL_TABLE	m_MLSkillTable[MAX_SKILL_TABLE_DATA];		// 스킬 트리 정보(클라이언트 표시용)
																// 지금은 안쓰는데 나중에 쓸 데 있을까봐 읽어둔다.

public:
	CMasterSkillSystem();
	virtual ~CMasterSkillSystem();

	VOID	Init();
	VOID	Load( LPCSTR lpszFileName );


	//////////////////////////////////////////////////////////////////////////
	// S E T
	//////////////////////////////////////////////////////////////////////////
	VOID	InitPassiveSkillData( LPOBJECTSTRUCT lpObj );									// 유저의 패시브스킬 데이터를 초기화한다.
	VOID	CalcPassiveSkillData( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel );	// 유저의 관련 패시브스킬 데이터를 계산한다.


	//////////////////////////////////////////////////////////////////////////
	// C H E C K
	//////////////////////////////////////////////////////////////////////////	
	INT		CheckSkillCondition( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel );		// 마스터레벨 스킬을 습득할 수 있는지 조건 검사
	BOOL	CheckCompleteBrandOfSkill( LPOBJECTSTRUCT lpObj, INT iMLSkill );				// 선행스킬이 달성되었는지 확인한다.

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	BOOL	CheckMasterLevelSkill( INT iMLSkill );											// 스킬 번호로 마스터레벨 스킬인지 확인한다.
	BOOL	IsExistMLSBasedSkill( LPOBJECTSTRUCT lpObj, INT iSkill );						// 일반 스킬번호가 기본이되는 마스터레벨 스킬이 있어?
#endif
	

	//////////////////////////////////////////////////////////////////////////
	// G E T
	//////////////////////////////////////////////////////////////////////////
#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	
	INT		GetBaseMasterLevelSkill( INT iMLSkill );										// 스킬값으로 기본 마스터레벨 스킬 번호를 얻는다.	
#endif


#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	//////////////////////////////////////////////////////////////////////////
	// A D D
	//////////////////////////////////////////////////////////////////////////	
	INT		gObjMagicAdd_MLS( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel );		// 마스터레벨스킬을 마법리스트에 추가한다.	
#endif


	//////////////////////////////////////////////////////////////////////////
	// Client <-> GS 처리
	//////////////////////////////////////////////////////////////////////////
	VOID CGReqGetMasterLevelSkill( LPPMSG_REQ_MASTERLEVEL_SKILL lpMsg, int aIndex );		// CG [0xF3][0x52] 마스터레벨 스킬 습득 요청		


#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	//////////////////////////////////////////////////////////////////////////
	// A C T I V E    S K I L L   R U N N I N G
	//////////////////////////////////////////////////////////////////////////
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터 레벨 4랭크( 스킬 사용 위치 추가)
	BOOL	RunningSkill_MLS( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic, BOOL bCombo, BYTE x = 0, BYTE y = 0, BYTE aTargetPos = 0 );
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	BOOL	RunningSkill_MLS( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic, BOOL bCombo );
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
	VOID	MLS_WizardMagicDefense( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );				// 소울바리어 강화
	VOID	MLS_Healing( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );						// 치료강화 (요정)
	VOID	MLS_Wheel( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic, INT aTargetIndex, BOOL isCombo = FALSE );	// 회오리베기 강화	
	VOID	MLS_KnightSkillAddLife( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic );											// 스웰라이프 강화	
	BOOL	MLS_DarkHorseAttack( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );				// 어스쉐이크강화
	BOOL	MLS_FireBurst( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );						// 파이어버스트 강화
#endif

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터 레벨 4랭크
	BOOL	MLS_SkillAreaMonsterAttack(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP = TRUE, int nSuccessRate = 0, int nDelay = 0);	// 범위 스킬 사용
	BOOL	MLS_SuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);	// 서든 아이스 강화
	int		MLS_CalDistance(int x, int y, int x1, int y1);
	int		MLS_GetAngle(int x, int y, int tx, int ty);
	void	MLS_SkillFrustrum(int aIndex, float fangle, float tx,float ty);
	BOOL	MLS_KnightBlow(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL isCombo);				// 블로우 강화
	void	MLS_SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo);	// 분노의 일격 강화
	void	MLS_SkillDefense(int aIndex, int aTargetIndex, CMagicInf *lpMagic);				// 방어력 증가 강화
	void	MLS_SkillAttack(int aIndex, int aTargetIndex, CMagicInf *lpMagic);				// 공격력 증가 강화
	void	MLS_MaGumSkillDefenseDown(int aIndex, int aTargetIndex);	// 블러드 어택 강화
	void	MLS_PowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE Targetangle, int aTargetIndex);	// 파워 슬래쉬 강화
	BOOL	MLS_FireScream( int aIndex, int aTargetIndex, CMagicInf *lpMagic );		// 파이어 스크림 강화
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
// 강화된 무기스킬 제거(실질적으로는 lpObj->Magic[n]에서 지우지 않음)
	BOOL	MLS_WeaponSkillDel(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
// 강화된 무기스킬 추가(실질적으로는 lpObj->Magic[n]에서 추가하지 않음)
	BOOL	MLS_WeaponSkillAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
	BOOL	CheckUsableWeaponSkill(LPOBJECTSTRUCT lpObj, int nCount);
#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807	
};

extern CMasterSkillSystem g_MasterSkillSystem;
#endif

#endif // !defined(AFX_MASTERSKILLSYSTEM_H__D1E7613A_BBDC_4296_8186_0043960ABF4B__INCLUDED_)
