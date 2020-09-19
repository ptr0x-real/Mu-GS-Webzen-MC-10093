// MagicDamage.cpp: implementation of the CMagicDamage class.
// ����(��ų)������ ������ ���� �����ϴ� Ŭ���� 
// ���� : ��ų��ȣ�� �ߺ��� �� ����.
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

// Skill.txt �� �о�´�.
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
	int	 bp = 0;	// AG�Ŀ�
	int  dis;
	int  rEnergy;
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int  iSkillType;
#endif

#ifdef DARKLORD_WORK
	int  rLeadership;
	int	 rDelay;
#endif

	int  attr;		// �Ӽ�

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	int	iSkillUseType;	// ��å ��ų����, �⺻ ��ų���� ���
	int	iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_STATUS] = {0,};
#endif

	BYTE  RequireClass[MAX_CLASSTYPE];
	
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	short		nSkillRank;				// ��ų��ũ	
	short		nSkillGroup;			// ��ų�׷�	
	short		nRequireMLPoint;		// �ʿ丶���ͷ��� ����Ʈ
	short		nHP;					// �Ҹ�ü��	
	short		nSD;					// �Ҹ�SD	
	WORD		wKeepTime;				// ���ӽð�	
	short		nRequireStr;			// �ʿ���	
	short		nRequireDex;			// �ʿ��ø
	BYTE		btIconNum;				// ������ ��ȣ(Ŭ���̾�Ʈ��)
#endif

#ifdef ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515
	int			nSkillType;				// ��ųŸ��
#endif // ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			//   ��ȣ	    �̸�	 �ʿ䷹��  ������ ��븶����	�Ÿ�
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// skil ��� �ű� Ŭ���� �߰�
			Token = (*GetToken)(); RequireClass[5]	= (int)TokenNumber;
#else
			Token = (*GetToken)();
#endif	

			Token = (*GetToken)(); //Rage Fighter Bypass
			
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// skill(kor).txt ���� �б�
			Token = (*GetToken)();		nSkillRank			= (short)TokenNumber;	// ��ų��ũ	
			Token = (*GetToken)();		nSkillGroup			= (short)TokenNumber;	// ��ų�׷�	
			Token = (*GetToken)();		nRequireMLPoint		= (short)TokenNumber;	// �ʿ丶���ͷ��� ����Ʈ
			Token = (*GetToken)();		nHP					= (short)TokenNumber;	// �Ҹ�ü��	
			Token = (*GetToken)();		nSD					= (short)TokenNumber;	// �Ҹ�SD	
			Token = (*GetToken)();		wKeepTime			= (WORD)TokenNumber;	// ���ӽð�	
			Token = (*GetToken)();		nRequireStr			= (short)TokenNumber;	// �ʿ���	
			Token = (*GetToken)();		nRequireDex			= (short)TokenNumber;	// �ʿ��ø
			Token = (*GetToken)();		btIconNum			= (BYTE)TokenNumber;	// ������ ��ȣ
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
			Token = (*GetToken)();		nSkillType			= (int)TokenNumber;	// ��ųŸ��
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
			
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// �����ͷ��� ��ų ���� ����
			SetMasterLevelSkillInfo( number, nSkillRank, nSkillGroup, nRequireMLPoint, nHP, nSD, wKeepTime, nRequireStr, nRequireDex );
#endif
				DefMagicInf[number].Set(number, 0);
		}
	}
	fclose(SMDFile);

	LogAdd(lMsg.Get(456), filename);
}

// �ؿ����� ���ؼ� ��ų�̸��� ��� �´�
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
// Skill.txt �� �о�´�.
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
	int	 bp = 0;	// AG�Ŀ�
	int  dis;
	int  rEnergy;
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int  iSkillType;
#endif

#ifdef DARKLORD_WORK
	int		rLeadership;
	int		rDelay;
#endif

	int  attr;		// �Ӽ�

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	int iSkillUseType;
	int	iSkillBrand;
	int iKillCount;
	
	int RequireStatus[MAX_STATUS] = {0,};
#endif
	
	BYTE  RequireClass[MAX_CLASSTYPE];

#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	int		iSkillRank;				// ��ų��ũ	
	int		iSkillGroup;			// ��ų�׷�	
	short	nRequireMasterLevel;	// �ʿ丶���ͷ���	
	int		iHP;					// �Ҹ�ü��	
	int		iSD;					// �Ҹ�SD	
	WORD	wKeepTime;				// ���ӽð�	
	int		iRequireStr;			// �ʿ���	
	int		iRequireDex;			// �ʿ��ø
	BYTE	btIconNum;				// ������ ��ȣ
#endif

#ifdef ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515
	int		nSkillType;				// ��ųŸ��
#endif // ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515

	while( true )
	{	
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			//   ��ȣ	    �̸�	 �ʿ䷹��  ������ ��븶����	�Ÿ�
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// skil ��� �ű� Ŭ���� �߰�
			Token = WzMemScript.GetToken(); RequireClass[5]	= WzMemScript.GetNumber();			
#endif

#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// skill(kor).txt ���� �б� (AUTH)
			Token = WzMemScript.GetToken();		iSkillRank			= WzMemScript.GetNumber();	// ��ų��ũ	
			Token = WzMemScript.GetToken();		iSkillGroup			= WzMemScript.GetNumber();	// ��ų�׷�	
			Token = WzMemScript.GetToken();		nRequireMasterLevel	= WzMemScript.GetNumber();	// �ʿ丶���ͷ���	
			Token = WzMemScript.GetToken();		iHP					= WzMemScript.GetNumber();	// �Ҹ�ü��	
			Token = WzMemScript.GetToken();		iSD					= WzMemScript.GetNumber();	// �Ҹ�SD	
			Token = WzMemScript.GetToken();		wKeepTime			= WzMemScript.GetNumber();	// ���ӽð�	
			Token = WzMemScript.GetToken();		iRequireStr			= WzMemScript.GetNumber();	// �ʿ���	
			Token = WzMemScript.GetToken();		iRequireDex			= WzMemScript.GetNumber();	// �ʿ��ø
			Token = WzMemScript.GetToken();		btIconNum			= WzMemScript.GetNumber();	// ������ ��ȣ
#endif

#ifdef ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515
			Token = WzMemScript.GetToken();		nSkillType			= WzMemScript.GetNumber();	// ��ųŸ��
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
			
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// �����ͷ��� ��ų ���� ���� (AUTH)
			SetMasterLevelSkillInfo( number, iSkillRank, iSkillGroup, nRequireMasterLevel, iHP, iSD, wKeepTime, iRequireStr, iRequireDex );
#endif
			
			DefMagicInf[number].Set(number, 0);
		}
	}

	LogAdd(lMsg.Get(456), "Skill");
}

// �ؿ����� ���ؼ� ��ų�̸��� ��� �´�
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
// Skil.txt �� �о�´�.
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
	int	 bp = 0;	// AG�Ŀ�
	int  dis;
	int  rEnergy;
	BYTE  RequireClass[MAX_CLASSTYPE];

	while( true )
	{
		
		Token = Encoder.GetToken();
		if(Token == T_END) break;
		if(Token == T_NUMBER)
		{
			//   ��ȣ	    �̸�	 �ʿ䷹��  ������ ��븶����	�Ÿ�
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

// �ؿ����� ���ؼ� ��ų�̸��� ��� �´�
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
// Ȯ�� �� ������ ��ų ������ �����Ѵ�. (RKFKA)
//----------------------------------------------------------------------------
void CMagicDamage::SetMasterLevelSkillInfo( int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		MsgBox(lMsg.Get(457),__FILE__, __LINE__);
		return;
	}

	// ��ų�� ���ٸ�.
	if( m_Damage[iSkill] == -1 ) 
	{
		MsgBox(lMsg.Get(562),__FILE__, __LINE__);
		return;
	}

	m_iSkillRank[iSkill]			= iSkillRank;			// ��ų��ũ	
	m_iSkillGroup[iSkill]			= iSkillGroup;			// ��ų�׷�	
	m_nRequireMLPoint[iSkill]		= iRequireMLPoint;		// �ʿ丶���ͷ�������Ʈ	
	m_iHP[iSkill]					= iHP;					// �Ҹ�ü��	
	m_iSD[iSkill]					= iSD;					// �Ҹ�SD	
	m_wKeepTime[iSkill]				= wKeepTime;			// ���ӽð�	
	m_iRequireStr[iSkill]			= iRequireStr;			// �ʿ���	
	m_iRequireDex[iSkill]			= iRequireDex;			// �ʿ��ø

}
#endif	// EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122

//----------------------------------------------------------------------------
// Ȯ�� �� ��ų���� �����Ѵ�. (apple)
//----------------------------------------------------------------------------
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
void CMagicDamage::SetEx( int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int *pReqStatus  )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		MsgBox(lMsg.Get(457),__FILE__, __LINE__);
		return;
	}

	// ��ų�� ���ٸ�.
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
// ��ų���� �����Ѵ�.
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
	if( skill == AT_SKILL_WHEEL )				m_RequireLevel[skill] = 80;			// ȸ��������
	else if( skill == AT_SKILL_BLOWOFFURY )		m_RequireLevel[skill] = 170;		// �г��� �ϰ�
	else if( skill == AT_SKILL_STRIKE )			m_RequireLevel[skill] = 160;		// ��ο�
	else if( skill == AT_SKILL_KNIGHTSPEAR )	m_RequireLevel[skill] = 28;		// â���
	else if( skill == AT_SKILL_KNIGHTADDLIFE )	m_RequireLevel[skill] = 120;		// ����������			
	else if( skill == AT_SKILL_PENETRATION )	m_RequireLevel[skill] = 130;		// ����
#endif
	
	if( skill == 30 ) m_RequireEnergy[skill] = 30;
	else if( skill == 31 ) m_RequireEnergy[skill] = 60;
	else if( skill == 32 ) m_RequireEnergy[skill] = 90;
	else if( skill == 33 ) m_RequireEnergy[skill] = 130;
	else if( skill == 34 ) m_RequireEnergy[skill] = 170;
	else if( skill == 35 ) m_RequireEnergy[skill] = 210;
	else if( skill == 36 ) m_RequireEnergy[skill] = 300;
	else if( skill == AT_SKILL_WHEEL )			m_RequireEnergy[skill] = 0;		// ȸ��������
	else if( skill == AT_SKILL_BLOWOFFURY )		m_RequireEnergy[skill] = 0;		// �г��� �ϰ�
	else if( skill == AT_SKILL_STRIKE )			m_RequireEnergy[skill] = 0;		// ��ο�
	else if( skill == AT_SKILL_KNIGHTSPEAR )	m_RequireEnergy[skill] = 0;		// â���
	else if( skill == AT_SKILL_KNIGHTADDLIFE )	m_RequireEnergy[skill] = 0;		// ����������
	else if( skill == AT_SKILL_KNIGHTDINORANT ) m_RequireEnergy[skill] = 0;		// ���̵� ��
	else if( skill == AT_SKILL_DEFENSEDOWN )	m_RequireEnergy[skill] = 0;		// �������
	else if( skill == AT_SKILL_ELFHARDEN )		m_RequireEnergy[skill] = 0;		// ���̽��ַο�
	else if( skill == AT_SKILL_PENETRATION )	m_RequireEnergy[skill] = 0;		// ����
	else if( skill == AT_SKILL_CROSSBOW )	m_RequireEnergy[skill] = 0;			// ���� �ٹ�
	else if( skill == AT_SKILL_ENERGYBALL )	m_RequireEnergy[skill] = 0;			// ��������
	else if( skill == AT_SKILL_BLOCKING )	m_RequireEnergy[skill] = 0;			// ��� ��ų
	else if( skill == AT_SKILL_SWORD1 )	m_RequireEnergy[skill] = 0;				// �������
	else if( skill == AT_SKILL_SWORD2 )	m_RequireEnergy[skill] = 0;				// ���
	else if( skill == AT_SKILL_SWORD3 )	m_RequireEnergy[skill] = 0;				// �÷�ġ��
	else if( skill == AT_SKILL_SWORD4 )	m_RequireEnergy[skill] = 0;				// ����ġ��
	else if( skill == AT_SKILL_SWORD5 )	m_RequireEnergy[skill] = 0;				// ����
	else if( skill == AT_SKILL_BOW )	m_RequireEnergy[skill] = 0;				// �Ⱦ��� ���� ����	
	else if( skill == AT_SKILL_SWORD6 )	m_RequireEnergy[skill] = 0;				// ������ε�
#ifdef DARKLORD_WORK
	else if( skill == AT_SKILL_SPEAR )	m_RequireEnergy[skill] = 0;				// ������ε�
#endif
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	else if( skill == AT_SKILL_RUSH )			m_RequireEnergy[skill] = 0;		// ���̵� ����Ʈ
	else if( skill == AT_SKILL_JAVALIN )		m_RequireEnergy[skill] = 0;		// �ҿ� ������
	else if( skill == AT_SKILL_DEEPIMPACT )		m_RequireEnergy[skill] = 0;		// ���� ����
	else if( skill == AT_SKILL_ONE_FLASH )		m_RequireEnergy[skill] = 0;		// ����� ���˻�
	else if( skill == AT_SKILL_DEATH_CANNON )	m_RequireEnergy[skill] = 0;		// ������ ���˻�
	else if( skill == AT_SKILL_SPACE_SPLIT )	m_RequireEnergy[skill] = 0;		// ��ũ�ε� ���� ��ų
#endif
#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	else if( skill == AT_SKILL_STUN )				m_RequireEnergy[skill] = 0;		// ����
	else if( skill == AT_SKILL_REMOVAL_STUN )		m_RequireEnergy[skill] = 0;		// ��������
	else if( skill == AT_SKILL_ADD_MANA )			m_RequireEnergy[skill] = 0;		// ��������
	else if( skill == AT_SKILL_INVISIBLE )			m_RequireEnergy[skill] = 0;		// ����
	else if( skill == AT_SKILL_REMOVAL_INVISIBLE )	m_RequireEnergy[skill] = 0;		// ��������
	else if( skill == AT_SKILL_REMOVAL_MAGIC )		m_RequireEnergy[skill] = 0;		// ��������
#endif
#ifdef ADD_ITEM_FENRIR_01_20051110
	else if( skill == AT_SKILL_FENRIR_ATTACK )		m_RequireEnergy[skill] = 0;	 // �渱 ��ų
#endif

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	else if( skill == AT_SKILL_INFINITY_ARROW )		m_RequireEnergy[skill] = 0;		// ����� ���Ǵ�Ƽ �ַο�
#endif
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601		// CMagicDamage::Set(...) ���̾� ��ũ�� �䱸������ ����
	else if( skill == AT_SKILL_FIRESCREAM )			m_RequireEnergy[skill] = 0;		// ��ũ�ε� ���̾ũ��
	else if( skill == AT_SKILL_EXPLOSION )			m_RequireEnergy[skill] = 0;		// ��ũ�ε� ����
#endif
	
#ifdef MODIFY_DARKLORD_EARTHQUAKE_REQENERGY_BUGFIX_20060914
	else if( skill == AT_SKILL_DARKHORSE_ATTACK )	m_RequireEnergy[skill] = 0;		// ��ũ�ε� �����ũ 
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	else if( skill == AT_SKILL_DRAINLIFE)			m_RequireEnergy[skill] = 0;		// �巹�ζ�����
	else if( skill == AT_SKILL_CHAINLIGHTNING)		m_RequireEnergy[skill] = 0;		// ü�ζ���Ʈ��
	else if( skill == AT_SKILL_LIGHTNINGORB)			m_RequireEnergy[skill] = 0;		// ����Ʈ�׿���

	else if( skill == AT_SKILL_THORNS)				m_RequireEnergy[skill] = 0;		// ����

	else if( skill == AT_SKILL_SLEEP)				m_RequireEnergy[skill] = 0;		// ����
	else if( skill == AT_SKILL_BLIND)				m_RequireEnergy[skill] = 0;		// ����ε�

	else if( skill == AT_SKILL_EXPLOSION_2)			m_RequireEnergy[skill] = 0;		// �ͽ��÷���
	else if( skill == AT_SKILL_REQUIEM)				m_RequireEnergy[skill] = 0;		// ������
#endif
	
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
	else if( skill == AT_SKILL_WEAKNESS)			m_RequireEnergy[skill] = 0;		// ��ũ�Ͻ�
	else if( skill == AT_SKILL_ENERVATION)			m_RequireEnergy[skill] = 0;		// �̳ʺ��̼�
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
	else if( skill == AT_SKILL_PULLUTION )			m_RequireEnergy[skill] = 0;		// �޷��
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
	
#ifdef SEASON4_PATCH_GROOVING
	// --->
	// ���� 4.0 �߰� ��ų  grooving 2008/04/24
	else if( skill == AT_SKILL_LIGHTING_SHOCK)		m_RequireEnergy[skill] = 0;		// ����Ʈ�� ��ũ
	else if( skill == AT_SKILL_BERSERKER)			m_RequireEnergy[skill] = 0;		// ����Ŀ
	// <---
#endif
	
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// �䱸 ������ ����
	else if( skill == AT_SKILL_DESTRUCTION )		m_RequireEnergy[skill] = 0;		// �ı��� �ϰ�
	else if( skill == AT_SKILL_MAGICPOWRER )		m_RequireEnergy[skill] = 0;		// ���� ����
	else if( skill == AT_SKILL_RECOVERY )			m_RequireEnergy[skill] = 0;		// ȸ��
	else if( skill == AT_SKILL_MULTISHOT )			m_RequireEnergy[skill] = 0;		// ��Ƽ��
	else if( skill == AT_SKILL_FLAMESTRIKE )		m_RequireEnergy[skill] = 0;		// �÷��� ��Ʈ����ũ
	else if( skill == AT_SKILL_GIGANTICSTORM )		m_RequireEnergy[skill] = 0;		// �Ⱓƽ ����
	else if( skill == AT_SKILL_CHAOTICDESAIR )		m_RequireEnergy[skill] = 0;		// ī��ƽ ���̾�
#endif	// ADD_SEASON4_NEW_SKILL_20080522
}

//----------------------------------------------------------------------------
// ���ϴ� ��ų�� ������ ���� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� ������ ���� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� ������ ���� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� BP ���� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� �䱸 ������ �ɷ�ġ�� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� ��밡���� Ŭ������ ��´�.
// Cclass : ĳ���� ����
//----------------------------------------------------------------------------
#ifdef NEW_SKILL_FORSKYLAND
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
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

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
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
	{	// ���������̸�
		if( requireclass != ChangeUP+1 )
			return FALSE;
	}
#else
	if( requireclass == 0 ) return FALSE;
	if( requireclass > 1 )
	{	// ���������̸�
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

int CMagicDamage::GetSkillType(int iSkill)			// ��ų�� �Ӽ��� ����/���� ���� �ƴϸ� ������ Ȯ��
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
// ��å�� �ʿ����� �˻�
BOOL CMagicDamage::CheckStatus( INT iSkill, INT iGuildStatus )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return FALSE;
	}

	if( m_iSkillUseType[iSkill] == SKILL_USETYPE_NORMAL )
		return TRUE;

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų ���� ��å�˻��� �� ó��
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

// Brand Of Skill �� �ʿ����� �˻�
BOOL CMagicDamage::CheckBrandOfSkill( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL-1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return FALSE;
	}

	// Brand Of Skill �� �ʿ���
	if( m_iSkillBrand[iSkill] > 0 )
		return TRUE;

	return FALSE;
}

// KillCount�� �´��� üũ ( 0 >= ųī��Ʈ�� �����. ������ ��ȯ, 0 < ųī��Ʈ�� ������. �������� ������ ǥ�� )
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
// ��ų��ȣ�� �̿��Ͽ� �ʿ� �����ͷ��� ����Ʈ�� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� �ʿ����� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� �ʿ��ø�� ��´�.
//----------------------------------------------------------------------------
INT CMagicDamage::GetRequireDexterity( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}

	return	m_iRequireDex[iSkill];			// �ʿ��ø
}

//----------------------------------------------------------------------------
// ��ų��ȣ�� �̿��Ͽ� ���ེų ��ȣ�� ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� ���� ��� ������ ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� ��ų ���� ������ ��´�.
//----------------------------------------------------------------------------
INT CMagicDamage::GetSkillLevel( INT iSkill )
{
	if( iSkill < 0 || iSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return -1;
	}
	
	// ��ũ��Ʈ ���Ͽ��� ���� ���� ������ �����ش�.
	// �䱸������ ���� ���ǵǾ� �ִ�.
	return m_rLevel[iSkill];
}

//----------------------------------------------------------------------------
// ��ų��ȣ�� �̿��Ͽ� ��ũ �׷� ������ ��´�.
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
// ��ų��ȣ�� �̿��Ͽ� ��ũ ������ ��´�.
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