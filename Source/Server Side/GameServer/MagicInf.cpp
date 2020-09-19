// MagicInf.cpp: implementation of the CMagicInf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\public.h"
#include "MagicDamage.h"
#include "MagicInf.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAX_MAGICINF		255

CMagicInf	DefMagicInf[MAX_MAGICINF];

/*
void MagicInfInit()
{
	for( int n=0; n<MAX_MAGICINF; n++)
	{
		DefMagicInf[n].Clear();
	}
}
*/

CMagicInf::CMagicInf()
{
	Clear();	
}

CMagicInf::~CMagicInf()
{
	m_DamageMin = 0;
	m_DamageMax = 0;
}

void CMagicInf::Clear()
{
	m_Skill = 0xFF;
	m_Level = 0;
}

BOOL CMagicInf::IsMagic()
{
	if( m_Skill == 0xFF ) return FALSE;
	return TRUE;
}

// �ε��� ��ȣ(�������� �ε��� ��ȣ)�� ��ų ��ȣ(���� ���۹�ȣ)�� ��´�.
int GetSkillNumberInex(int type, int Index, int level)
{
	int skillnumber=-1;

	if( type == 15 )	// ����
	{
		if( Index >= 0 && Index < 16 )
		{	// ������ ��ų�ε����� ���󰣴�
			skillnumber = AT_SKILL_POISON+Index;
		}
#ifdef ADD_SKILL_WITH_COMBO
		else
		{
			if( Index == 16 )
			{
				skillnumber = AT_SKILL_EXPPOISON;
			}
			else if( Index == 17 )
			{
				skillnumber = AT_SKILL_EXPICE;
			}
			else if( Index == 18 )
			{
				skillnumber = AT_SKILL_EXPHELL;
			}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ������ ��ų �߰�
			else if( Index == 19 )
			{
				skillnumber = AT_SKILL_CHAINLIGHTNING;
			}
			else if( Index == 20)
			{
				skillnumber = AT_SKILL_DRAINLIFE;
			}
#ifdef SEASON4_PATCH_GROOVING
			// --->
			// ���� 4.0 �߰� ��ų  grooving 2008/04/24 : ����Ʈ�� ��ũ
			// ���� ����Ʈ�׿��� ���� ����Ʈ�� ��ũ�� ����
			else if( Index == 21 )
			{
				skillnumber = AT_SKILL_LIGHTING_SHOCK;
			}
			// <---			
#else
			else if( Index == 21 )
			{
				skillnumber = AT_SKILL_LIGHTNINGORB;
			}
#endif

			else if( Index == 22 )
			{
				skillnumber = AT_SKILL_THORNS;
			}
#ifdef SEASON4_PATCH_GROOVING
			// --->
			// ���� 4.0 �߰� ��ų  grooving 2008/04/24 : ����Ŀ
			else if( Index == 23 )
			{
				skillnumber = AT_SKILL_BERSERKER;
			}
			// <---
#endif
			//else if( Index == 23 )
			//{
				//skillnumber = AT_SKILL_EXPHELL;
			//}
			else if( Index == 24 )
			{
				skillnumber = AT_SKILL_SLEEP;
			}
			else if( Index == 25 )
			{
				skillnumber = AT_SKILL_BLIND;
			}
			//else if( Index == 26 )
			//{
			//	skillnumber = AT_SKILL_EXPHELL;
			//}
			//else if( Index == 27 )
			//{
			//	skillnumber = AT_SKILL_EXPHELL;
			//}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	
			
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// ��ũ�Ͻ� �̳ʺ��̼Ǿ����� ���
			else if( Index == 26 )
			{
				skillnumber = AT_SKILL_WEAKNESS;
			}
			else if( Index == 27 )
			{
				skillnumber = AT_SKILL_ENERVATION;
			}
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111
			
#ifdef ADD_SEASON4_NEW_SKILL_20080522		// �űԽ�ų�����ۿ� ���� ��ų ��ȣ(index = 15)
			else if( Index == 28 )	skillnumber = AT_SKILL_MAGICPOWRER;		// ���� ����
			else if( Index == 29 )	skillnumber = AT_SKILL_GIGANTICSTORM;	// �Ⱓƽ ����
#endif	// ADD_SEASON4_NEW_SKILL_20080522
		}
#endif
	}
	else if( type == 12 )
	{
		switch( Index )
		{
		case 7  :   skillnumber = AT_SKILL_WHEEL;   break;			// ȸ��������
		case 8  :	skillnumber = AT_SKILL_HEALING; break;			// ġ��
		case 9  :	skillnumber = AT_SKILL_DEFENSE; break;			// �������
		case 10 :	skillnumber = AT_SKILL_ATTACK;  break;			// ���ݷ����

#ifdef NEW_SKILL_FORSKYLAND
		// ��� ���ο� ��ų		
		case 12 :   skillnumber = AT_SKILL_BLOWOFFURY;  break;		// �г����ϰ�
		case 13 :	skillnumber = AT_SKILL_KNIGHTSPEAR;  break;		// ����â
		case 14 :	skillnumber = AT_SKILL_KNIGHTADDLIFE;  break;	// ��������	
		case 19 :	skillnumber = AT_SKILL_STRIKE;  break;			// �ϴ���(��Ʈ����ũ)
		//case 16 :	skillnumber = AT_SKILL_SWORDAURORA;  break;

		// ���� ���ο� ��ų
		case 17 :	skillnumber = AT_SKILL_PENETRATION;  break;		// ����
		case 18 :	skillnumber = AT_SKILL_ELFHARDEN;  break;		// ��ȭ		

		// ���˻� ���ο� ��ų
		case 16 :	skillnumber = AT_SKILL_DEFENSEDOWN;  break;		// �����
#endif
			
#ifdef DARKLORD_WORK
		//case 20 :	skillnumber = AT_SKILL_IMPROVE_AG_REFILL;  break;	
		case 21 :	skillnumber = AT_SKILL_FIREBURST;  break;				// ���̾����Ʈ
		case 22 :	skillnumber = AT_SKILL_RECALL_PARTY;  break;			// ��Ƽ��ȯ
		case 23 :	skillnumber = AT_SKILL_ADD_CRITICALDAMAGE;  break;		// ũ��Ƽ�� ������ ����
		case 24 :	skillnumber = AT_SKILL_ELECTRICSPARK;  break;			// �Ϸ�Ʈ�� ����ũ
#endif

//#ifdef ADD_SKILL_WITH_COMBO
//		case 20 :	skillnumber = AT_SKILL_4CROSSBOW;  break;		// �����
//#endif	
		case 11 : 
					skillnumber = AT_SKILL_CALLMON1+level;			// ���� ��ȯ
			break;
				 
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾� ��ũ�� ��ũ�� ��ȣ ����
		case 35:
			skillnumber = AT_SKILL_FIRESCREAM;		
			break;
#endif

#ifdef ADD_SEASON4_NEW_SKILL_20080522		// �űԽ�ų�����ۿ� ���� ��ų ��ȣ(index = 12)
		case 44:	skillnumber = AT_SKILL_DESTRUCTION;		break;	// �ı��� �ϰ�
		case 45:	skillnumber = AT_SKILL_MULTISHOT;		break;	// ��Ƽ��
		case 46:	skillnumber = AT_SKILL_RECOVERY;		break;	// ȸ��
		case 47:	skillnumber = AT_SKILL_FLAMESTRIKE;		break;	// �÷��� ��Ʈ����ũ
		case 48:	skillnumber = AT_SKILL_CHAOTICDESAIR;	break;	// ī��ƽ ����
#endif	// ADD_SEASON4_NEW_SKILL_20080522
		}
	}
	return skillnumber;
}

// ����å���� ��ﶧ ����
int CMagicInf::Set(BYTE aType, BYTE aIndex, BYTE aLevel)
{
	int damage;

	if( m_Skill != 0xFF ) 
	{
		LogAdd(lMsg.Get(459),__FILE__, __LINE__);
		return -1;
	}
	m_Level = aLevel;
	// ���� ������ �˷�����Ѵ�.
	//if( aIndex >= 0 && aIndex < 16 )
	//{
		m_Skill = GetSkillNumberInex(aType, aIndex, aLevel);
	if( m_Skill < 0 ) m_Skill = AT_SKILL_POISON;
	//}
	//else 
	/*{
		m_Skill = AT_SKILL_POISON;
		LogAdd("�ε��� ���� : (%s %d)", __FILE__, __LINE__);
	} */
	
	damage      = MagicDamageC.SkillGet(m_Skill);
	m_DamageMin = damage;
	m_DamageMax = damage+damage/2;

	return m_Skill;
}

int CMagicInf::GetDamage()
{
	int damage=0;

	if( m_Skill == 0xFF ) {
		LogAdd(lMsg.Get(458), __FILE__, __LINE__);
		return 0;
	}
	
	int subd = m_DamageMax-m_DamageMin;
	__try
	{
		damage = m_DamageMin+(rand()%(+1));
	}
	__except( subd = 1, EXCEPTION_EXECUTE_HANDLER )
	{
		LogAdd(lMsg.Get(111), __FILE__, __LINE__);
	}
	return damage;
}


#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// CMagicInf::Set(...) Type ����
int CMagicInf::Set(int aSkill, BYTE aLevel)
#else
int CMagicInf::Set(BYTE aSkill, BYTE aLevel)
#endif
{
	int damage;

	if( m_Skill != 0xFF ) 
	{
		//LogAdd(lMsg.Get(459),__FILE__, __LINE__);
		return -1;
	}
	m_Skill = aSkill;
	m_Level = aLevel;
	
	damage = MagicDamageC.SkillGet(m_Skill);
	m_DamageMin = damage;
	m_DamageMax = damage+damage/2;

	return m_Skill;
}

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122		// UpdateMasterSkill(...)
// �����ͷ��� ��ų ���� �����ϱ�
int CMagicInf::UpdateMasterSkill( int iSkill, BYTE btLevel )
{
	int damage;

	if( m_Skill != 0xFF ) 
	{
		m_Skill = iSkill;
		m_Level = btLevel;
		
		damage		= MagicDamageC.SkillGet(m_Skill);
		m_DamageMin = damage;
		m_DamageMax = damage+damage/2;

		return m_Skill;		
	}

	LogAdd( "[MasterSkill] [ERROR!!] Fail - Update Master Skill:%d, Level:%d", iSkill, btLevel );
	return -1;	

}
#endif

//----------------------------------------------------------------------------
// ���� ������ 3����Ʈ ������ �ٲ۴�.
//----------------------------------------------------------------------------
void MagicByteConvert(LPBYTE buf, CMagicInf Magici[], int maxmagic)
{
	int n=0;

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// DB�� �������� �����ϱ�
	// DB�� ���������� ������ �� 3BYTE�� �ϳ��� ��ų�� �ȴ�.
	// ù ��° BYTE : ��ų ��ȣ
	// �� ��° BYTE : 5bit : ��ų ����(32���� ����)
	//				  3bit : ��ų��ȣ�� 1BYTE ������ ���� �� ���� ���⿡ �ʰ��� BYTE ������ �����Ѵ�.
	//						�� ��° BYTE�� ����ؼ� ���� ��ų��ȣ�� ���� �� �ִ�.(iExpendSkillCount)
	// �� ��° BYTE : 255���� �Ѵ� ��ȣ�� �ɰ��� ���⿡ �ִ´�.(iExpendSkillNumber)
	// -> DBȮ���̳� ��ȯ���� ��ų �߰� ����. ( 255 * 8 )

	int iExpendSkillCount;
	int iExpendSkillNumber;
	int iExpendSkillExtraNumber;
	for( int index=0; index < maxmagic; index++ )
	{
		// �ʱ�ȭ
		iExpendSkillNumber = Magici[index].m_Skill;

		iExpendSkillCount = 0;		
		iExpendSkillExtraNumber = 0;

		// ��ų��ȣ ��ȯ	: 255(0xFF)�� ��ų �������� ���� ������ ���ȴ�.
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų ���� ���(DB) �߰�&����
		if( Magici[index].m_Skill > 255 * 3 )
		{
			iExpendSkillNumber	= 255;
			iExpendSkillCount	= 3;
			iExpendSkillExtraNumber = Magici[index].m_Skill - ( iExpendSkillNumber * 3 );
		}
		else if( Magici[index].m_Skill > 255 * 2 )
		{
			iExpendSkillNumber	= 255;
			iExpendSkillCount	= 2;
			iExpendSkillExtraNumber = Magici[index].m_Skill - ( iExpendSkillNumber * 2 );
		}
		else
#endif		
		if( Magici[index].m_Skill > 255 )
		{
			iExpendSkillNumber	= 255;
			iExpendSkillCount	= 1;
			iExpendSkillExtraNumber = Magici[index].m_Skill - iExpendSkillNumber;
		}

		buf[n]  = (BYTE)iExpendSkillNumber;
		n++;
		buf[n]  = Magici[index].m_Level<<3;			// Level	  : 5bit
		buf[n] |= iExpendSkillCount&0x07;			// iExpendSkillCount : 3bit
		n++;
		buf[n] = iExpendSkillExtraNumber;									
		n++;
	}
#else
	// type , index, Level, Dur, Special 
	//   4  ,   4  ,   5,   3,    8
	for( int index=0; index<maxmagic; index++)
	{
		buf[n]  = (BYTE)Magici[index].m_Skill;
		n++;
		buf[n]  = Magici[index].m_Level<<3;			// Level	  : 5bit
		//buf[n] |= Magici[index].&0x07;	// Durability : 3bit
		n++;
		buf[n] = 0;									// special    : 8bit
		n++;
	}
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122
}
