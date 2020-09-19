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

// 인덱스 번호(아이템의 인덱스 번호)로 스킬 번호(마법 동작번호)를 얻는다.
int GetSkillNumberInex(int type, int Index, int level)
{
	int skillnumber=-1;

	if( type == 15 )	// 법서
	{
		if( Index >= 0 && Index < 16 )
		{	// 법서는 스킬인덱스를 따라간다
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
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 양피지 스킬 추가
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
			// 시즌 4.0 추가 스킬  grooving 2008/04/24 : 라이트닝 쇼크
			// 기존 라이트닝오브 에서 라이트닝 쇼크로 변경
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
			// 시즌 4.0 추가 스킬  grooving 2008/04/24 : 버서커
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
			
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// 위크니스 이너베이션양피지 사용
			else if( Index == 26 )
			{
				skillnumber = AT_SKILL_WEAKNESS;
			}
			else if( Index == 27 )
			{
				skillnumber = AT_SKILL_ENERVATION;
			}
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111
			
#ifdef ADD_SEASON4_NEW_SKILL_20080522		// 신규스킬아이템에 대한 스킬 번호(index = 15)
			else if( Index == 28 )	skillnumber = AT_SKILL_MAGICPOWRER;		// 마력 증대
			else if( Index == 29 )	skillnumber = AT_SKILL_GIGANTICSTORM;	// 기간틱 스톰
#endif	// ADD_SEASON4_NEW_SKILL_20080522
		}
#endif
	}
	else if( type == 12 )
	{
		switch( Index )
		{
		case 7  :   skillnumber = AT_SKILL_WHEEL;   break;			// 회오리베기
		case 8  :	skillnumber = AT_SKILL_HEALING; break;			// 치료
		case 9  :	skillnumber = AT_SKILL_DEFENSE; break;			// 방어력향상
		case 10 :	skillnumber = AT_SKILL_ATTACK;  break;			// 공격력향상

#ifdef NEW_SKILL_FORSKYLAND
		// 기사 새로운 스킬		
		case 12 :   skillnumber = AT_SKILL_BLOWOFFURY;  break;		// 분노의일격
		case 13 :	skillnumber = AT_SKILL_KNIGHTSPEAR;  break;		// 유니창
		case 14 :	skillnumber = AT_SKILL_KNIGHTADDLIFE;  break;	// 생명증가	
		case 19 :	skillnumber = AT_SKILL_STRIKE;  break;			// 일대일(스트라이크)
		//case 16 :	skillnumber = AT_SKILL_SWORDAURORA;  break;

		// 요정 새로운 스킬
		case 17 :	skillnumber = AT_SKILL_PENETRATION;  break;		// 관통
		case 18 :	skillnumber = AT_SKILL_ELFHARDEN;  break;		// 석화		

		// 마검사 새로운 스킬
		case 16 :	skillnumber = AT_SKILL_DEFENSEDOWN;  break;		// 벙어감소
#endif
			
#ifdef DARKLORD_WORK
		//case 20 :	skillnumber = AT_SKILL_IMPROVE_AG_REFILL;  break;	
		case 21 :	skillnumber = AT_SKILL_FIREBURST;  break;				// 파이어버스트
		case 22 :	skillnumber = AT_SKILL_RECALL_PARTY;  break;			// 파티소환
		case 23 :	skillnumber = AT_SKILL_ADD_CRITICALDAMAGE;  break;		// 크리티컬 데미지 증가
		case 24 :	skillnumber = AT_SKILL_ELECTRICSPARK;  break;			// 일렉트릭 스파크
#endif

//#ifdef ADD_SKILL_WITH_COMBO
//		case 20 :	skillnumber = AT_SKILL_4CROSSBOW;  break;		// 벙어감소
//#endif	
		case 11 : 
					skillnumber = AT_SKILL_CALLMON1+level;			// 요정 소환
			break;
				 
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// 파이어 스크림 스크롤 번호 리턴
		case 35:
			skillnumber = AT_SKILL_FIRESCREAM;		
			break;
#endif

#ifdef ADD_SEASON4_NEW_SKILL_20080522		// 신규스킬아이템에 대한 스킬 번호(index = 12)
		case 44:	skillnumber = AT_SKILL_DESTRUCTION;		break;	// 파괴의 일격
		case 45:	skillnumber = AT_SKILL_MULTISHOT;		break;	// 멀티샷
		case 46:	skillnumber = AT_SKILL_RECOVERY;		break;	// 회복
		case 47:	skillnumber = AT_SKILL_FLAMESTRIKE;		break;	// 플레임 스트라이크
		case 48:	skillnumber = AT_SKILL_CHAOTICDESAIR;	break;	// 카오틱 디세이
#endif	// ADD_SEASON4_NEW_SKILL_20080522
		}
	}
	return skillnumber;
}

// 마법책으로 배울때 세팅
int CMagicInf::Set(BYTE aType, BYTE aIndex, BYTE aLevel)
{
	int damage;

	if( m_Skill != 0xFF ) 
	{
		LogAdd(lMsg.Get(459),__FILE__, __LINE__);
		return -1;
	}
	m_Level = aLevel;
	// 마법 종류를 알려줘야한다.
	//if( aIndex >= 0 && aIndex < 16 )
	//{
		m_Skill = GetSkillNumberInex(aType, aIndex, aLevel);
	if( m_Skill < 0 ) m_Skill = AT_SKILL_POISON;
	//}
	//else 
	/*{
		m_Skill = AT_SKILL_POISON;
		LogAdd("인덱싱 오류 : (%s %d)", __FILE__, __LINE__);
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


#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// CMagicInf::Set(...) Type 변경
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
// 마스터레벨 스킬 레벨 갱신하기
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
// 마법 정보를 3바이트 정보로 바꾼다.
//----------------------------------------------------------------------------
void MagicByteConvert(LPBYTE buf, CMagicInf Magici[], int maxmagic)
{
	int n=0;

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// DB에 마법정보 저장하기
	// DB에 마법정보를 저장할 때 3BYTE가 하나의 스킬이 된다.
	// 첫 번째 BYTE : 스킬 번호
	// 두 번째 BYTE : 5bit : 스킬 레벨(32까지 가능)
	//				  3bit : 스킬번호가 1BYTE 단위를 넘을 때 마다 여기에 초과된 BYTE 단위를 설정한다.
	//						세 번째 BYTE와 계산해서 최종 스킬번호를 얻을 수 있다.(iExpendSkillCount)
	// 세 번째 BYTE : 255번을 넘는 번호는 쪼개서 여기에 넣는다.(iExpendSkillNumber)
	// -> DB확장이나 변환없이 스킬 추가 가능. ( 255 * 8 )

	int iExpendSkillCount;
	int iExpendSkillNumber;
	int iExpendSkillExtraNumber;
	for( int index=0; index < maxmagic; index++ )
	{
		// 초기화
		iExpendSkillNumber = Magici[index].m_Skill;

		iExpendSkillCount = 0;		
		iExpendSkillExtraNumber = 0;

		// 스킬번호 변환	: 255(0xFF)는 스킬 아이템이 없는 것으로 사용된다.
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬 저장 방식(DB) 추가&변경
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
