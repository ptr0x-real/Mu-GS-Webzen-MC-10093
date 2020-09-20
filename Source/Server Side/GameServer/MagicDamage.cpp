// MagicDamage.cpp: implementation of the CMagicDamage class.
// 마법(스킬)마다의 데미지 값을 관리하는 클래스 
// 주의 : 스킬번호는 중복될 수 없다.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Include\public.h"
#include "Include\Readscript.h"
#include "MagicInf.h"
#include "MagicDamage.h"

#ifdef SCRIPT_DECODE_WORK	
#include "Common\WZScriptEncode.h"
#endif

#ifdef AUTH_GAMESERVER
#include "Common\WzMemScript.h"
#endif

CMagicDamage	MagicDamageC;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMagicDamage::CMagicDamage()
{
	//LogSkillList();
}

CMagicDamage::~CMagicDamage()
{

}

void CMagicDamage::Init()
{
	for( int n=0; n<MAX_SKILL; n++)
	{
		m_Damage[n] = -1;
		//m_Index[n]  = -1;
	}
}

// Skill.txt 를 읽어온다.
void CMagicDamage::LogSkillList(char *filename)
{
	Init();

	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox(lMsg.Get(455), filename);
		return ;
	}
	SMDToken Token;
		
	int  n=0;
	int  number;
	char name[50];
	int  requireLevel;
	int  damage;
	int  mana;
	int	 bp = 0;	// AG파워
	int  dis;
	int  rEnergy;
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int  iSkillType;
#endif

#ifdef DARKLORD_WORK
	int  rLeadership;
	int	 rDelay;
#endif

	int  attr;		// 속성

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	int	iSkillUseType;	// 직책 스킬인지, 기본 스킬인지 등등
	int	iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_STATUS] = {0,};
#endif

	BYTE  RequireClass[MAX_CLASSTYPE];
	
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	short		nSkillRank;				// 스킬랭크	
	short		nSkillGroup;			// 스킬그룹	
	short		nRequireMLPoint;		// 필요마스터레벨 포인트
	short		nHP;					// 소모체력	
	short		nSD;					// 소모SD	
	WORD		wKeepTime;				// 지속시간	
	short		nRequireStr;			// 필요힘	
	short		nRequireDex;			// 필요민첩
	BYTE		btIconNum;				// 아이콘 번호(클라이언트용)
#endif

#ifdef ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515
	int			nSkillType;				// 스킬타입
#endif // ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			//   번호	    이름	 필요레벨  데미지 사용마나량	거리
			number = (int)TokenNumber;
			Token = (*GetToken)(); strcpy(name,TokenString);
			Token = (*GetToken)(); requireLevel = (int)TokenNumber;
			Token = (*GetToken)(); damage       = (int)TokenNumber;			
			Token = (*GetToken)(); mana         = (int)TokenNumber;

			Token = (*GetToken)(); bp         = (int)TokenNumber;

			Token = (*GetToken)(); dis			= (int)TokenNumber;
#ifdef DARKLORD_WORK
			Token = (*GetToken)(); rDelay		= (int)TokenNumber;
#else
			Token = (*GetToken)();
#endif
			Token = (*GetToken)(); rEnergy		= (int)TokenNumber;
			
#ifdef DARKLORD_WORK
			Token = (*GetToken)(); rLeadership		= (int)TokenNumber;
#else
			Token = (*GetToken)();
#endif
			
			
			Token = (*GetToken)(); attr	= (int)TokenNumber;

#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
			Token = (*GetToken)(); iSkillType = (int)TokenNumber;
#else
			Token = (*GetToken)();
#endif


#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
			Token = (*GetToken)(); iSkillUseType	= (int)TokenNumber;
			Token = (*GetToken)(); iSkillBrand		= (int)TokenNumber;
			Token = (*GetToken)(); iKillCount		= (int)TokenNumber;

			Token = (*GetToken)(); RequireStatus[0] = (int)TokenNumber;
			Token = (*GetToken)(); RequireStatus[1] = (int)TokenNumber;
			Token = (*GetToken)(); RequireStatus[2] = (int)TokenNumber;
#else
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();

			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
#endif

			Token = (*GetToken)(); RequireClass[0]	= (int)TokenNumber;
			Token = (*GetToken)(); RequireClass[1]	= (int)TokenNumber;
			Token = (*GetToken)(); RequireClass[2]	= (int)TokenNumber;
			Token = (*GetToken)(); RequireClass[3]	= (int)TokenNumber;
			
#ifdef DARKLORD_WORK
			Token = (*GetToken)(); RequireClass[4]	= (int)TokenNumber;
#else
			Token = (*GetToken)();
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// skil 사용 신규 클래스 추가
			Token = (*GetToken)(); RequireClass[5]	= (int)TokenNumber;
#else
			Token = (*GetToken)();
#endif	

			Token = (*GetToken)(); //Rage Fighter Bypass
			
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// skill(kor).txt 파일 읽기
			Token = (*GetToken)();		nSkillRank			= (short)TokenNumber;	// 스킬랭크	
			Token = (*GetToken)();		nSkillGroup			= (short)TokenNumber;	// 스킬그룹	
			Token = (*GetToken)();		nRequireMLPoint		= (short)TokenNumber;	// 필요마스터레벨 포인트
			Token = (*GetToken)();		nHP					= (short)TokenNumber;	// 소모체력	
			Token = (*GetToken)();		nSD					= (short)TokenNumber;	// 소모SD	
			Token = (*GetToken)();		wKeepTime			= (WORD)TokenNumber;	// 지속시간	
			Token = (*GetToken)();		nRequireStr			= (short)TokenNumber;	// 필요힘	
			Token = (*GetToken)();		nRequireDex			= (short)TokenNumber;	// 필요민첩
			Token = (*GetToken)();		btIconNum			= (BYTE)TokenNumber;	// 아이콘 번호
#else
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
			Token = (*GetToken)();
#endif

#ifdef ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515
			Token = (*GetToken)();		nSkillType			= (int)TokenNumber;	// 스킬타입
#else
			Token = (*GetToken)();
#endif // ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515

			Token = (*GetToken)();
			Token = (*GetToken)();

#ifdef DARKLORD_WORK
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, iSkillType, RequireClass, rEnergy, rLeadership,rDelay);
#else
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, RequireClass, rEnergy, rLeadership,rDelay);
#endif
#else
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, iSkillType, RequireClass, rEnergy);
#else
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, RequireClass, rEnergy);
#endif
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
			SetEx( number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus );
#endif
			
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// 마스터레벨 스킬 정보 설정
			SetMasterLevelSkillInfo( number, nSkillRank, nSkillGroup, nRequireMLPoint, nHP, nSD, wKeepTime, nRequireStr, nRequireDex );
#endif
				DefMagicInf[number].Set(number, 0);
		}
	}
	fclose(SMDFile);

	LogAdd(lMsg.Get(456), filename);
}

// 해외판을 위해서 스킬이름만 얻어 온다
void CMagicDamage::LogSkillNameList(char *filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox(lMsg.Get(455), filename);
		return ;
	}
	SMDToken Token;
		
	int  n=0;
	int  number;
	char name[50];	

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{			
			number = (int)TokenNumber;
			Token = (*GetToken)(); strcpy(name,TokenString);

			strcpy(m_Name[number], name);
		}
	}
	fclose(SMDFile);

	LogAdd(lMsg.Get(456), filename);
}


#ifdef AUTH_GAMESERVER
// Skill.txt 를 읽어온다.
void CMagicDamage::LogSkillList(char *Buffer, int iSize)
{
	Init();

	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);

	SMDToken Token;
		
	int  n=0;
	int  number;
	char name[50];
	int  requireLevel;
	int  damage;
	int  mana;
	int	 bp = 0;	// AG파워
	int  dis;
	int  rEnergy;
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int  iSkillType;
#endif

#ifdef DARKLORD_WORK
	int		rLeadership;
	int		rDelay;
#endif

	int  attr;		// 속성

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	int iSkillUseType;
	int	iSkillBrand;
	int iKillCount;
	
	int RequireStatus[MAX_STATUS] = {0,};
#endif
	
	BYTE  RequireClass[MAX_CLASSTYPE];

#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	int		iSkillRank;				// 스킬랭크	
	int		iSkillGroup;			// 스킬그룹	
	short	nRequireMasterLevel;	// 필요마스터레벨	
	int		iHP;					// 소모체력	
	int		iSD;					// 소모SD	
	WORD	wKeepTime;				// 지속시간	
	int		iRequireStr;			// 필요힘	
	int		iRequireDex;			// 필요민첩
	BYTE	btIconNum;				// 아이콘 번호
#endif

#ifdef ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515
	int		nSkillType;				// 스킬타입
#endif // ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515

	while( true )
	{	
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			//   번호	    이름	 필요레벨  데미지 사용마나량	거리
			number = WzMemScript.GetNumber();			
			Token = WzMemScript.GetToken(); strcpy(name,WzMemScript.GetString());			
			Token = WzMemScript.GetToken(); requireLevel = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); damage       = WzMemScript.GetNumber();			
			Token = WzMemScript.GetToken(); mana         = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken(); bp         = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken(); dis			= WzMemScript.GetNumber();

#ifdef DARKLORD_WORK
			Token = WzMemScript.GetToken(); rDelay			= WzMemScript.GetNumber();
#endif

			Token = WzMemScript.GetToken(); rEnergy		= WzMemScript.GetNumber();
			
#ifdef DARKLORD_WORK
			Token = WzMemScript.GetToken(); rLeadership	= WzMemScript.GetNumber();			
#endif
			
			Token = WzMemScript.GetToken(); attr	= WzMemScript.GetNumber();
			
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
			Token = WzMemScript.GetToken();	iSkillType = WzMemScript.GetNumber();
#endif


#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
			Token = WzMemScript.GetToken();	iSkillUseType = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();	iSkillBrand = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();	iKillCount = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken(); RequireStatus[0] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); RequireStatus[1] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); RequireStatus[2] = WzMemScript.GetNumber();
		
#endif

			Token = WzMemScript.GetToken(); RequireClass[0]	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); RequireClass[1]	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); RequireClass[2]	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); RequireClass[3]	= WzMemScript.GetNumber();

#ifdef DARKLORD_WORK
			Token = WzMemScript.GetToken(); RequireClass[4]	= WzMemScript.GetNumber();			
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// skil 사용 신규 클래스 추가
			Token = WzMemScript.GetToken(); RequireClass[5]	= WzMemScript.GetNumber();			
#endif

#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// skill(kor).txt 파일 읽기 (AUTH)
			Token = WzMemScript.GetToken();		iSkillRank			= WzMemScript.GetNumber();	// 스킬랭크	
			Token = WzMemScript.GetToken();		iSkillGroup			= WzMemScript.GetNumber();	// 스킬그룹	
			Token = WzMemScript.GetToken();		nRequireMasterLevel	= WzMemScript.GetNumber();	// 필요마스터레벨	
			Token = WzMemScript.GetToken();		iHP					= WzMemScript.GetNumber();	// 소모체력	
			Token = WzMemScript.GetToken();		iSD					= WzMemScript.GetNumber();	// 소모SD	
			Token = WzMemScript.GetToken();		wKeepTime			= WzMemScript.GetNumber();	// 지속시간	
			Token = WzMemScript.GetToken();		iRequireStr			= WzMemScript.GetNumber();	// 필요힘	
			Token = WzMemScript.GetToken();		iRequireDex			= WzMemScript.GetNumber();	// 필요민첩
			Token = WzMemScript.GetToken();		btIconNum			= WzMemScript.GetNumber();	// 아이콘 번호
#endif

#ifdef ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515
			Token = WzMemScript.GetToken();		nSkillType			= WzMemScript.GetNumber();	// 스킬타입
#endif // ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515

#ifdef DARKLORD_WORK
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, iSkillType, RequireClass, rEnergy, rLeadership,rDelay);
#else
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, RequireClass, rEnergy, rLeadership,rDelay);
#endif
#else
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, iSkillType, RequireClass, rEnergy);
#else
				Set(name, number, damage, requireLevel, mana, bp, dis, attr, RequireClass, rEnergy);
#endif
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
			SetEx( number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
#endif
			
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// 마스터레벨 스킬 정보 설정 (AUTH)
			SetMasterLevelSkillInfo( number, iSkillRank, iSkillGroup, nRequireMasterLevel, iHP, iSD, wKeepTime, iRequireStr, iRequireDex );
#endif
			
			DefMagicInf[number].Set(number, 0);
		}
	}

	LogAdd(lMsg.Get(456), "Skill");
}

// 해외판을 위해서 스킬이름만 얻어 온다
void CMagicDamage::LogSkillNameList(char *Buffer, int iSize)
{
	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);

	SMDToken Token;
		
	int  n=0;
	int  number;
	char name[50];	

	while( true )
	{		
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{			
			number = (int)TokenNumber;
			Token = WzMemScript.GetToken(); strcpy(name,TokenString);

			strcpy(m_Name[number], name);
		}
	}
	LogAdd(lMsg.Get(456), "Skill_Local");
}
#endif

#ifdef SCRIPT_DECODE_WORK
// Skil.txt 를 읽어온다.
void CMagicDamage::LogSkillListDecode(char *filename)
{
	Init();

	CWZScriptEncode		Encoder;

	if( !Encoder.Open(filename) )
		return;

	WZSMDToken Token;
		
	int  n=0;
	int  number;
	char name[50];
	int  requireLevel;
	int  damage;
	int  mana;
	int	 bp = 0;	// AG파워
	int  dis;
	int  rEnergy;
	BYTE  RequireClass[MAX_CLASSTYPE];

	while( true )
	{
		
		Token = Encoder.GetToken();
		if(Token == T_END) break;
		if(Token == T_NUMBER)
		{
			//   번호	    이름	 필요레벨  데미지 사용마나량	거리
			number = Encoder.GetNumber();
			Token = Encoder.GetToken(); strcpy(name,Encoder.GetString());
			Token = Encoder.GetToken(); requireLevel = Encoder.GetNumber();
			Token = Encoder.GetToken(); damage       = Encoder.GetNumber();			
			Token = Encoder.GetToken(); mana         = Encoder.GetNumber();

#ifdef NEW_FORSKYLAND2
			Token = Encoder.GetToken(); bp         = Encoder.GetNumber();
#endif

			Token = Encoder.GetToken(); dis			= Encoder.GetNumber();
			Token = Encoder.GetToken(); rEnergy		= Encoder.GetNumber();
			
			Token = Encoder.GetToken(); RequireClass[0]	= Encoder.GetNumber();
			Token = Encoder.GetToken(); RequireClass[1]	= Encoder.GetNumber();
			Token = Encoder.GetToken(); RequireClass[2]	= Encoder.GetNumber();
			Token = Encoder.GetToken(); RequireClass[3]	= Encoder.GetNumber();

#ifdef NEW_FORSKYLAND2
			Set(name, number, damage, requireLevel, mana, bp, dis, RequireClass, rEnergy);
#else
			Set(name, number, damage, requireLevel, mana, dis, RequireClass, rEnergy);
#endif
			DefMagicInf[number].Set(number, 0);
		}
	}

	LogAdd(lMsg.Get(456), filename);
}

// 해외판을 위해서 스킬이름만 얻어 온다
void CMagicDamage::LogSkillNameListDecode(char *filename)
{
	CWZScriptEncode		Encoder;

	if( !Encoder.Open(filename) )
		return;

	WZSMDToken Token;
		
	int  n=0;
	int  number;
	char name[50];	

	while( true )
	{
		Token = Encoder.GetToken();
		if(Token == T_END) break;
		if(Token == T_NUMBER)
		{			
			number = Encoder.GetNumber();
			Token = Encoder.GetToken(); strcpy(name,Encoder.GetString());

			strcpy(m_Name[number], name);
		}
	}

	LogAdd(lMsg.Get(456), filename);
}
#endif

#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// SetMasterLevelSkillInfo(...)
//----------------------------------------------------------------------------
// 확장 된 마스터 스킬 정보를 세팅한다. (RKFKA)
//----------------------------------------------------------------------------
void CMagicDamage::SetMasterLevelSkillInfo( int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		MsgBox(lMsg.Get(457),__FILE__, __LINE__);
		return;
	}

	// 스킬이 없다면.
	if( m_Damage[iSkill] == -1 ) 
	{
		MsgBox(lMsg.Get(562),__FILE__, __LINE__);
		return;
	}

	m_iSkillRank[iSkill]			= iSkillRank;			// 스킬랭크	
	m_iSkillGroup[iSkill]			= iSkillGroup;			// 스킬그룹	
	m_nRequireMLPoint[iSkill]		= iRequireMLPoint;		// 필요마스터레벨포인트	
	m_iHP[iSkill]					= iHP;					// 소모체력	
	m_iSD[iSkill]					= iSD;					// 소모SD	
	m_wKeepTime[iSkill]				= wKeepTime;			// 지속시간	
	m_iRequireStr[iSkill]			= iRequireStr;			// 필요힘	
	m_iRequireDex[iSkill]			= iRequireDex;			// 필요민첩

}
#endif	// EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122

//----------------------------------------------------------------------------
// 확장 된 스킬값을 세팅한다. (apple)
//----------------------------------------------------------------------------
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
void CMagicDamage::SetEx( int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int *pReqStatus  )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		MsgBox(lMsg.Get(457),__FILE__, __LINE__);
		return;
	}

	// 스킬이 없다면.
	if( m_Damage[iSkill] == -1 ) 
	{
		MsgBox(lMsg.Get(562),__FILE__, __LINE__);
		return;
	}

	m_iSkillUseType[iSkill] = iSkillUseType;
	m_iSkillBrand[iSkill]	= iSkillBrand;
	m_iKillCount[iSkill]	= iKillCount;

	memcpy(m_iRequireStatus[iSkill], pReqStatus, MAX_STATUS*sizeof(INT));
}


#endif

//----------------------------------------------------------------------------
// 스킬값을 세팅한다.
//----------------------------------------------------------------------------
#ifdef DARKLORD_WORK
	#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
		void CMagicDamage::Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay)
	#else
		void CMagicDamage::Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay)
	#endif
#else
	#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
		void CMagicDamage::Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy)
	#else
		void CMagicDamage::Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, BYTE * ReqClass, int rEnergy)
	#endif
#endif
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		MsgBox(lMsg.Get(457),__FILE__, __LINE__);
		return;
	}
	if( m_Damage[skill] != -1 ) 
	{
		MsgBox(lMsg.Get(562),__FILE__, __LINE__);
		return;
	}
	strcpy(m_Name[skill], name);
	
	m_Damage[skill]			= damage;
	m_rLevel[skill]			= rlevel;
	m_Mana[skill]			= mana;
	m_Distance[skill]		= dis;

#ifdef DARKLORD_WORK
	m_RequireLeadership[skill] = rLeadership;
	m_Delay[skill] = rdelay;
#endif

#ifdef NEW_FORSKYLAND2	
	m_BrainPower[skill]		= bp;
#endif

	m_Attr[skill] = Attr;

#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	m_iSkillType[skill] = iSkillType;
#endif	
	
	memcpy(m_RequireClass[skill], ReqClass, MAX_CLASSTYPE);
		
#ifdef MODIFY_BASIC_SKILL_REQENG_20080102
	int nBaseSkillReqEng = rEnergy * rlevel * 4;
	if( nBaseSkillReqEng > 0 )
		nBaseSkillReqEng /= 100;
	else
		nBaseSkillReqEng = 0;

	#ifdef MODIFY_SKILL_REQUIREENERGY_20080423
	m_RequireEnergy[skill]	= 20 + nBaseSkillReqEng;
	#else
	m_RequireEnergy[skill]	= 10 + nBaseSkillReqEng;
	#endif // MODIFY_SKILL_REQUIREENERGY_20080423
#else
	m_RequireEnergy[skill]	= 20+rEnergy*(rlevel)*4/100;
#endif
		
	if( skill == 13 )
		LogAddL("skill:%d",m_RequireEnergy[skill]);

#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
	m_RequireLevel[skill] = 0;
	if( skill == AT_SKILL_WHEEL )				m_RequireLevel[skill] = 80;			// 회오리베기
	else if( skill == AT_SKILL_BLOWOFFURY )		m_RequireLevel[skill] = 170;		// 분노의 일격
	else if( skill == AT_SKILL_STRIKE )			m_RequireLevel[skill] = 160;		// 블로우
	else if( skill == AT_SKILL_KNIGHTSPEAR )	m_RequireLevel[skill] = 28;		// 창찌르기
	else if( skill == AT_SKILL_KNIGHTADDLIFE )	m_RequireLevel[skill] = 120;		// 스웰라이프			
	else if( skill == AT_SKILL_PENETRATION )	m_RequireLevel[skill] = 130;		// 관통
#endif
	
	if( skill == 30 ) m_RequireEnergy[skill] = 30;
	else if( skill == 31 ) m_RequireEnergy[skill] = 60;
	else if( skill == 32 ) m_RequireEnergy[skill] = 90;
	else if( skill == 33 ) m_RequireEnergy[skill] = 130;
	else if( skill == 34 ) m_RequireEnergy[skill] = 170;
	else if( skill == 35 ) m_RequireEnergy[skill] = 210;
	else if( skill == 36 ) m_RequireEnergy[skill] = 300;
	else if( skill == AT_SKILL_WHEEL )			m_RequireEnergy[skill] = 0;		// 회오리베기
	else if( skill == AT_SKILL_BLOWOFFURY )		m_RequireEnergy[skill] = 0;		// 분노의 일격
	else if( skill == AT_SKILL_STRIKE )			m_RequireEnergy[skill] = 0;		// 블로우
	else if( skill == AT_SKILL_KNIGHTSPEAR )	m_RequireEnergy[skill] = 0;		// 창찌르기
	else if( skill == AT_SKILL_KNIGHTADDLIFE )	m_RequireEnergy[skill] = 0;		// 스웰라이프
	else if( skill == AT_SKILL_KNIGHTDINORANT ) m_RequireEnergy[skill] = 0;		// 레이드 슛
	else if( skill == AT_SKILL_DEFENSEDOWN )	m_RequireEnergy[skill] = 0;		// 블러드어택
	else if( skill == AT_SKILL_ELFHARDEN )		m_RequireEnergy[skill] = 0;		// 아이스애로우
	else if( skill == AT_SKILL_PENETRATION )	m_RequireEnergy[skill] = 0;		// 관통
	else if( skill == AT_SKILL_CROSSBOW )	m_RequireEnergy[skill] = 0;			// 요정 다발
	else if( skill == AT_SKILL_ENERGYBALL )	m_RequireEnergy[skill] = 0;			// 에너지볼
	else if( skill == AT_SKILL_BLOCKING )	m_RequireEnergy[skill] = 0;			// 방어 스킬
	else if( skill == AT_SKILL_SWORD1 )	m_RequireEnergy[skill] = 0;				// 내려찍기
	else if( skill == AT_SKILL_SWORD2 )	m_RequireEnergy[skill] = 0;				// 찌르기
	else if( skill == AT_SKILL_SWORD3 )	m_RequireEnergy[skill] = 0;				// 올려치기
	else if( skill == AT_SKILL_SWORD4 )	m_RequireEnergy[skill] = 0;				// 돌려치기
	else if( skill == AT_SKILL_SWORD5 )	m_RequireEnergy[skill] = 0;				// 베기
	else if( skill == AT_SKILL_BOW )	m_RequireEnergy[skill] = 0;				// 안쓰는 예전 관통	
	else if( skill == AT_SKILL_SWORD6 )	m_RequireEnergy[skill] = 0;				// 데쓰브로드
#ifdef DARKLORD_WORK
	else if( skill == AT_SKILL_SPEAR )	m_RequireEnergy[skill] = 0;				// 데쓰브로드
#endif
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	else if( skill == AT_SKILL_RUSH )			m_RequireEnergy[skill] = 0;		// 블레이드 나이트
	else if( skill == AT_SKILL_JAVALIN )		m_RequireEnergy[skill] = 0;		// 소울 마스터
	else if( skill == AT_SKILL_DEEPIMPACT )		m_RequireEnergy[skill] = 0;		// 뮤즈 엘프
	else if( skill == AT_SKILL_ONE_FLASH )		m_RequireEnergy[skill] = 0;		// 기사형 마검사
	else if( skill == AT_SKILL_DEATH_CANNON )	m_RequireEnergy[skill] = 0;		// 법사형 마검사
	else if( skill == AT_SKILL_SPACE_SPLIT )	m_RequireEnergy[skill] = 0;		// 다크로드 공격 스킬
#endif
#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	else if( skill == AT_SKILL_STUN )				m_RequireEnergy[skill] = 0;		// 스턴
	else if( skill == AT_SKILL_REMOVAL_STUN )		m_RequireEnergy[skill] = 0;		// 스턴해제
	else if( skill == AT_SKILL_ADD_MANA )			m_RequireEnergy[skill] = 0;		// 스웰마나
	else if( skill == AT_SKILL_INVISIBLE )			m_RequireEnergy[skill] = 0;		// 투명
	else if( skill == AT_SKILL_REMOVAL_INVISIBLE )	m_RequireEnergy[skill] = 0;		// 투명해제
	else if( skill == AT_SKILL_REMOVAL_MAGIC )		m_RequireEnergy[skill] = 0;		// 마법해제
#endif
#ifdef ADD_ITEM_FENRIR_01_20051110
	else if( skill == AT_SKILL_FENRIR_ATTACK )		m_RequireEnergy[skill] = 0;	 // 펜릴 스킬
#endif

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	else if( skill == AT_SKILL_INFINITY_ARROW )		m_RequireEnergy[skill] = 0;		// 뮤즈엘프 인피니티 애로우
#endif
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601		// CMagicDamage::Set(...) 파이어 스크림 요구에너지 설정
	else if( skill == AT_SKILL_FIRESCREAM )			m_RequireEnergy[skill] = 0;		// 다크로드 파이어스크림
	else if( skill == AT_SKILL_EXPLOSION )			m_RequireEnergy[skill] = 0;		// 다크로드 폭발
#endif
	
#ifdef MODIFY_DARKLORD_EARTHQUAKE_REQENERGY_BUGFIX_20060914
	else if( skill == AT_SKILL_DARKHORSE_ATTACK )	m_RequireEnergy[skill] = 0;		// 다크로드 어스퀘이크 
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	else if( skill == AT_SKILL_DRAINLIFE)			m_RequireEnergy[skill] = 0;		// 드레인라이프
	else if( skill == AT_SKILL_CHAINLIGHTNING)		m_RequireEnergy[skill] = 0;		// 체인라이트닝
	else if( skill == AT_SKILL_LIGHTNINGORB)			m_RequireEnergy[skill] = 0;		// 라이트닝오브

	else if( skill == AT_SKILL_THORNS)				m_RequireEnergy[skill] = 0;		// 쏜즈

	else if( skill == AT_SKILL_SLEEP)				m_RequireEnergy[skill] = 0;		// 슬립
	else if( skill == AT_SKILL_BLIND)				m_RequireEnergy[skill] = 0;		// 블라인드

	else if( skill == AT_SKILL_EXPLOSION_2)			m_RequireEnergy[skill] = 0;		// 익스플로젼
	else if( skill == AT_SKILL_REQUIEM)				m_RequireEnergy[skill] = 0;		// 레퀴엠
#endif
	
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
	else if( skill == AT_SKILL_WEAKNESS)			m_RequireEnergy[skill] = 0;		// 위크니스
	else if( skill == AT_SKILL_ENERVATION)			m_RequireEnergy[skill] = 0;		// 이너베이션
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
	else if( skill == AT_SKILL_PULLUTION )			m_RequireEnergy[skill] = 0;		// 펄루션
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
	
#ifdef SEASON4_PATCH_GROOVING
	// --->
	// 시즌 4.0 추가 스킬  grooving 2008/04/24
	else if( skill == AT_SKILL_LIGHTING_SHOCK)		m_RequireEnergy[skill] = 0;		// 라이트닝 쇼크
	else if( skill == AT_SKILL_BERSERKER)			m_RequireEnergy[skill] = 0;		// 버서커
	// <---
#endif
	
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 요구 에너지 설정
	else if( skill == AT_SKILL_DESTRUCTION )		m_RequireEnergy[skill] = 0;		// 파괴의 일격
	else if( skill == AT_SKILL_MAGICPOWRER )		m_RequireEnergy[skill] = 0;		// 마력 증가
	else if( skill == AT_SKILL_RECOVERY )			m_RequireEnergy[skill] = 0;		// 회복
	else if( skill == AT_SKILL_MULTISHOT )			m_RequireEnergy[skill] = 0;		// 멀티샷
	else if( skill == AT_SKILL_FLAMESTRIKE )		m_RequireEnergy[skill] = 0;		// 플레임 스트라이크
	else if( skill == AT_SKILL_GIGANTICSTORM )		m_RequireEnergy[skill] = 0;		// 기간틱 스톰
	else if( skill == AT_SKILL_CHAOTICDESAIR )		m_RequireEnergy[skill] = 0;		// 카오틱 디세이어
#endif	// ADD_SEASON4_NEW_SKILL_20080522
}

//----------------------------------------------------------------------------
// 원하는 스킬의 데미지 값을 얻는다.
//----------------------------------------------------------------------------
int CMagicDamage::Get(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return m_Damage[0];
	}
	if( m_Damage[skill] == -1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return m_Damage[0];
	}
	return m_Damage[skill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 데미지 값을 얻는다.
//----------------------------------------------------------------------------
int CMagicDamage::SkillGet(int skill)
{	
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return m_Damage[0];
	}
	if( m_Damage[skill] == -1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return m_Damage[0];
	}
//	LogAdd("skill:%d = %d", skill, m_Damage[skill]);

	return m_Damage[skill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 데미지 값을 얻는다.
//----------------------------------------------------------------------------
int CMagicDamage::SkillGetMana(int skill)
{	
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return m_Mana[0];
	}
	if( m_Damage[skill] == -1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return m_Damage[0];
	}
//	LogAdd("skill:%d = %d", skill, m_Mana[skill]);
	return m_Mana[skill];
}


#ifdef NEW_FORSKYLAND2
//----------------------------------------------------------------------------
// 스킬번호를 이용하여 BP 값을 얻는다.
//----------------------------------------------------------------------------
int CMagicDamage::SkillGetBP(int skill)
{	
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return 0;
	}
	if( m_Damage[skill] == -1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return 0;
	}
//	LogAdd("skill:%d = %d", skill, m_Mana[skill]);
	return m_BrainPower[skill];
}
#endif

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 요구 에너지 능력치를 얻는다.
//----------------------------------------------------------------------------
int CMagicDamage::SkillGetRequireEnergy(int skill)
{	
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	if( m_RequireEnergy[skill] == -1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return -1;
	}
//	LogAdd("skill:%d = %d", skill, m_Mana[skill]);

	return m_RequireEnergy[skill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 사용가능한 클래스를 얻는다.
// Cclass : 캐릭터 종족
//----------------------------------------------------------------------------
#ifdef NEW_SKILL_FORSKYLAND
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3차전직 추가
int CMagicDamage::SkillGetRequireClass(int Cclass, int ChangeUP, int ThirdChangeUp, int skill)
#else
int CMagicDamage::SkillGetRequireClass(int Cclass, int ChangeUP,int skill)
#endif
{	
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	if( Cclass < 0 || Cclass > MAX_CLASSTYPE-1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return -1;
	}

	int requireclass = m_RequireClass[skill][Cclass];

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3차전직 추가
	if( requireclass == 0 )
		return FALSE;
	else if( requireclass == 3 )
	{
		if( ThirdChangeUp > 0 )
			return TRUE;
		else 
			return FALSE;
	}

	if( requireclass > 1 )
	{	// 전직전용이면
		if( requireclass != ChangeUP+1 )
			return FALSE;
	}
#else
	if( requireclass == 0 ) return FALSE;
	if( requireclass > 1 )
	{	// 전직전용이면
		if( requireclass != ChangeUP+1 )
			return FALSE;
	}
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122
	return TRUE;
}
#else
int CMagicDamage::SkillGetRequireClass(int Cclass, int skill)
{	
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	if( Cclass < 0 || Cclass > MAX_CLASSTYPE-1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return -1;
	}
	return m_RequireClass[skill][Cclass];
}
#endif


int CMagicDamage::GetSkillAttr(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		return -1;
	}
	
	return m_Attr[skill];
}

BOOL CMagicDamage::CheckSkillAttr(int skill, int attr)
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{	
		return FALSE;
	}
	
	return m_Attr[skill];
}



#ifdef DARKLORD_WORK
int CMagicDamage::GetskillRequireLeadership(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	if( m_RequireLeadership[skill] == -1 ) 
	{
		LogAdd(lMsg.Get(458),__FILE__, __LINE__);
		return -1;
	}
//	LogAdd("skill:%d = %d", skill, m_Mana[skill]);

	return m_RequireLeadership[skill];	
}


int CMagicDamage::GetDelayTime(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return 0;
	}
	return m_Delay[skill];	
}


#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812

int CMagicDamage::GetSkillType(int iSkill)			// 스킬의 속성이 물리/마법 인지 아니면 없는지 확인
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return 0;
	}
	return m_iSkillType[iSkill];
}

#endif

#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
int CMagicDamage::SkillGetRequireLevel(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	
	return m_RequireLevel[skill];
}
#endif


#endif


#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
// 직책이 필요한지 검사
BOOL CMagicDamage::CheckStatus( INT iSkill, INT iGuildStatus )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return FALSE;
	}

	if( m_iSkillUseType[iSkill] == SKILL_USETYPE_NORMAL )
		return TRUE;

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬 사용시 직책검사할 때 처리
	if( g_MasterSkillSystem.CheckMasterLevelSkill( iSkill ) )
		return TRUE;
#endif
	
	if( iGuildStatus == G_MASTER )
	{
		if( m_iRequireStatus[iSkill][SKILL_USABLE_STATUS_GUILDMASTER] == SKILL_USETYPE_STATUS )
			return TRUE;
	}
	else if( iGuildStatus == G_SUB_MASTER )
	{
		if( m_iRequireStatus[iSkill][SKILL_USABLE_STATUS_SUBMASTER] == SKILL_USETYPE_STATUS )
			return TRUE;
	}
	else if( iGuildStatus == G_BATTLE_MASTER )
	{
		if( m_iRequireStatus[iSkill][SKILL_USABLE_STATUS_BATTLEMASTER] == SKILL_USETYPE_STATUS )
			return TRUE;
	}

	return FALSE;
}

// Brand Of Skill 이 필요한지 검사
BOOL CMagicDamage::CheckBrandOfSkill( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return FALSE;
	}

	// Brand Of Skill 이 필요함
	if( m_iSkillBrand[iSkill] > 0 )
		return TRUE;

	return FALSE;
}

// KillCount가 맞는지 체크 ( 0 >= 킬카운트가 충분함. 여분을 반환, 0 < 킬카운트가 부족함. 부족분을 음수로 표현 )
INT CMagicDamage::CheckKillCount( INT iSkill, INT iKillCount )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return (iKillCount - m_iKillCount[iSkill]);
}

#endif

#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
int CMagicDamage::GetSkillDistance(int skill)
{
	if( skill < 0 || skill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	
	return m_Distance[skill];
}
#endif //MODIFY_SKILL_CHECK_DISTANCE_20060413

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
//----------------------------------------------------------------------------
// 스킬번호를 이용하여 필요 마스터레벨 포인트를 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetRequireMLPoint( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return m_nRequireMLPoint[iSkill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 필요힘을 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetRequireStrength( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return m_iRequireStr[iSkill];
}
//----------------------------------------------------------------------------
// 스킬번호를 이용하여 필요민첩을 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetRequireDexterity( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return	m_iRequireDex[iSkill];			// 필요민첩
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 선행스킬 번호를 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetBrandOfSkill( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return m_iSkillBrand[iSkill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 적용 대상 정보를 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetSkillUseType( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return m_iSkillUseType[iSkill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 스킬 레벨 정보를 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetSkillLevel( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	
	// 스크립트 파일에서 읽은 레벨 정보를 돌려준다.
	// 요구레벨은 따로 정의되어 있다.
	return m_rLevel[iSkill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 랭크 그룹 정보를 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetSkillGroup( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return m_iSkillGroup[iSkill];
}

//----------------------------------------------------------------------------
// 스킬번호를 이용하여 랭크 정보를 얻는다.
//----------------------------------------------------------------------------
INT CMagicDamage::GetSkillRank( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return m_iSkillRank[iSkill];
}

#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122