// ObjUseSkill.h: interface for the CObjUseSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJUSESKILL_H__E0BD0DDC_575D_4D6E_BCE5_6A3C248B679A__INCLUDED_)
#define AFX_OBJUSESKILL_H__E0BD0DDC_575D_4D6E_BCE5_6A3C248B679A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjUseSkill  
{
public:
	CObjUseSkill();
	virtual ~CObjUseSkill();

	// 일반 마법 공격 공속핵 체크
	BOOL	SpeedHackCheck(int aIndex);
	// 사용가능한 스킬 공격인가?
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD 
	BOOL	EnableSkill( int Skill, int SkillLevel );
#else
	BOOL	EnableSkill(BYTE Skill);
#endif
	// 필요한 마나량를 얻는다
	int		GetUseMana(int aIndex, CMagicInf *lpMagic);	

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
	int		GetAddUseMana( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic );
#endif

#ifdef NEW_FORSKYLAND2
	// 필요한 AG파워량을 얻는다	
	int		GetUseBP(int aIndex, CMagicInf *lpMagic);	
#endif

	// 스킬을 사용한다(지속마법 : 공격판정은 후에 클라이언트에서 들어 온다)
#ifdef EXPAND_ATTACK_PACKET_0X1E
	void	UseSkill(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos = 0, int aTargetIndex = -1);
#else
	void	UseSkill(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos = 0);
#endif

	// 스킬 공격을 한다(마법 : 서버족에서 공격 및 스킬 사용 판정을 한다
	void	UseSkill(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 스킬 발동
	BOOL	RunningSkill(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL bCombo = FALSE);

	// 마검사방어력감소
	void	MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level);
	// 기사 생명력 증가
	void	KnightSkillAddLife(int aIndex, int skill_level);
	// 법사방어력증가
#ifdef NEW_FORSKYLAND3
	void	WizardMagicDefense(int aIndex, int aTargetIndex, int skill_level);
#else
	void	WizardMagicDefense(int aIndex, int skill_level);
#endif
	// 방어력증가 (약30초간 (아아템레벨+1)*3만큼 증가)
	void	SkillDefense(int aIndex, int aTargetIndex, int skill_level);
	//공격력증가 (약30초간 (아아템레벨+1)*3만큼 증가)
	void	SkillAttack(int aIndex, int aTargetIndex, int skill_level);
	// 변신 마법 사용
	BOOL	SkillChangeUse(int aIndex);
	// 요정이 힐링 마법을 사용한다
	void	SkillHealing(int aIndex, int aTargetIndex, int skill_level);
	// 몬스터를 소환한다.
	BOOL	SkillMonsterCall(int aIndex, int MonsterType, int x, int y);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 공격 판정
	////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NEW_ATTACK_PROC		
	// 관통
	void	SkillPenetration(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	// 방어력 감소(블러드어택)
	void	SkillDefenseDown(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	// 불기둥
	void	SkillFlame(int aIndex, CMagicInf *lpMagic);			
	// 악령
	void	SkillEvil(int aIndex, CMagicInf *lpMagic);			
	// 헬파이어
	void	SkillHellFire(int aIndex, CMagicInf *lpMagic);			
	// 블래스트
	void	SkillBlast(int aIndex, CMagicInf *lpMagic);		

	void	SkillWheel(int aIndex, CMagicInf *lpMagic);

	void	SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 공격 판정 끝
	////////////////////////////////////////////////////////////////////////////////////////////////////////	
#endif

#ifdef ADD_SKILL_WITH_COMBO	
	BOOL	SkillDeathPoison(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);
	BOOL	SkillSuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);

	// 헬파이어2 시작
	BOOL	SkillHellFire2Start(int aIndex, CMagicInf *lpMagic);
	// 헬파이어2 적용
	BOOL	SkillHellFire2(int aIndex, int aTargetIndex,CMagicInf *lpMagic);	

	// 분노의 일격
	void	SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo = FALSE);
	// 회오리
	void	SkillWheel(int aIndex, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo = FALSE);

	// 블로우(기사)
	BOOL	SkillKnightBlow(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL isCombo = FALSE);	
#endif

#ifdef ADD_SKILL_20031022			
	// 파워 슬래쉬 (마검 스킬)
	#ifdef EXPAND_ATTACK_PACKET_0X1E
		void SkillPowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	#else
		void SkillPoweSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	#endif
#endif

	void	SkillFrustrum(int aIndex, float bangle, float tx,float ty);
	int		GetTargetLinePath(int sx, int sy, int tx, int ty, int* PathX, int* PathY, int distance);

	int		GetAngle(int x, int y, int tx, int ty);
	int		CalDistance(int x, int y, int x1, int y1);

#ifdef DARKLORD_WORK

	void	SkillFrustrum2(int aIndex, float fangle, float tx,float ty);
	// 스킬 공격 영역을 검사 한다: 방향각, 나의위치(x,y), 대상위치(tx,ty) 거리(dis), 호의크기(arc)
	BOOL	SkillAreaCheck(int angel, int x, int y, int tx, int ty, int dis, int arc);
	
	// 스피어(다크로드)
	BOOL	SkillSpear(int aIndex, int aTargetIndex,CMagicInf *lpMagic);
	// 파이어 버스트(다크로드)
	BOOL	SkillFireBurst(int aIndex, int aTargetIndex,CMagicInf *lpMagic);
	// 다크호스 어택(다크로드)
	BOOL	SkillDarkHorseAttack(int aIndex, int aTargetIndex,CMagicInf *lpMagic);

	// 파티 소환걸린 파티원이 있는지 검사
	BOOL	RecallPartyCheck(int aIndex, int skill_level);
	// 파티 소환(다크로드)
	void	SkillRecallParty(int aIndex, int skill_level);
	// 크리티컬 데미지추가(다크로드)
	void	SkillAddCriticalDamage(int aIndex, int skill_level);
	// 일렉트릭 스파크(다크로드)
	void	SkillElectricSpark(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
#endif
	
#ifdef ADD_NEW_MAP_KALIMA_20040518
	// 몬스터가 몬스터를 소환하는 마법
	BOOL	SkillSummon			(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 마법방어 무적
	BOOL	SkillImmuneToMagic	(int aIndex, CMagicInf *lpMagic);
	// 물리방어 무적
	BOOL	SkillImmuneToHarm	(int aIndex, CMagicInf *lpMagic);
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// [ 0x19 ]

	// 블레이드 나이트
	BOOL	SkillKnightRush			(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 소울 마스터
	BOOL	SkillWizardJavalin		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 뮤즈 엘프
	BOOL	SkillElfRush			(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 기사형 마검사
	BOOL	SkillMagumOneFlash		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 법사형 마검사
	BOOL	SkillMagumDeathCannon	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 다크로드 공격 스킬
	BOOL	SkillDarklordSpaceSplit	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);

	// 다크로드 보조 스킬
	BOOL	SkillBrandOfSkill	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 스턴 해제	
	BOOL	SkillRemoveStun		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 스웰 마나
	BOOL	SkillAddMana		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 투명
	BOOL	SkillCloaking		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 투명 해제
	BOOL	SkillRemoveCloaking	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// 마법 해제
	BOOL	SkillRemoveMagic	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	BOOL	RemoveCloakingEffect(int aIndex);
#endif

	// [ 0x1E ]
	// 스턴
	BOOL	SkillStun			(int aIndex, int aTargetIndex, CMagicInf *lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos );

	// 스킬 상태 해제
	BOOL	RemoveAllCharacterInvalidMagicAndSkillState( LPOBJECTSTRUCT lpObj );	
#endif
	
#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	BOOL	RemoveSpecificSkillState( LPOBJECTSTRUCT lpObj, INT iSkillNumber );

#endif


#ifdef ADD_ITEM_FENRIR_01_20051110

	// [ 0x1E ]
	BOOL	SkillFenrirAttack	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);

#endif
	
#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	// [ 0x19 ]
	BOOL	SkillInfinityArrow( int aIndex, int aTargetIndex, CMagicInf *lpMagic );// 뮤즈엘프 스킬 - 인피니티 애로우
#endif
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// 파이어 스크림 폭발 처리 함수 선언
	// [ 0x1D ]
	BOOL	SkillFireScream( int aIndex, int aTargetIndex, CMagicInf *lpMagic );
	BOOL	FireScreamExplosionAttack( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int iAttackDamage);		// 파이어 스크림을 사용한 후 더블데미지가 뜨면 주위의 몬스터도 데미지를 받는다.
#endif

#ifdef MODIFY_SKILL_EVEIL_AT_SERVER_20070117
	// [ 0x1E ]
	BOOL	SkillNewEvil( int aIndex, int aTargetIndex, CMagicInf *lpMagic );		// 악령 - 서버판정
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	//////////////////////////////////////////////////////////////////////////
	//	특정 스킬에 대한 추가 처리
	//////////////////////////////////////////////////////////////////////////
	void SpecificSkillAdditionTreat( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf* lpMagic, int iAttackDamage );
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	BOOL	SkillLightningOrb(int aIndex, int aTargetIndex,CMagicInf *lpMagic);

	// 스플레쉬 데미지 계산
	BOOL	SplashDamage(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int nSkill, int nAttackDamage, int nDistance = 1, int nRate = 50);

	BOOL	SkillDrainLife(int aIndex, int aTargetIndex,CMagicInf *lpMagic);
	BOOL	SkillChainLightning(int aIndex, int aTargetIndex,CMagicInf *lpMagic);

	BOOL	SkillSummons(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);	// 소환수 스킬
	
	BOOL	SkillThorns(int aIndex, int aTargetIndex, CMagicInf *lpMagic);	// 손즈(데미지반사)
	BOOL	SkillSleep(int aIndex, int aTargetIndex, CMagicInf *lpMagic);		// 슬립
	BOOL	SkillBlind(int aIndex, int aTargetIndex, CMagicInf *lpMagic);	// 블라인드
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	
	
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
	BOOL	SkillWeakness(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	BOOL	SkillEnervation(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
#endif

#ifdef SEASON4_PATCH_GROOVING
	// --->
	// 시즌 4.0 추가 스킬  grooving 2008/04/24
	BOOL	SkillLightningShock( int aIndex, int aTargetIndex, CMagicInf *lpMagic );	// 라이트닝 쇼크 
	BOOL	SkillBerserker(int aIndex, CMagicInf *lpMagic);			// 버서커
	// <---
#endif

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	// 환수스킬 펄루션
	BOOL	UseSkillPollution(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int nMsg = MSG_SKILL_PULLUTION1);		// 펄루션
	BOOL	SkillAreaMonsterAttack(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP = TRUE, int nDelay = 0);	// 범위 스킬 사용
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	
	
#ifdef ADD_SEASON4_NEW_SKILL_20080522	//  신규 스킬 사용
	BOOL	SkillDestruction(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);		// 파괴의 일격(흑기사)
	void	SkillFrustrum3(int aIndex, BYTE bAngle, float tx1, float ty1, float tx2, float ty2 );
	void	SkillFlameStrike(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);		// 플레임 스트라이크(마검사)
	BOOL	SkillGiganticStorm(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);	// 기간틱 스톰(마검사)
	void	SkillMultiShot(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);			// 멀티샷(요정)
	void	SkillRecovery(int aIndex, int aTargetIndex, CMagicInf *lpMagic);						// 회복(요정)
	BOOL	SkillMagicPower(int aIndex, int aTargetIndex, CMagicInf *lpMagic);						// 마력 증대(법사)
	void	SkillchaoticDesair(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);		// 카오틱 디세이어(다크로드)
#endif	// ADD_SEASON4_NEW_SKILL_20080522
};

extern CObjUseSkill gObjUseSkill;

#endif // !defined(AFX_OBJUSESKILL_H__E0BD0DDC_575D_4D6E_BCE5_6A3C248B679A__INCLUDED_)
