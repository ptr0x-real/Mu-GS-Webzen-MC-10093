// MagicDamage.h: interface for the CMagicDamage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICDAMAGE_H__46975186_DBC4_428B_9854_61414B8B3BF4__INCLUDED_)
#define AFX_MAGICDAMAGE_H__46975186_DBC4_428B_9854_61414B8B3BF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
#define MAX_STATUS	3	// 길드마스터, 부길드마스터, 배틀마스터
enum SKILL_USABLE_STATUS
{
	SKILL_USABLE_STATUS_GUILDMASTER		= 0,
	SKILL_USABLE_STATUS_SUBMASTER		= 1,
	SKILL_USABLE_STATUS_BATTLEMASTER	= 2,
};

enum SKILL_USETYPE
{
	SKILL_USETYPE_NORMAL	= 0,	// 일반스킬
	SKILL_USETYPE_STATUS	= 1,	// 직책스킬
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	// 2번은 클라이언트에서 무언가에 쓰고있다고 한다.
	SKILL_USETYPE_MASTERSKILL_PASSIVE	= 3,	// 마스터레벨 유저가 사용 가능한 스킬 타입 - 패시브스킬
#endif
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	SKILL_USETYPE_MASTERSKILL_ACTIVE	= 4,	// 마스터레벨 유저가 사용 가능한 스킬 타입 - 액티브스킬
#endif	
};

#endif


class CMagicDamage
{
private :
	int  m_Damage[MAX_SKILL];						// 데미지 값
	char m_Name[MAX_SKILL][50];						// 스킬 이름
	BYTE m_rLevel[MAX_SKILL];						// 
	WORD m_Mana[MAX_SKILL];							// 스킬 사용시 소모 마나
	BYTE m_Distance[MAX_SKILL];						// 스킬 사용 거리
	int  m_RequireEnergy[MAX_SKILL];				// 필요 에너지
	BYTE m_RequireClass[MAX_SKILL][MAX_CLASSTYPE];	// 사용 가능한 클래스


#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
	int	m_RequireLevel[MAX_SKILL];					// 요구 레벨
#endif

#ifdef DARKLORD_WORK
	int  m_RequireLeadership[MAX_SKILL];			// 필요 통솔
	int	 m_Delay[MAX_SKILL];						// 스킬 사용 후 재사용까지 걸리는 시간(초단위)
#endif

#ifdef NEW_FORSKYLAND2
	WORD m_BrainPower[MAX_SKILL];					// 필요 AG
#endif

	int m_Attr[MAX_SKILL];							// 속성1 -1:없음 0:얼음 1:독 2:번개 3:불 4:땅 5:바람 6:물

#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int m_iSkillType[MAX_SKILL];					// 속성2 -1:없음 0:물리공격 1:마법공격
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116			// 스킬의 브랜드와 킬카운트가 들어감
	int		m_iSkillUseType[MAX_SKILL];				// 직책스킬 등
	int		m_iSkillBrand[MAX_SKILL];
	int		m_iKillCount[MAX_SKILL];
	int		m_iRequireStatus[MAX_SKILL][MAX_STATUS];
#endif
	
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	short		m_iSkillRank[MAX_SKILL];			// 스킬랭크	
	short		m_iSkillGroup[MAX_SKILL];			// 스킬그룹	
	short		m_nRequireMLPoint[MAX_SKILL];		// 필요마스터레벨 포인트
	short		m_iHP[MAX_SKILL];					// 소모체력	
	short		m_iSD[MAX_SKILL];					// 소모SD	
	WORD		m_wKeepTime[MAX_SKILL];				// 지속시간	
	short		m_iRequireStr[MAX_SKILL];			// 필요힘	
	short		m_iRequireDex[MAX_SKILL];			// 필요민첩
#endif
	
	//int  m_Index[MAX_SKILL];	// 마법책 인덱스 (어떤 마법인가를 알려줌)
	int  Get(int skill);

#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	// 확장 된 마스터 스킬 정보를 세팅한다.
	void SetMasterLevelSkillInfo( int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex );
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	void SetEx( int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int *pReqStatus );
#endif
	
#ifdef DARKLORD_WORK

	#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay);
	#else
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay);
	#endif

#else
	#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy);
	#else
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, BYTE * ReqClass, int rEnergy);
	#endif
#endif
	
	BOOL	CheckSkillAttr(int skill, int attr);


public:
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812		// 스킬의 물리/마법 속성 들어감
	enum {
		SKILL_TYPE_NONE			= -1,
		SKILL_TYPE_PHYSICS		= 0,
		SKILL_TYPE_MAGIC		= 1,
	};
#endif

	CMagicDamage();
	virtual ~CMagicDamage();


	void Init();
	void LogSkillList(char *filename="..\\data\\skill.txt");
	void LogSkillNameList(char *filename);

#ifdef SCRIPT_DECODE_WORK	
	void LogSkillListDecode(char *filename);
	void LogSkillNameListDecode(char *filename);
#endif
	
#ifdef AUTH_GAMESERVER	
	void LogSkillList(char *Buffer, int	iSize);
	void LogSkillNameList(char *Buffer, int iSize);
#endif	

	int  SkillGet(int skill);
	int  SkillGetMana(int skill);
	int  SkillGetRequireEnergy(int skill);

#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
	int	 SkillGetRequireLevel(int skill);
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3차전직 추가
	int  SkillGetRequireClass(int Cclass, int ChangeUP, int ThirdChangeUp, int skill);
#else
#ifdef NEW_SKILL_FORSKYLAND
	int  SkillGetRequireClass(int Cclass, int ChangeUP, int skill);
#else
	int  SkillGetRequireClass(int Cclass, int skill);
#endif
#endif

#ifdef NEW_FORSKYLAND2
	int SkillGetBP(int skill);	
#endif

	int	GetSkillAttr(int skill);

#ifdef DARKLORD_WORK
	int GetskillRequireLeadership(int skill);
	int	GetDelayTime(int skill);
#endif

#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int GetSkillType(int iSkill);			// 스킬의 속성이 물리/마법 인지 아니면 없는지 확인
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// 직책이 필요한지 검사
	BOOL CheckStatus( INT iSkill, INT iGuildStatus );
	// Brand Of Skill 이 필요한지 검사
	BOOL CheckBrandOfSkill( INT iSkill );
	// KillCount가 맞는지 체크 ( 0 >= 킬카운트가 충분함. 여분을 반환, 0 < 킬카운트가 부족함. 부족분을 음수로 표현 )
	INT CheckKillCount( INT iSkill, INT iKillCount );
#endif
	
#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
	int GetSkillDistance(int skill);
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	INT GetRequireMLPoint( INT iSkill );		// 스킬번호를 이용하여 필요 마스터레벨을 얻는다.
	INT GetRequireStrength( INT iSkill );		// 스킬번호를 이용하여 필요힘을 얻는다.
	INT GetRequireDexterity( INT iSkill );		// 스킬번호를 이용하여 필요민첩을 얻는다.
	INT GetBrandOfSkill( INT iSkill );			// 스킬번호를 이용하여 선행스킬 번호를 얻는다.
	INT	GetSkillUseType( INT iSkill );			// 스킬번호를 이용하여 적용 대상 정보를 얻는다.
	INT GetSkillLevel( INT iSkill );			// 스킬번호를 이용하여 스킬 레벨 정보를 얻는다.
	INT GetSkillGroup( INT iSkill );			// 스킬번호를 이용하여 랭크 그룹 정보를 얻는다.
	INT GetSkillRank( INT iSkill );				// 스킬번호를 이용하여 랭크 정보를 얻는다.
#endif
	
};

extern CMagicDamage	MagicDamageC;

#endif // !defined(AFX_MAGICDAMAGE_H__46975186_DBC4_428B_9854_61414B8B3BF4__INCLUDED_)
