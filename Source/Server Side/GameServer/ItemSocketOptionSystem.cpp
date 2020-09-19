// ItemSocketOptionSystem.cpp: implementation of the CItemSocketOptionSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemSocketOptionSystem.h"

#include "Common\winutil.h"
#include "Include\ReadScript.h"

#include "Dirpath.h"
extern CDirPath gDirPath;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
extern ITEM_ATTRIBUTE ItemAttribute[MAX_ITEM];
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
#include "LargeRand.h"
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

CItemSocketOptionSystem g_SocketOptionSystem;
int g_RequireMoneyForSeedExtract = 0;
int g_RequireMoneyForSeedSphereComposite = 0;
int g_RequireMoneyForSeedSphereSet = 0;
int g_RequireMoneyForSeedSphereRemove = 0;

#ifdef MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808
int g_ShopBuySocketItemSlotCount = 0;
#endif // MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808

CItemSocketOptionSystem::CItemSocketOptionSystem()
{

}

CItemSocketOptionSystem::~CItemSocketOptionSystem()
{

}

void CItemSocketOptionSystem::Initialize()
{
	int iClearIndex = 0;

	for( iClearIndex = 0 ; iClearIndex < MAX_SEED_COUNT ; iClearIndex++ )
	{
		m_SeedData[iClearIndex].Clear();
	}

	for( iClearIndex = 0 ; iClearIndex < MAX_SPHERE_LEVEL ; iClearIndex++ )
	{
		m_SphereData[iClearIndex].Clear();
	}

	for( iClearIndex = 0 ; iClearIndex < MAX_BONUS_OPTION ; iClearIndex++ )
	{
		m_BonusSocketOptionData[iClearIndex].Clear();
	}

	m_MakeSocketSlotCountPool.InitPool();
	m_SeedExtractPool.InitPool();
	
#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	for( iClearIndex = 0 ; iClearIndex < MAX_SPHERE_LEVEL ; iClearIndex++ )
	{
		m_SphereDropRate[iClearIndex]	= 0;
		m_SphereDropLevel[iClearIndex]	= 0;
	}
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
}

bool CItemSocketOptionSystem::LoadOptionScript(char* pchFileName)
{
	Initialize();

	if( (SMDFile = fopen( pchFileName, "r" ) ) == FALSE )
	{
		// ����
		MsgBox( "[SocketOption] SocketOption Script file Load failed. (%s)", pchFileName );
		return false;
	}

	SMDToken Token;
	int iCategoryIndex = 0;
	int iSeedOptionDataCount = 0;
	int iSphereOptionDataCount = 0;
	int iBonusSocketOptionDataCount = 0;
	int iSetSocketoptionDataCount = 0;

	while( true )
	{
		Token = (*GetToken)();
		if( Token == END )
		{
			break;
		}

		if( strcmp("end", TokenString ) == NULL )
		{
			// ���� ī�װ��� �̵�
			break;
		}

		if( Token == NUMBER )
		{
			iCategoryIndex = (int)TokenNumber;
			while( true )
			{
				// �õ� �ɼ� ������
				if( iCategoryIndex == 0 )
				{
					BYTE btSeedIndex = 0;
					BYTE btSeedType = 0;				// �õ� �Ӽ� ( 1 : ��, 2 : ��, 3 : ����, 4 : �ٶ�, 5 : ����, 6 : ��, 7 : ����ũ )
					BYTE btItemLevel = 0;				// �õ� �Ӽ� ���� ������ ���� (������ ������ ���� �� �Ӽ��� �����ȴ�.)
														// ������ ������ ����
					BYTE btOptionValueType = 0;			// ������ �Ӽ��� ���� ��ġ Ÿ�� ( 1 : ���, 2 : �ۼ�Ʈ, 3 : ��ġ/�ѷ��� )
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
					BYTE btOptionValue[MAX_SPHERE_LEVEL] = {0,};				// ������ �Ӽ��� ���� ��ġ
#else
					BYTE btOptionValue = 0;				// ������ �Ӽ��� ���� ��ġ
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
					int iExtractRate = 0;				// ����Ȯ��

					// ������ �ڵ�� Ÿ�Կ� ���� �̸� ������ �õ� �Ӽ��� ���� �����Ѵ�.
					int iItemCode = 0;

					Token = (*GetToken)();
					if( strcmp("end", TokenString ) == NULL )
					{
						break;
					}

					btSeedIndex = (int)TokenNumber;
					Token = (*GetToken)();
					btSeedType = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btItemLevel = (BYTE)TokenNumber;
					// ������ ����
					Token = (*GetToken)();
					Token = (*GetToken)();
					btOptionValueType = (BYTE)TokenNumber;
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
					Token = (*GetToken)();
					btOptionValue[0] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btOptionValue[1] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btOptionValue[2] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btOptionValue[3] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btOptionValue[4] = (BYTE)TokenNumber;
#else
					Token = (*GetToken)();
					btOptionValue = (BYTE)TokenNumber;
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
					Token = (*GetToken)();
					iExtractRate = (int)TokenNumber;

					m_SeedExtractPool.AddValue( btSeedIndex, iExtractRate );
					m_SeedData[iSeedOptionDataCount].SetOption( btSeedIndex, btSeedType, btItemLevel, btOptionValueType, btOptionValue, iExtractRate );

					iSeedOptionDataCount++;

					if( iSeedOptionDataCount > MAX_SEEDSPHERE )
					{
						MsgBox( "[ERROR] MAX SeedSphere Option Data Overflow! [%d]", iSeedOptionDataCount );
						break;
					}
				}
				// ���ʽ� ���� �ɼ� ������
				else if( iCategoryIndex == 1 )
				{
					BONUS_SOCKET_OPTION BonusOptionData;

					Token = (*GetToken)();
					if( strcmp("end", TokenString ) == NULL )
					{
						break;
					}
					
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
					memset( &BonusOptionData, 0, sizeof( BONUS_SOCKET_OPTION ) );
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710

					BonusOptionData.OptionIndex = (int)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.ItemTypeBegin = (BYTE)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.ItemTypeEnd = (BYTE)TokenNumber;
					// �ɷ� �̸� �÷��� ������� �ʴ´�.
					Token = (*GetToken)();
					Token = (*GetToken)();
					BonusOptionData.OptionValueType = (BYTE)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.OptionValue = (BYTE)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.ExtractRate = (int)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.SocketSlotType[0] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.SocketSlotType[1] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.SocketSlotType[2] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.SocketSlotType[3] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					BonusOptionData.SocketSlotType[4] = (BYTE)TokenNumber;

					for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
					{
						if( BonusOptionData.SocketSlotType[i] != 0 )
						{
							BonusOptionData.SocketSlotCount++;
						}
					}

					switch( BonusOptionData.OptionIndex )
					{
					case BONUS_OPTION_WEAPON_ATTACK_POWER:
						BonusOptionData.EffectType = SEED_EFFECT_TYPE_ATTACK_POWER;
						break;
					case BONUS_OPTION_WEAPON_ATTACK_SKILL_POWER:
						BonusOptionData.EffectType = SEED_EFFECT_TYPE_ATTACK_SKILL_POWER;
						break;
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
					case BONUS_OPTION_STAFF_ATTACK_POWER:
						BonusOptionData.EffectType = SEED_EFFECT_TYPE_ATTACK_POWER;
#else
					case BONUS_OPTION_STAFF_MAGIC_POWER:
						BonusOptionData.EffectType = SEED_EFFECT_TYPE_MAGIC_POWER;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
						break;
					case BONUS_OPTION_STAFF_ATTACK_SKILL_POWER:
						BonusOptionData.EffectType = SEED_EFFECT_TYPE_ATTACK_SKILL_POWER;
						break;
					case BONUS_OPTION_PROTECT_BLOCK_POWER:
						BonusOptionData.EffectType = SEED_EFFECT_TYPE_BLOCK_POWER;
						break;
					case BONUS_OPTION_PROTECT_MAX_HP_INC:
						BonusOptionData.EffectType = SEED_EFFECT_TYPE_MAX_HP_INC;
						break;
					}

					memcpy( &m_BonusSocketOptionData[iBonusSocketOptionDataCount], &BonusOptionData, sizeof( BONUS_SOCKET_OPTION ) );
					iBonusSocketOptionDataCount++;
				}
				// ��Ʈ �ɼ� ������
				else if( iCategoryIndex == 2 )
				{
					int iSetSocketOptionIndex = 0;
					BYTE btSetSocketType = 0;
					BYTE btSetSocketOptionValueType = 0;
					BYTE btSetSocketOptionValue = 0;
					int iExtractRate = 0;
					BYTE btSetSocketTypeIndex[6] = {0,};

					Token = (*GetToken)();
					if( strcmp("end", TokenString ) == NULL )
					{
						break;
					}

					iSetSocketOptionIndex = (int)TokenNumber;
					Token = (*GetToken)();
					btSetSocketType = (BYTE)TokenNumber;
					// �ɷ� �̸� �÷��� ������� �ʴ´�.
					Token = (*GetToken)();
					Token = (*GetToken)();
					btSetSocketOptionValueType = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btSetSocketOptionValue = (BYTE)TokenNumber;
					Token = (*GetToken)();
					iExtractRate = (int)TokenNumber;
					Token = (*GetToken)();
					btSetSocketTypeIndex[0] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btSetSocketTypeIndex[1] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btSetSocketTypeIndex[2] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btSetSocketTypeIndex[3] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btSetSocketTypeIndex[4] = (BYTE)TokenNumber;
					Token = (*GetToken)();
					btSetSocketTypeIndex[5] = (BYTE)TokenNumber;

					m_SetSocketOptionData[iSetSocketoptionDataCount].OptionIndex		= iSetSocketoptionDataCount;
					m_SetSocketOptionData[iSetSocketoptionDataCount].OptionValueType	= btSetSocketOptionValueType;
					m_SetSocketOptionData[iSetSocketoptionDataCount].OptionValue		= btSetSocketOptionValue;
					m_SetSocketOptionData[iSetSocketoptionDataCount].ExtractRate		= iExtractRate;
					m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[0]	= btSetSocketTypeIndex[0];
					m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[1]	= btSetSocketTypeIndex[1];
					m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[2]	= btSetSocketTypeIndex[2];
					m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[3]	= btSetSocketTypeIndex[3];
					m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[4]	= btSetSocketTypeIndex[4];
					m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[5]	= btSetSocketTypeIndex[5];
					switch( iSetSocketOptionIndex )
					{
					case 0:
						m_SetSocketOptionData[iSetSocketoptionDataCount].EffectType	= SEED_EFFECT_TYPE_SET_DOUBLE_DAMAGE_RATE_INC;
						break;
					case 1:
						m_SetSocketOptionData[iSetSocketoptionDataCount].EffectType	= SEED_EFFECT_TYPE_SET_IGNORE_ENERMY_DEFENSE;
						break;
					}

					iSetSocketoptionDataCount++;
				}
#ifndef MODIFY_SOCKET_OPTION_SYSTEM_20080725				// !! NOT
				// ���Ǿ� ������ ��ġ ������
				else if( iCategoryIndex == 3 )
				{
					BYTE btSphereLevel = 0;
					float fIncreaseRate = 0.f;
					int iIncreaseRate = 0;

					Token = (*GetToken)();
					if( strcmp("end", TokenString ) == NULL )
					{
						break;
					}

					btSphereLevel = (BYTE)TokenNumber;
					Token = (*GetToken)();
					fIncreaseRate = (float)TokenNumber;
					iIncreaseRate = fIncreaseRate * 10000;

					m_SphereData[iSphereOptionDataCount].SetOption( btSphereLevel, iIncreaseRate );

					iSphereOptionDataCount++;
				}
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
			}
		}
	}

	fclose(SMDFile);

	LoadCommonServerScript();

	return true;
}

void CItemSocketOptionSystem::LoadCommonServerScript()
{
	int iNoneSlotItemRate = 10000;

#ifdef MODIFY_SOCKET_ITEM_DROP_SYSTEM_FIX_20080828
	// ���� ������ ��� Ȯ���� �ɼ� ���ε尡 �����ϵ��� ����
	m_MakeSocketSlotCountPool.InitPool();
	m_TwoHandWeaponMakeSocketSlotCountPool.InitPool();
#endif // MODIFY_SOCKET_ITEM_DROP_SYSTEM_FIX_20080828
	
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	m_MakeSocketSlotRate[0]	= GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate1",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[1] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate2",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[2] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate3",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[3] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate4",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[4] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate5",		0,	gDirPath.GetNewPath("commonserver.cfg"));

	m_TwoHandWeaponMakeSocketSlotRate[0]	= GetPrivateProfileInt("GameServerInfo",	"TwoHandWeaponSocketSlotCountRate1",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_TwoHandWeaponMakeSocketSlotRate[1]	= GetPrivateProfileInt("GameServerInfo",	"TwoHandWeaponSocketSlotCountRate2",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_TwoHandWeaponMakeSocketSlotRate[2]	= GetPrivateProfileInt("GameServerInfo",	"TwoHandWeaponSocketSlotCountRate3",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_TwoHandWeaponMakeSocketSlotRate[3]	= GetPrivateProfileInt("GameServerInfo",	"TwoHandWeaponSocketSlotCountRate4",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_TwoHandWeaponMakeSocketSlotRate[4]	= GetPrivateProfileInt("GameServerInfo",	"TwoHandWeaponSocketSlotCountRate5",		0,	gDirPath.GetNewPath("commonserver.cfg"));
#else
	m_MakeSocketSlotRate[0]	= GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate1",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[1] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate2",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[2] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate3",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[3] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate4",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_MakeSocketSlotRate[4] = GetPrivateProfileInt("GameServerInfo",	"SocketSlotCountRate5",		0,	gDirPath.GetNewPath("commonserver.cfg"));
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

	g_RequireMoneyForSeedExtract			= GetPrivateProfileInt("GameServerInfo",	"SeedExtractMoney",				1000000,	gDirPath.GetNewPath("commonserver.cfg"));
	g_RequireMoneyForSeedSphereComposite	= GetPrivateProfileInt("GameServerInfo",	"SeedSphereCompositeMoney",		1000000,	gDirPath.GetNewPath("commonserver.cfg"));
	g_RequireMoneyForSeedSphereSet			= GetPrivateProfileInt("GameServerInfo",	"SeedSphereSetMoney",			1000000,	gDirPath.GetNewPath("commonserver.cfg"));
	g_RequireMoneyForSeedSphereRemove		= GetPrivateProfileInt("GameServerInfo",	"SeedSphereRemoveMoney",		1000000,	gDirPath.GetNewPath("commonserver.cfg"));

#ifdef MODIFY_SOCKET_ITEM_DROP_SYSTEM_FIX_20080828
	// 0��¥�� ���� �������� ��ӵ��� �ʵ��� ����
	m_MakeSocketSlotCountPool.AddValue( 1, m_MakeSocketSlotRate[0] );
	m_MakeSocketSlotCountPool.AddValue( 2, m_MakeSocketSlotRate[1] );
	m_MakeSocketSlotCountPool.AddValue( 3, m_MakeSocketSlotRate[2] );
	m_MakeSocketSlotCountPool.AddValue( 4, m_MakeSocketSlotRate[3] );
	m_MakeSocketSlotCountPool.AddValue( 5, m_MakeSocketSlotRate[4] );

	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 1, m_TwoHandWeaponMakeSocketSlotRate[0] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 2, m_TwoHandWeaponMakeSocketSlotRate[1] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 3, m_TwoHandWeaponMakeSocketSlotRate[2] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 4, m_TwoHandWeaponMakeSocketSlotRate[3] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 5, m_TwoHandWeaponMakeSocketSlotRate[4] );
#else
	for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
	{
		iNoneSlotItemRate -= m_MakeSocketSlotRate[i];
	}

	m_MakeSocketSlotCountPool.AddValue( 0, iNoneSlotItemRate ); 
	m_MakeSocketSlotCountPool.AddValue( 1, m_MakeSocketSlotRate[0] );
	m_MakeSocketSlotCountPool.AddValue( 2, m_MakeSocketSlotRate[1] );
	m_MakeSocketSlotCountPool.AddValue( 3, m_MakeSocketSlotRate[2] );
	m_MakeSocketSlotCountPool.AddValue( 4, m_MakeSocketSlotRate[3] );
	m_MakeSocketSlotCountPool.AddValue( 5, m_MakeSocketSlotRate[4] );

	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	iNoneSlotItemRate = 10000;

	for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
	{
		iNoneSlotItemRate -= m_TwoHandWeaponMakeSocketSlotRate[i];
	}

	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 0, iNoneSlotItemRate ); 
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 1, m_TwoHandWeaponMakeSocketSlotRate[0] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 2, m_TwoHandWeaponMakeSocketSlotRate[1] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 3, m_TwoHandWeaponMakeSocketSlotRate[2] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 4, m_TwoHandWeaponMakeSocketSlotRate[3] );
	m_TwoHandWeaponMakeSocketSlotCountPool.AddValue( 5, m_TwoHandWeaponMakeSocketSlotRate[4] );
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
#endif // MODIFY_SOCKET_ITEM_DROP_SYSTEM_FIX_20080828
	
#ifdef MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808
	// ���� �ǸŵǴ� ���� �������� ������ ����
	g_ShopBuySocketItemSlotCount = GetPrivateProfileInt("GameServerInfo",	"ShopBuySocketItemSlotCount",				3,	gDirPath.GetNewPath("commonserver.cfg"));
#endif // MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808

#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	m_SphereDropOn				= GetPrivateProfileInt("GameServerInfo",	"SphereDropOn",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropRate[0]			= GetPrivateProfileInt("GameServerInfo",	"SphereDropRate_Lv1",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropRate[1]			= GetPrivateProfileInt("GameServerInfo",	"SphereDropRate_Lv2",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropRate[2]			= GetPrivateProfileInt("GameServerInfo",	"SphereDropRate_Lv3",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropRate[3]			= GetPrivateProfileInt("GameServerInfo",	"SphereDropRate_Lv4",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropRate[4]			= GetPrivateProfileInt("GameServerInfo",	"SphereDropRate_Lv5",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropLevel[0]		= GetPrivateProfileInt("GameServerInfo",	"SphereDropLevel_Lv1",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropLevel[1]		= GetPrivateProfileInt("GameServerInfo",	"SphereDropLevel_Lv2",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropLevel[2]		= GetPrivateProfileInt("GameServerInfo",	"SphereDropLevel_Lv3",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropLevel[3]		= GetPrivateProfileInt("GameServerInfo",	"SphereDropLevel_Lv4",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_SphereDropLevel[4]		= GetPrivateProfileInt("GameServerInfo",	"SphereDropLevel_Lv5",				0,	gDirPath.GetNewPath("commonserver.cfg"));
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
}

//////////////////////////////////////////////////////////////////////////
// 1. ���� ������ Ȯ���� ���� �Լ���.
// 1-1. ���� ������ ������ ���������� Ȯ��
bool CItemSocketOptionSystem::IsEnableSocketItem(CItem* lpItemData)
{
	if( lpItemData == NULL )
	{
		return false;
	}

	if( lpItemData->IsItem() == FALSE )
	{
		return false;
	}

	return IsEnableSocketItem( lpItemData->m_Type );
}

// 1-1. ���� ������ ������ ���������� Ȯ�� (������ �ڵ�� Ȯ��)
bool CItemSocketOptionSystem::IsEnableSocketItem(int ItemCode)
{
	switch( ItemCode )
	{
	// ���� (�˷�)
	case MAKE_ITEMNUM2( 0, 26 ):					// �ö�������
	case MAKE_ITEMNUM2( 0, 27 ):					// �ҵ�극��Ŀ
	case MAKE_ITEMNUM2( 0, 28 ):					// ��ٽ�Ÿ��
	// ���� (�б��)
	case MAKE_ITEMNUM2( 2, 16 ):					// ���ν�Ʈ���̽�
	case MAKE_ITEMNUM2( 2, 17 ):					// �ۼַ�Ʈ����
	// ���� (Ȱ/���÷�)
	case MAKE_ITEMNUM2( 4, 23 ):					// ��ũ���ð�
	// ���� (�����̷�)
	case MAKE_ITEMNUM2( 5, 30 ):					// ���鸮������
	case MAKE_ITEMNUM2( 5, 31 ):					// �κ����˽�����
	case MAKE_ITEMNUM2( 5, 32 ):					// �ҿ�긵��
	// �� (���з�)
	case MAKE_ITEMNUM2( 6, 17 ):					// ũ�����۷θ�
	case MAKE_ITEMNUM2( 6, 18 ):					// �����Ǵ�����
	case MAKE_ITEMNUM2( 6, 19 ):					// ���ν�Ʈ�踮��
	case MAKE_ITEMNUM2( 6, 20 ):					// ����ȹ���
	// �� (������)
	case MAKE_ITEMNUM2( 7, 45 ):					// Ƽź����
	case MAKE_ITEMNUM2( 7, 46 ):					// �극�̺����
//#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	case MAKE_ITEMNUM2( 7, 49 ):					// ���������
	case MAKE_ITEMNUM2( 7, 50 ):					// ��������
	case MAKE_ITEMNUM2( 7, 51 ):					// �пո���ũ
	case MAKE_ITEMNUM2( 7, 52 ):					// �ϵ������
	case MAKE_ITEMNUM2( 7, 53 ):					// ��ť�������
// #endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	// �� (���ʷ�)
	case MAKE_ITEMNUM2( 8, 45 ):					// Ƽź����
	case MAKE_ITEMNUM2( 8, 46 ):					// �극�̺갩��
	case MAKE_ITEMNUM2( 8, 47 ):					// ���Ұ���
	case MAKE_ITEMNUM2( 8, 48 ):					// ��Ʈ���̰���
	case MAKE_ITEMNUM2( 8, 49 ):					// �����˰���
	case MAKE_ITEMNUM2( 8, 50 ):					// ����ΰ���
	case MAKE_ITEMNUM2( 8, 51 ):					// �пհ���
	case MAKE_ITEMNUM2( 8, 52 ):					// �ϵ�������
	case MAKE_ITEMNUM2( 8, 53 ):					// ��ť��������
	// �� (������)
	case MAKE_ITEMNUM2( 9, 45 ):					// Ƽź����
	case MAKE_ITEMNUM2( 9, 46 ):					// �극�̺����
	case MAKE_ITEMNUM2( 9, 47 ):					// ���ҹ���
	case MAKE_ITEMNUM2( 9, 48 ):					// ��Ʈ���̹���
	case MAKE_ITEMNUM2( 9, 49 ):					// �����˹���
	case MAKE_ITEMNUM2( 9, 50 ):					// ����ι���
	case MAKE_ITEMNUM2( 9, 51 ):					// �пչ���
	case MAKE_ITEMNUM2( 9, 52 ):					// �ϵ�������
	case MAKE_ITEMNUM2( 9, 53 ):					// ��ť��������
	// �� (�尩��)
	case MAKE_ITEMNUM2( 10, 45 ):					// Ƽź�尩
	case MAKE_ITEMNUM2( 10, 46 ):					// �극�̺��尩
	case MAKE_ITEMNUM2( 10, 47 ):					// �����尩
	case MAKE_ITEMNUM2( 10, 48 ):					// ��Ʈ�����尩
	case MAKE_ITEMNUM2( 10, 49 ):					// �������尩
	case MAKE_ITEMNUM2( 10, 50 ):					// ������尩
	case MAKE_ITEMNUM2( 10, 51 ):					// �п��尩
	case MAKE_ITEMNUM2( 10, 52 ):					// �ϵ����尩
	case MAKE_ITEMNUM2( 10, 53 ):					// ��ť�����尩
	// �� (������)
	case MAKE_ITEMNUM2( 11, 45 ):					// Ƽź����
	case MAKE_ITEMNUM2( 11, 46 ):					// �극�̺����
	case MAKE_ITEMNUM2( 11, 47 ):					// ���Һ���
	case MAKE_ITEMNUM2( 11, 48 ):					// ��Ʈ���̺���
	case MAKE_ITEMNUM2( 11, 49 ):					// �����˺���
	case MAKE_ITEMNUM2( 11, 50 ):					// ����κ���
	case MAKE_ITEMNUM2( 11, 51 ):					// �пպ���
	case MAKE_ITEMNUM2( 11, 52 ):					// �ϵ�������
	case MAKE_ITEMNUM2( 11, 53 ):					// ��ť��������
		return true;
	}

	return false;
}

// 1-2. �ش� �������� ���� ���� ������ Ȯ��
BYTE CItemSocketOptionSystem::GetEmptySlotCount(CItem* lpItemData)
{
	BYTE btEmptySlotCount = 0;

	for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
	{
		if( lpItemData->m_SocketOption[i] == SOCKETSLOT_EMPTY )
		{
			btEmptySlotCount++;
		}
	}

	return btEmptySlotCount;
}

// 1-3. �ش� �õ� ���Ǿ��� ȿ���� Ȯ��
bool CItemSocketOptionSystem::GetSeedSphereEffect(BYTE btSeedSphereIndex, BYTE& pbSeedType, BYTE& pbEffectValueType, BYTE& pbEffectValue)
{
	BYTE SeedIndex = 0;
	BYTE SphereLevel = 0;
	CSeedItem* lpSeedData = NULL;
	CSphereItem* lpSphereData = NULL;
	CSeedSphereItem SeedSphere;

	SeedIndex	= btSeedSphereIndex % MAX_SEED_COUNT;
	SphereLevel = ( ( btSeedSphereIndex - SeedIndex ) / MAX_SEED_COUNT ) + 1;

	lpSeedData = GetSeedData( SeedIndex );
	lpSphereData = GetSphereData( SphereLevel );

#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	if( lpSeedData == NULL )
	{
		return false;
	}

	SeedSphere.SetOption( lpSeedData, SphereLevel );
#else
	lpSphereData = GetSphereData( SphereLevel );

	if( lpSeedData == NULL || lpSphereData == NULL )
	{
		return false;
	}

	SeedSphere.SetOption( lpSeedData, lpSphereData );
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

	pbSeedType			= SeedSphere.m_SeedIndex;
	pbEffectValueType	= SeedSphere.m_SeedOptionValueType;
	pbEffectValue		= SeedSphere.m_SeedOptionValue;

	return true;
}

// 1-4. �ش� �������� ���� ������ ����Ʈ �迭�� ��ȯ
void CItemSocketOptionSystem::GetSocketOption(CItem* lpItemData, BYTE* pbSocketOption, BYTE& pbSocketBonusOption)
{
	if( lpItemData == NULL )
	{
		return;
	}

	if( lpItemData->IsItem() == FALSE || IsEnableSocketItem( lpItemData ) == false )
	{
		return;
	}

	for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
	{
		pbSocketOption[i] = lpItemData->m_SocketOption[i];
	}

	pbSocketBonusOption = lpItemData->m_BonusSocketOption;
}

// 1-5. ���� ���������� Ȯ��
bool CItemSocketOptionSystem::IsSocketItem(CItem* lpItemData)
{
	if( lpItemData == NULL )
	{
		return false;
	}

	if( lpItemData->IsItem() == FALSE )
	{
		return false;
	}

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	if( IsEnableSocketItem( lpItemData ) == false )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
	{
		if( lpItemData->m_SocketOption[i] != SOCKETSLOT_NONE )
		{
			return true;
		}
	}

	return false;
#else
	return IsSocketItem( lpItemData->m_Type );
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
}

#ifndef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630					// !! NOT
// 1-6. ���� ������ �ִ� ���������� Ȯ�� (������ �ڵ�� Ȯ��) - ������
bool CItemSocketOptionSystem::IsSocketItem(int ItemCode)
{
	BYTE btItemType = 0;
	BYTE btItemIndex = 0;

	btItemIndex = ItemCode % MAX_ITEM_INDEX;
	btItemType = ( ItemCode - btItemIndex ) / 512;

	if( btItemType == 12 )
	{
		if( btItemIndex >= 60 && btItemIndex <= 129 )
		{
			return true;
		}
	}

	return false;
}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630

// 1-7. �õ� ���������� Ȯ��
bool CItemSocketOptionSystem::IsSeedItem(int ItemCode)
{
	switch( ItemCode )
	{
	case MAKE_ITEMNUM2( 12, 60 ):
	case MAKE_ITEMNUM2( 12, 61 ):
	case MAKE_ITEMNUM2( 12, 62 ):
	case MAKE_ITEMNUM2( 12, 63 ):
	case MAKE_ITEMNUM2( 12, 64 ):
	case MAKE_ITEMNUM2( 12, 65 ):
		return true;
	}

	return false;
}

// 1-8. ���Ǿ� ���������� Ȯ��
bool CItemSocketOptionSystem::IsSphereItem(int ItemCode)
{
	switch( ItemCode )
	{
	case MAKE_ITEMNUM2( 12, 70 ):
	case MAKE_ITEMNUM2( 12, 71 ):
	case MAKE_ITEMNUM2( 12, 72 ):
	case MAKE_ITEMNUM2( 12, 73 ):
	case MAKE_ITEMNUM2( 12, 74 ):
		return true;
	}

	return false;
}

// 1-9. �õ彺�Ǿ� ���������� Ȯ��
bool CItemSocketOptionSystem::IsSeedSphereItem(int ItemCode)
{
	BYTE btItemType = 0;
	BYTE btItemIndex = 0;

	btItemIndex = ItemCode % MAX_ITEM_INDEX;
	btItemType = ( ItemCode - btItemIndex ) / 512;

	if( btItemType == 12 )
	{
		if( btItemIndex >= 100 && btItemIndex <= 129 )
		{
			return true;
		}
	}

	return false;
}

// 1-10. �ش� �������� �õ� �ε����� Ȯ��
BYTE CItemSocketOptionSystem::GetSeedIndex(int ItemCode, BYTE ItemLevel)
{
	for( int i = 0 ; i < MAX_SEED_COUNT ; i++ )
	{
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
		if( m_SeedData[i].m_ItemCode == ItemCode && m_SeedData[i].m_ItemLevel == ItemLevel )
#else
		if( m_SeedData[i].m_ItemCode == ItemCode && m_SeedData->m_ItemLevel == ItemLevel )
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
		{
			return m_SeedData[i].m_SeedIndex;
		}
	}

	return SOCKETSLOT_NONE;
}

// 1-11. �ش� �������� ���Ǿ� ������ Ȯ��
BYTE CItemSocketOptionSystem::GetSphereLevel(int ItemCode)
{
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	switch( ItemCode )
	{
	case MAKE_ITEMNUM2( 12, 70 ):
		return 1;
	case MAKE_ITEMNUM2( 12, 71 ):
		return 2;
	case MAKE_ITEMNUM2( 12, 72 ):
		return 3;
	case MAKE_ITEMNUM2( 12, 73 ):
		return 4;
	case MAKE_ITEMNUM2( 12, 74 ):
		return 5;
	}

	return 0;
#else
	for( int i = 0 ; i < MAX_SPHERE_LEVEL ; i++ )
	{
		if( m_SphereData[i].m_ItemCode == ItemCode )
		{
			return m_SphereData[i].m_SphereLevel;
		}
	}

	return SOCKETSLOT_NONE;
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
}

// 1-12. �ش� �����ۿ� ���� Ÿ���� �õ彺�Ǿ �ִ��� Ȯ��
bool CItemSocketOptionSystem::CheckItemForSameSeedType(CItem* lpItemData, WORD SeedSphereItemCode, BYTE ItemLevel)
{
	BYTE btSeedIndex = 0;
	BYTE btSeedSphereIndex = 0;

	if( IsEnableSocketItem( lpItemData ) == false )
	{
		return false;
	}

	btSeedSphereIndex = GetSeedSphereIndexForItemCode( SeedSphereItemCode, ItemLevel );
	btSeedIndex = btSeedSphereIndex % MAX_SEED_COUNT;

	for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
	{
		if( lpItemData->m_SocketOption[i] == SOCKETSLOT_NONE || lpItemData->m_SocketOption[i] == SOCKETSLOT_EMPTY )
		{
			continue;
		}

		if( ( lpItemData->m_SocketOption[i] % MAX_SEED_COUNT ) == btSeedIndex )
		{
			return false;
		}
	}

	return true;
}

// 1-13. �õ彺�Ǿ� �������ڵ�� ������ �õ彺�Ǿ��� �ε����� Ȯ��
BYTE CItemSocketOptionSystem::GetSeedSphereIndexForItemCode(WORD ItemCode, BYTE ItemLevel)
{
	BYTE btSeedType = 0;
	BYTE btSphereLevel = 0;
	BYTE btSeedSphereIndex = 0;

	btSphereLevel = ( ( ( ItemCode % MAX_ITEM_INDEX ) - 100 ) / 6 );
	btSeedType = ( ( ( ItemCode % MAX_ITEM_INDEX ) - 100 ) % 6 ) + 1;

	for( int i = 0 ; i < MAX_SEED_COUNT ; i++ )
	{
		if( m_SeedData[i].m_SeedType == btSeedType && m_SeedData[i].m_ItemLevel == ItemLevel )
		{
			btSeedSphereIndex = ( btSphereLevel * MAX_SEED_COUNT ) + m_SeedData[i].m_SeedIndex;
			return btSeedSphereIndex;
		}
	}

	return SOCKETSLOT_NONE;
}

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
// 1-14. �õ彺�Ǿ� �ε����� ������ ������ Ȯ��
DWORD CItemSocketOptionSystem::GetSeedSphereValue(BYTE btSeedSphereIndex)
{
	BYTE btSeedIndex = 0;
	BYTE btSphereLevel = 0;
	CSeedItem* lpSeedData = NULL;
	CSphereItem* lpSphereData = NULL;
	CSeedSphereItem SeedSphere;
	DWORD dwSeedSphereValue = 0;

	btSeedIndex = btSeedSphereIndex % MAX_SEED_COUNT;
	btSphereLevel = ( ( btSeedSphereIndex - btSeedIndex ) / MAX_SEED_COUNT ) + 1;

	lpSeedData = GetSeedData( btSeedIndex );
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	if( lpSeedData == NULL )
	{
		return false;
	}

	SeedSphere.SetOption( lpSeedData, btSphereLevel );
#else
	lpSphereData = GetSphereData( btSphereLevel );

	if( lpSeedData == NULL || lpSphereData == NULL )
	{
		return 0;
	}

	SeedSphere.SetOption( lpSeedData, lpSphereData );
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

	dwSeedSphereValue = ItemAttribute[SeedSphere.GetItemCode()].BuyMoney;

	return dwSeedSphereValue;
}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
// 1-15. �ش� �õ彺�Ǿ �ش� �����ۿ� ������ �� �ִ��� Ȯ��
bool CItemSocketOptionSystem::CheckSetSeedSphereItemType(CItem* lpItemData, BYTE btSeedSphereIndex)
{
	if( IsEnableSocketItem( lpItemData ) == false )
	{
		return false;
	}

	BYTE btItemType = 0;
	BYTE btItemIndex = 0;
	BYTE btSeedSphereType = SEED_TYPE_NONE;

	btItemIndex = lpItemData->m_Type % MAX_ITEM_INDEX;
	btItemType = ( lpItemData->m_Type - btItemIndex ) / MAX_ITEM_INDEX;

	btSeedSphereType = GetSocketType( btSeedSphereIndex );

	// �������� �������� ������ Ȯ��
	if( btItemType >= 0 && btItemType <= 5 )			// ����
	{
		switch( btSeedSphereType )
		{
		case SEED_TYPE_FIRE:
		case SEED_TYPE_ICE:
		case SEED_TYPE_LIGHTNING:
			return true;
		}
	}
	else if( btItemType >= 6 && btItemType <= 11 )
	{
		// ���� ��� �õ彺�Ǿ� Ÿ�� 2,4,6 �� ��� ���� ����
		switch( btSeedSphereType )
		{
		case SEED_TYPE_WATER:
		case SEED_TYPE_WIND:
		case SEED_TYPE_EARTH:
			return true;
		}
	}
	else
	{
		return false;
	}

	return false;
}
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

#ifdef MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825
// 1-16. ���� �������� ��� ������ ������ Ȯ��
bool CItemSocketOptionSystem::IsEnableDropSocketItemMap(int iMapIndex)
{
	switch( iMapIndex )
	{
	case 57:
	case 58:
		return true;
	}

	return false;
}
#endif // MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825

#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
// 1-17. ���Ǿ� ��� Ȯ�� Ȯ���� ���� �Լ�
int CItemSocketOptionSystem::GetSphereDropInfo(int iMonsterLevel)
{
	int iItemCode = -1;
	int iSphereLevel = 0;

	if( m_SphereDropOn != TRUE )
	{
		return iItemCode;
	}

	// ���� ������ ���� ��� ������ ���Ǿ� ������ Ȯ���Ѵ�.
	if( m_SphereDropLevel[4] != 0 && m_SphereDropLevel[4] <= iMonsterLevel )
	{
		iSphereLevel = 5;
	}
	else if( m_SphereDropLevel[3] != 0 && m_SphereDropLevel[3] <= iMonsterLevel )
	{
		iSphereLevel = 4;
	}
	else if( m_SphereDropLevel[2] != 0 && m_SphereDropLevel[2] <= iMonsterLevel )
	{
		iSphereLevel = 3;
	}
	else if( m_SphereDropLevel[1] != 0 && m_SphereDropLevel[1] <= iMonsterLevel )
	{
		iSphereLevel = 2;
	}
	else if( m_SphereDropLevel[0] != 0 && m_SphereDropLevel[0] <= iMonsterLevel )
	{
		iSphereLevel = 1;
	}
	else
	{
		return iItemCode;
	}

	for( int i = 0 ; i < iSphereLevel ; i++ )
	{
		if( m_SphereDropRate[i] >= ( GetLargeRand() % 10000 ) )
		{
			iItemCode = MAKE_ITEMNUM( 12, 70 ) + i;
		}
	}

	return iItemCode;
}
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 2. �õ�� ���Ǿ�� �õ彺�Ǿ� ������ ���� �Լ���.
// 2-1. �õ�� ���Ǿ�� �õ彺�Ǿ� �ε����� ����
BYTE CItemSocketOptionSystem::GetSeedSphereIndex(BYTE btSeedIndex, BYTE btSphereLevel)
{
	return (BYTE)((btSphereLevel*MAX_SEED_COUNT)+btSeedIndex);
}

// 2-2. �õ�� ���Ǿ�� �õ彺�Ǿ� �����͸� ����
bool CItemSocketOptionSystem::MakeSeedSphere(CSeedSphereItem* lpSeedSphere, BYTE btSeedIndex, BYTE btSphereLevel)
{
	CSeedItem* lpSeedData = NULL;
	CSphereItem* lpSphereData = NULL;

	lpSeedData = GetSeedData( btSeedIndex );
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	if( lpSeedData == NULL )
	{
		return false;
	}

	if( lpSeedSphere->SetOption( lpSeedData, btSphereLevel ) == false )
	{
		return false;
	}
#else
	lpSphereData = GetSphereData( btSphereLevel );

	if( lpSeedData == NULL || lpSphereData == NULL )
	{
		return false;
	}

	if( lpSeedSphere->SetOption( lpSeedData, lpSphereData ) == false )
	{
		return false;
	}
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

	return true;
}

// 2-3. �õ嵥���͸� �˻�
CSeedItem* CItemSocketOptionSystem::GetSeedData(BYTE btSeedIndex)
{
	for( int i = 0 ; i < MAX_SEED_COUNT ; i++ )
	{
		if( m_SeedData[i].m_SeedIndex == btSeedIndex )
		{
			return &m_SeedData[i];
		}
	}

	return NULL;
}

// 2-4. ���Ǿ���͸� �˻�
CSphereItem* CItemSocketOptionSystem::GetSphereData(BYTE btSphereLevel)
{
	for( int i = 0 ; i < MAX_SPHERE_LEVEL ; i++ )
	{
		if( m_SphereData[i].m_SphereLevel == btSphereLevel )
		{
			return &m_SphereData[i];
		}
	}

	return NULL;
}

// 2-5. �õ� ���� Ȯ���� ���� �õ� ������ ����
BYTE CItemSocketOptionSystem::GetExtractSeed()
{
	int iSeedDataIndex = 0;

	iSeedDataIndex = m_SeedExtractPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );

	return iSeedDataIndex;
}

//////////////////////////////////////////////////////////////////////////
// 3. ���ʽ� ���� �ɼ��� �����ϱ� ���� �Լ���.
// 3-1. �õ� ���Ǿ� �ε����� �ش� �õ��� Ÿ���� Ȯ��
BYTE CItemSocketOptionSystem::GetSocketType(BYTE btSeedSphereIndex)
{
	BYTE btSeedIndex = 0;
	CSeedItem* lpSeedData = NULL;

	btSeedIndex = btSeedSphereIndex % MAX_SEED_COUNT;

	lpSeedData = GetSeedData( btSeedIndex );
	if( lpSeedData == NULL )
	{
		return SEED_TYPE_NONE;
	}

	return lpSeedData->m_SeedType;
}

LPBONUS_SOCKET_OPTION CItemSocketOptionSystem::GetBonusSocketOption(BYTE btBonusSocketOptionIndex)
{
	for( int i = 0 ; i < MAX_BONUS_OPTION ; i++ )
	{
		if( m_BonusSocketOptionData[i].OptionIndex == btBonusSocketOptionIndex )
		{
			return &m_BonusSocketOptionData[i];
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 4. �õ彺�Ǿ��� ȿ���� �����Ű�� ���� �Լ���.
// 4-1. �õ彺�Ǿ��� ȿ���� ����
int CItemSocketOptionSystem::ApplySeedSphereEffect(LPOBJECTSTRUCT lpObj)
{
	int iTotalSeedSphereCount = 0;
	BYTE SeedSphereIndex = 0;
	BYTE SeedIndex = 0;
	BYTE SphereLevel = 0;
	CSeedSphereItem SeedSphere;

	// ȣ��� lpObj�� NULL üũ�� ���ֱ� ������ �����ص� ��

	for( int iEquip = 0 ; iEquip < MAX_EQUIPMENT ; iEquip++ )
	{
		// ���� ������ ������ �������� �ƴϸ� ����
		if( IsEnableSocketItem( &lpObj->pInventory[iEquip] ) == false )
		{
			continue;
		}

		for( int iSocketSlot = 0 ; iSocketSlot < MAX_SOCKET_SLOT ; iSocketSlot++ )
		{
			SeedSphereIndex = lpObj->pInventory[iEquip].m_SocketOption[iSocketSlot];
			// ���� ������ ��������� ����.
			if( SeedSphereIndex == SOCKETSLOT_NONE || SeedSphereIndex == SOCKETSLOT_EMPTY )
			{
				continue;
			}

			SeedIndex = SeedSphereIndex % MAX_SEED_COUNT;
			SphereLevel = ( ( SeedSphereIndex - SeedIndex ) / MAX_SEED_COUNT ) + 1;

			if( MakeSeedSphere( &SeedSphere, SeedIndex, SphereLevel ) == false )
			{
				continue;
			}

			SetEffect( lpObj, SOCKET_OPTION_TYPE_SEED, SeedIndex, SeedSphere.m_SeedOptionValueType, SeedSphere.m_SeedOptionValue );

			iTotalSeedSphereCount++;
		}
	}

	return iTotalSeedSphereCount;
}

// 4-2. ���ʽ� ���� �ɼ��� ȿ���� ����
void CItemSocketOptionSystem::ApplyBonusOptionEffect(LPOBJECTSTRUCT lpObj)
{
	BYTE btBonusSocketIndex = 0;
	LPBONUS_SOCKET_OPTION lpBonusOptionData = NULL;

	for( int iEquip = 0 ; iEquip < MAX_EQUIPMENT ; iEquip++ )
	{
		if( IsEnableSocketItem( &lpObj->pInventory[iEquip] ) == false )
		{
			continue;
		}

		btBonusSocketIndex = lpObj->pInventory[iEquip].m_BonusSocketOption;

		if( btBonusSocketIndex == SOCKETSLOT_NONE || btBonusSocketIndex == SOCKETSLOT_EMPTY )
		{
			continue;
		}

		lpBonusOptionData = GetBonusSocketOption( btBonusSocketIndex );
		if( lpBonusOptionData == NULL )
		{
			continue;
		}
		
#ifdef ADD_SOCKET_OPTION_LOG_20080715
		LogAddTD( "[SocketItem] [%s][%s] Set Bonus Socket Option - OptionIndex : %d, EffectType : %d, OptionType : %d, OptionValue : %d",
			lpObj->AccountID, lpObj->Name,
			lpBonusOptionData->OptionIndex, lpBonusOptionData->EffectType, lpBonusOptionData->OptionValueType, lpBonusOptionData->OptionValue );
#endif // ADD_SOCKET_OPTION_LOG_20080715

		SetEffect( lpObj, SOCKET_OPTION_TYPE_BONUS, lpBonusOptionData->EffectType, lpBonusOptionData->OptionValueType, lpBonusOptionData->OptionValue );
	}
}

// 4-3. ��Ʈ ���� �ɼ��� ȿ���� ����
void CItemSocketOptionSystem::ApplySetOptionEffect(LPOBJECTSTRUCT lpObj)
{
	BYTE btTotalSeedTypeList[SEED_TYPE_MAX];
	BYTE btSeedType = 0;
	BYTE btMatchedCount = 0;

	memset( btTotalSeedTypeList, 0, SEED_TYPE_MAX );

	for( int iUserSocketIndex = 0 ; iUserSocketIndex < MAX_SEEDSPHERE_OPTION_DATA ; iUserSocketIndex++ )
	{
		btSeedType = GetSocketType( lpObj->m_SeedOptionList[iUserSocketIndex].m_SocketOptionIndex );
		if( btSeedType == SEED_TYPE_NONE )
		{
			break;
		}

		btTotalSeedTypeList[btSeedType-1] += lpObj->m_SeedOptionList[iUserSocketIndex].m_SocketOptionCount;
	}

	for( int iSetOptionIndex = 0 ; iSetOptionIndex < MAX_SET_OPTION ; iSetOptionIndex++ )
	{
		for( int i = 0 ; i < 6 ; i++ )
		{
			if( m_SetSocketOptionData[iSetOptionIndex].SeedTypeTable[i] <= btTotalSeedTypeList[i] )
			{
				btMatchedCount++;
			}
		}

		if( btMatchedCount == 6 )
		{
			SetEffect( lpObj, SOCKET_OPTION_TYPE_SET, m_SetSocketOptionData[iSetOptionIndex].EffectType, m_SetSocketOptionData[iSetOptionIndex].OptionValueType, m_SetSocketOptionData[iSetOptionIndex].OptionValue );
		}

		btMatchedCount = 0;
	}
}

// 4-4. ��� ���� �������� �ɼ��� ����
bool CItemSocketOptionSystem::SetApplySocketEffect(LPOBJECTSTRUCT lpObj)
{
	int iTotalSeedSphereCount = 0;

	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->Type != OBJTYPE_CHARACTER || lpObj->Connected < CSS_GAMEPLAYING )
	{
		return false;
	}

	// ���� ������� ȿ���� �ʱ�ȭ
	SetClearSocketEffect( lpObj );

	iTotalSeedSphereCount = ApplySeedSphereEffect( lpObj );

	// ���ʽ��� ��Ʈ ������ ȿ���� �õ� ȿ���� ������ �������� �ʴ´�.
	if( iTotalSeedSphereCount <= 0 )
	{
		return false;
	}

	ApplyBonusOptionEffect( lpObj );
	ApplySetOptionEffect( lpObj );

	return true;
}

// 4-5. ������� ���� ������ ȿ���� ���
void CItemSocketOptionSystem::SetClearSocketEffect(LPOBJECTSTRUCT lpObj)
{
	if( lpObj == NULL )
	{
		return;
	}

	if( lpObj->Type != OBJTYPE_CHARACTER || lpObj->Connected < CSS_GAMEPLAYING )
	{
		return;
	}

	int iIndex = 0;

	for( iIndex = 0 ; iIndex < MAX_SEEDSPHERE_OPTION_DATA ; iIndex++ )
	{
		if( lpObj->m_SeedOptionList[iIndex].m_SocketOptionIndex == SOCKETSLOT_NONE )
		{
			break;
		}

		lpObj->m_SeedOptionList[iIndex].Clear();
	}

	for( iIndex = 0 ; iIndex < MAX_BONUS_OPTION_DATA ; iIndex++ )
	{
		if( lpObj->m_BonusOptionList[iIndex].m_SocketOptionIndex == SOCKETSLOT_NONE )
		{
			break;
		}

		lpObj->m_BonusOptionList[iIndex].Clear();
	}

	for( iIndex = 0 ; iIndex < MAX_SET_OPTION_DATA ; iIndex++ )
	{
		if( lpObj->m_SetOptionList[iIndex].m_SocketOptionIndex == SOCKETSLOT_NONE )
		{
			break;
		}

		lpObj->m_SetOptionList[iIndex].Clear();
	}
	
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	lpObj->m_btRefillHPSocketOption = 0;
	lpObj->m_btRefillMPSocketOption = 0;
	lpObj->m_wSocketOptionMonsterDieGetHp		= 0;
	lpObj->m_wSocketOptionMonsterDieGetMana		= 0;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	lpObj->m_btAGReduceRate = 0;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
}

// 4-6. ������ ȿ���� ���� �����ϱ� ���� �Լ�
void CItemSocketOptionSystem::SetEffect(LPOBJECTSTRUCT lpObj, BYTE btOptionType, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	if( btEffectType >= SEED_EFFECT_TYPE_FIRE_BEGIN && btEffectType <= SEED_EFFECT_TYPE_FIRE_END )
	{
		_SetFireEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}
	else if( btEffectType >= SEED_EFFECT_TYPE_WATER_BEGIN && btEffectType <= SEED_EFFECT_TYPE_WATER_END )
	{
		_SetWaterEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}
	else if( btEffectType >= SEED_EFFECT_TYPE_ICE_BEGIN && btEffectType <= SEED_EFFECT_TYPE_ICE_END )
	{
		_SetIceEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}
	else if( btEffectType >= SEED_EFFECT_TYPE_WIND_BEGIN && btEffectType <= SEED_EFFECT_TYPE_WIND_END )
	{
		_SetWindEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}
	else if( btEffectType >= SEED_EFFECT_TYPE_THUNDER_BEGIN && btEffectType <= SEED_EFFECT_TYPE_THUNDER_END )
	{
		_SetThunderEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}
	else if( btEffectType >= SEED_EFFECT_TYPE_EARTH_BEGIN && btEffectType <= SEED_EFFECT_TYPE_EARTH_END )
	{
		_SetEarthEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}
	else if( btEffectType >= SEED_EFFECT_TYPE_UNIQUE_BEGIN && btEffectType <= SEED_EFFECT_TYPE_UNIQUE_END )
	{
		_SetUniqueEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}
	else if( btEffectType >= SEED_EFFECT_TYPE_SET_BEGIN && btEffectType <= SEED_EFFECT_TYPE_SET_END )
	{
		_SetSetEffect( lpObj, btEffectType, btEffectValueType, wEffectValue );
	}

	for( int i = 0 ; i < MAX_USER_SOCKET_OPTION_DATA ; i++ )
	{
		// ���� �ɼ� ����Ʈ�� ���������� ���� ����� ����.
		if( lpObj->m_SeedOptionList[i].m_SocketOptionIndex == btEffectType && lpObj->m_SeedOptionList[i].m_SocketOptionValueType == btEffectValueType )
		{
			lpObj->m_SeedOptionList[i].m_SocketOptionValue += wEffectValue;
			lpObj->m_SeedOptionList[i].m_SocketOptionCount++;
			break;
		}

		if( lpObj->m_SeedOptionList[i].m_SocketOptionIndex == SOCKETSLOT_NONE )
		{
			lpObj->m_SeedOptionList[i].m_SocketOptionIndex		= btEffectType;
			lpObj->m_SeedOptionList[i].m_SocketOptionValueType	= btEffectValueType;
			lpObj->m_SeedOptionList[i].m_SocketOptionValue		= wEffectValue;
			lpObj->m_SeedOptionList[i].m_SocketOptionCount		= 1;
			break;
		}
	}
}

// 4-7. �� �Ӽ��� ȿ���� ���
void CItemSocketOptionSystem::ClearEffect(LPOBJECTSTRUCT lpObj, LPSOCKET_OPTION_LIST lpOptionData)
{

}

// 4-8. �� �Ӽ��� ȿ���� ����
void CItemSocketOptionSystem::_SetFireEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	int iCalcEffectValue = 0;

	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_ATTACK_POWER:
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMaxLeft		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMinLeft		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMaxRight		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMinRight		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMin				+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMax				+= iCalcEffectValue;
		break;
	case SEED_EFFECT_TYPE_ATTACK_SPEED:
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackSpeed, wEffectValue, btEffectValueType );
		lpObj->m_AttackSpeed				+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicSpeed, wEffectValue, btEffectValueType );
		lpObj->m_MagicSpeed					+= iCalcEffectValue;
		break;
	case SEED_EFFECT_TYPE_MAX_ATTACK_POWER:
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMaxLeft		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMaxRight		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMax				+= iCalcEffectValue;
		break;
	case SEED_EFFECT_TYPE_MIN_ATTACK_POWER:
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMinLeft		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMinRight		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMin				+= iCalcEffectValue;
		break;
	case SEED_EFFECT_TYPE_MIN_MAX_ATTACK_POWER:
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMaxLeft		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMinLeft		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMaxRight		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType );
		lpObj->m_AttackDamageMinRight		+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMin				+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMax				+= iCalcEffectValue;
		break;
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	case SEED_EFFECT_TYPE_REDUCE_AG_RATE:
		lpObj->m_btAGReduceRate				+= wEffectValue;
		if( lpObj->m_btAGReduceRate > 100 )
		{
			lpObj->m_btAGReduceRate = 100;
		}
		break;
#else
	case SEED_EFFECT_TYPE_ATTACK_SKILL_POWER:
		iCalcEffectValue = CalcEffectValue( lpObj->SetOpAddSkillAttack, wEffectValue, btEffectValueType );
		lpObj->SetOpAddSkillAttack			+= iCalcEffectValue;
		break;
	case SEED_EFFECT_TYPE_ATTACK_ACCURACY:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_AttackRating				+= wEffectValue;
#else
		iCalcEffectValue = CalcEffectValue( lpObj->m_AttackRating, wEffectValue, btEffectValueType );
		lpObj->m_AttackRating				+= iCalcEffectValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	case SEED_EFFECT_TYPE_PERSONAL_ATTACK_ACCURACY:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP	+= wEffectValue;
#else
		iCalcEffectValue = CalcEffectValue( lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP, wEffectValue, btEffectValueType );
		lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP	+= iCalcEffectValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	case SEED_EFFECT_TYPE_MAGIC_POWER:
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMin				+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType );
		lpObj->m_MagicDamageMax				+= iCalcEffectValue;
		break;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	}
#else

	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_ATTACK_POWER:
		lpObj->m_AttackDamageMaxLeft		+= wEffectValue;
		lpObj->m_AttackDamageMinLeft		+= wEffectValue;
		lpObj->m_AttackDamageMaxRight		+= wEffectValue;
		lpObj->m_AttackDamageMinRight		+= wEffectValue;
		lpObj->m_MagicDamageMin				+= wEffectValue;
		lpObj->m_MagicDamageMax				+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_ATTACK_SPEED:
		lpObj->m_AttackSpeed				+= wEffectValue;
		lpObj->m_MagicSpeed					+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_MAX_ATTACK_POWER:
		lpObj->m_AttackDamageMaxLeft		+= wEffectValue;
		lpObj->m_AttackDamageMaxRight		+= wEffectValue;
		lpObj->m_MagicDamageMax				+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_MIN_ATTACK_POWER:
		lpObj->m_AttackDamageMinLeft		+= wEffectValue;
		lpObj->m_AttackDamageMinRight		+= wEffectValue;
		lpObj->m_MagicDamageMin				+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_MIN_MAX_ATTACK_POWER:
		lpObj->m_AttackDamageMaxLeft		+= wEffectValue;
		lpObj->m_AttackDamageMinLeft		+= wEffectValue;
		lpObj->m_AttackDamageMaxRight		+= wEffectValue;
		lpObj->m_AttackDamageMinRight		+= wEffectValue;
		lpObj->m_MagicDamageMin				+= wEffectValue;
		lpObj->m_MagicDamageMax				+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_ATTACK_SKILL_POWER:
		lpObj->SetOpAddSkillAttack			+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_ATTACK_ACCURACY:
		lpObj->m_AttackRating				+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_PERSONAL_ATTACK_ACCURACY:
		lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP	+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_MAGIC_POWER:
		lpObj->m_MagicDamageMin				+= wEffectValue;
		lpObj->m_MagicDamageMax				+= wEffectValue;
		break;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}

void CItemSocketOptionSystem::_SetWaterEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	int iCalcEffectValue = 0;

	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_SUCCESS_BLOCK:
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		iCalcEffectValue = CalcEffectValue( lpObj->m_SuccessfulBlocking, wEffectValue, btEffectValueType );
		lpObj->m_SuccessfulBlocking			+= iCalcEffectValue;
#else
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_SuccessfulBlocking			+= wEffectValue;
	#else
		iCalcEffectValue = CalcEffectValue( lpObj->m_SuccessfulBlocking, wEffectValue, btEffectValueType );
		lpObj->m_SuccessfulBlocking			+= iCalcEffectValue;
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		break;
	case SEED_EFFECT_TYPE_BLOCK_POWER:
		iCalcEffectValue = CalcEffectValue( lpObj->m_Defense, wEffectValue, btEffectValueType );
		lpObj->m_Defense					+= iCalcEffectValue;
		iCalcEffectValue = CalcEffectValue( lpObj->m_MagicDefense, wEffectValue, btEffectValueType );
		lpObj->m_MagicDefense				+= iCalcEffectValue;
		break;
	case SEED_EFFECT_TYPE_SHIELD_BLOCK_POWER:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->SetOpImproveSheldDefence		+= wEffectValue;
#else
		iCalcEffectValue = CalcEffectValue( lpObj->SetOpImproveSheldDefence, wEffectValue, btEffectValueType );
		lpObj->SetOpImproveSheldDefence		+= iCalcEffectValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	case SEED_EFFECT_TYPE_REDUCE_DAMAGE:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->DamageMinus					+= wEffectValue;
#else
		iCalcEffectValue = CalcEffectValue( lpObj->DamageMinus, wEffectValue, btEffectValueType );
		lpObj->DamageMinus					+= iCalcEffectValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	case SEED_EFFECT_TYPE_REFLECT_DAMAGE:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->DamageReflect				+= wEffectValue;
#else
		iCalcEffectValue = CalcEffectValue( lpObj->DamageReflect, wEffectValue, btEffectValueType );
		lpObj->DamageReflect				+= iCalcEffectValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	}
#else
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_SUCCESS_BLOCK:
		lpObj->m_SuccessfulBlocking			+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_BLOCK_POWER:
		lpObj->m_Defense					+= wEffectValue;
		lpObj->m_MagicDefense				+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_SHIELD_BLOCK_POWER:
		lpObj->SetOpImproveSheldDefence		+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_REDUCE_DAMAGE:
		lpObj->DamageMinus					+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_REFLECT_DAMAGE:
		break;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}

void CItemSocketOptionSystem::_SetIceEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iAddValue = 0;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	switch( btEffectType )
	{
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_HP:
		iAddValue += CalcEffectValue( lpObj->MaxLife + lpObj->AddLife, wEffectValue, btEffectValueType );
		lpObj->m_wSocketOptionMonsterDieGetHp				+= iAddValue;
		break;
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_MP:
		iAddValue += CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->m_wSocketOptionMonsterDieGetMana				+= iAddValue;
		break;
#else
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_HP:
		iAddValue += CalcEffectValue( lpObj->MaxLife + lpObj->AddLife, wEffectValue, btEffectValueType );
		lpObj->MonsterDieGetLife							+= iAddValue;
		break;
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_MP:
		iAddValue += CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->MonsterDieGetMana							+= iAddValue;
		break;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	case SEED_EFFECT_TYPE_ATTACK_SKILL_POWER:
		iAddValue = CalcEffectValue( lpObj->SetOpAddSkillAttack, wEffectValue, btEffectValueType );
		lpObj->SetOpAddSkillAttack			+= iAddValue;
		break;
	case SEED_EFFECT_TYPE_ATTACK_ACCURACY:
		lpObj->m_AttackRating				+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_ITEM_DURABILITY_IMPROVE:
		break;
#else

	case SEED_EFFECT_TYPE_SD_POINT_REDUCE_RATE_INC:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
		lpObj->m_JewelOfHarmonyEffect.HJOpAddSDRate			+= wEffectValue;
#else
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate	+= wEffectValue;
	#else
		iAddValue += CalcEffectValue( lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate, wEffectValue, btEffectValueType );
		lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate	+= iAddValue;
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
		break;
	case SEED_EFFECT_TYPE_SD_IGNORE_RATE_INC:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate	+= wEffectValue;
#else
		iAddValue += CalcEffectValue( lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate, wEffectValue, btEffectValueType );
		lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate	+= iAddValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	}
#else
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_HP:
		iAddValue += CalcEffectValue( lpObj->MaxLife + lpObj->AddLife, wEffectValue, btEffectValueType );
		lpObj->MonsterDieGetLife							+= iAddValue;
		break;
	case SEED_EFFECT_TYPE_MONSTER_KILL_REFILL_MP:
		iAddValue += CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->MonsterDieGetMana							+= iAddValue;
		break;
	case SEED_EFFECT_TYPE_SD_POINT_REDUCE_RATE_INC:
		lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate	+= wEffectValue;
		break;
	case SEED_EFFECT_TYPE_SD_IGNORE_RATE_INC:
		lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate	+= wEffectValue;
		break;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}

void CItemSocketOptionSystem::_SetWindEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iAddValue = 0;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_REFILL_HP_INC:
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
		iAddValue = CalcEffectValue( lpObj->MaxLife + lpObj->AddLife, wEffectValue, btEffectValueType );
		lpObj->m_btRefillHPSocketOption += iAddValue;
#else
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_btRefillHPSocketOption += wEffectValue;
	#else
		iAddValue = CalcEffectValue( lpObj->MaxLife + lpObj->AddLife, wEffectValue, btEffectValueType );
		lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP += iAddValue;
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
		break;
	case SEED_EFFECT_TYPE_MAX_HP_INC:
		iAddValue = CalcEffectValue( lpObj->MaxLife + lpObj->AddLife, wEffectValue, btEffectValueType );
		lpObj->AddLife			+= iAddValue;
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield );
		break;
	case SEED_EFFECT_TYPE_MAX_MP_INC:
		iAddValue = CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->AddMana			+= iAddValue;
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
		GCManaSend( lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
	case SEED_EFFECT_TYPE_REFILL_MP_INC:
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
		iAddValue = CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->m_btRefillMPSocketOption += iAddValue;
#else
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_btRefillMPSocketOption += wEffectValue;
	#else
		iAddValue = CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP += iAddValue;
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
		break;
	case SEED_EFFECT_TYPE_MAX_AG_INC:
		iAddValue = CalcEffectValue( lpObj->MaxBP+lpObj->AddBP, wEffectValue, btEffectValueType );
		lpObj->AddBP += iAddValue;
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
		GCManaSend( lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
	case SEED_EFFECT_TYPE_REFILL_AG_INC:
		iAddValue = CalcEffectValue( lpObj->SetOpIncAGValue, wEffectValue, btEffectValueType );
		lpObj->SetOpIncAGValue += iAddValue;
		break;
	case SEED_EFFECT_TYPE_MONSTER_KILL_ZEN_INC:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->MonsterDieGetMoney += wEffectValue;
#else
		iAddValue = CalcEffectValue( lpObj->MonsterDieGetMoney, wEffectValue, btEffectValueType );
		lpObj->MonsterDieGetMoney += iAddValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	}
#else
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_REFILL_HP_INC:
		break;
	case SEED_EFFECT_TYPE_MAX_HP_INC:
		iAddValue = CalcEffectValue( lpObj->MaxLife + lpObj->AddLife, wEffectValue, btEffectValueType );
		lpObj->AddLife			+= iAddValue;
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield );
		break;
	case SEED_EFFECT_TYPE_MAX_MP_INC:
		iAddValue = CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->AddMana			+= iAddValue;
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
		GCManaSend( lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
	case SEED_EFFECT_TYPE_REFILL_MP_INC:
		iAddValue = CalcEffectValue( lpObj->MaxMana + lpObj->AddMana, wEffectValue, btEffectValueType );
		lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP += iAddValue;
		break;
	case SEED_EFFECT_TYPE_MAX_AG_INC:
		lpObj->AddBP += wEffectValue;
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
		GCManaSend( lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
	case SEED_EFFECT_TYPE_REFILL_AG_INC:
		lpObj->SetOpIncAGValue += wEffectValue;
		break;
	case SEED_EFFECT_TYPE_MONSTER_KILL_ZEN_INC:
		iAddValue = CalcEffectValue( lpObj->MonsterDieGetMoney, wEffectValue, btEffectValueType );
		lpObj->MonsterDieGetMoney += iAddValue;
		break;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}

void CItemSocketOptionSystem::_SetThunderEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iAddValue = 0;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_EXCELLENT_DAMAGE_INC:
		iAddValue = CalcEffectValue( lpObj->SetOpAddExDamage, wEffectValue, btEffectValueType );
		lpObj->SetOpAddExDamage += iAddValue;
		break;
	case SEED_EFFECT_TYPE_EXCELLENT_DAMAGE_RATE_INC:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_ExcelentDamage += wEffectValue;
#else
		iAddValue = CalcEffectValue( lpObj->m_ExcelentDamage, wEffectValue, btEffectValueType );
		lpObj->m_ExcelentDamage += iAddValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	case SEED_EFFECT_TYPE_CRITICAL_DAMAGE_INC:
		iAddValue = CalcEffectValue( lpObj->SetOpAddCriticalDamage, wEffectValue, btEffectValueType );
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		lpObj->SetOpAddCriticalDamage += iAddValue;
#else
		lpObj->SetOpAddExDamage += iAddValue;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		break;
	case SEED_EFFECT_TYPE_CRITICAL_DAMAGE_RATE_INC:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->m_CriticalDamage += wEffectValue;
#else
		iAddValue = CalcEffectValue( lpObj->m_CriticalDamage, wEffectValue, btEffectValueType );
		lpObj->m_CriticalDamage += iAddValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	}
#else
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_EXCELLENT_DAMAGE_INC:
		iAddValue = CalcEffectValue( lpObj->SetOpAddExDamage, wEffectValue, btEffectValueType );
		lpObj->SetOpAddExDamage += iAddValue;
		break;
	case SEED_EFFECT_TYPE_EXCELLENT_DAMAGE_RATE_INC:
		lpObj->m_ExcelentDamage += wEffectValue;
		break;
	case SEED_EFFECT_TYPE_CRITICAL_DAMAGE_INC:
		iAddValue = CalcEffectValue( lpObj->SetOpAddCriticalDamage, wEffectValue, btEffectValueType );
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		lpObj->SetOpAddCriticalDamage += iAddValue;
#else
		lpObj->SetOpAddExDamage += iAddValue;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		break;
	case SEED_EFFECT_TYPE_CRITICAL_DAMAGE_RATE_INC:
		lpObj->m_CriticalDamage += wEffectValue;
		break;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}

void CItemSocketOptionSystem::_SetEarthEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	int iAddValue = 0;

	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_STRENGTH_INC:
		iAddValue = CalcEffectValue( lpObj->Strength + lpObj->AddStrength, wEffectValue, btEffectValueType );
		lpObj->AddStrength += iAddValue;
		break;
	case SEED_EFFECT_TYPE_DEXTERITY_INC:
		iAddValue = CalcEffectValue( lpObj->Dexterity + lpObj->AddDexterity, wEffectValue, btEffectValueType );
		lpObj->AddDexterity += iAddValue;
		break;
	case SEED_EFFECT_TYPE_VITALITY_INC:
		iAddValue = CalcEffectValue( lpObj->Vitality + lpObj->AddVitality, wEffectValue, btEffectValueType );
		lpObj->AddVitality += iAddValue;
		break;
	case SEED_EFFECT_TYPE_ENERGY_INC:
		iAddValue = CalcEffectValue( lpObj->Energy + lpObj->AddEnergy, wEffectValue, btEffectValueType );
		lpObj->AddEnergy += iAddValue;
		break;
	case SEED_EFFECT_TYPE_REQUIRE_STRENGTH_DEC:
		break;
	case SEED_EFFECT_TYPE_REQUIRE_DEXTERITY_DEC:
		break;
	}
#else
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_STRENGTH_INC:
		lpObj->AddStrength += wEffectValue;
		break;
	case SEED_EFFECT_TYPE_DEXTERITY_INC:
		lpObj->AddDexterity += wEffectValue;
		break;
	case SEED_EFFECT_TYPE_VITALITY_INC:
		lpObj->AddVitality += wEffectValue;
		break;
	case SEED_EFFECT_TYPE_ENERGY_INC:
		lpObj->AddEnergy += wEffectValue;
		break;
	case SEED_EFFECT_TYPE_REQUIRE_STRENGTH_DEC:
		break;
	case SEED_EFFECT_TYPE_REQUIRE_DEXTERITY_DEC:
		break;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}

void CItemSocketOptionSystem::_SetUniqueEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_UNIQUE_1:
		break;
	case SEED_EFFECT_TYPE_UNIQUE_2:
		break;
	}
}

void CItemSocketOptionSystem::_SetSetEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	int iAddValue = 0;

	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_SET_DOUBLE_DAMAGE_RATE_INC:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->SetOpDoubleDamage += wEffectValue;
#else
		iAddValue = CalcEffectValue( lpObj->SetOpDoubleDamage, wEffectValue, btEffectValueType );
		lpObj->SetOpDoubleDamage += iAddValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	case SEED_EFFECT_TYPE_SET_IGNORE_ENERMY_DEFENSE:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		lpObj->SetOpIgnoreDefense += wEffectValue;
#else
		iAddValue = CalcEffectValue( lpObj->SetOpIgnoreDefense, wEffectValue, btEffectValueType );
		lpObj->SetOpIgnoreDefense += iAddValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		break;
	}
#else
	switch( btEffectType )
	{
	case SEED_EFFECT_TYPE_SET_DOUBLE_DAMAGE_RATE_INC:
		lpObj->SetOpDoubleDamage += wEffectValue;
		break;
	case SEED_EFFECT_TYPE_SET_IGNORE_ENERMY_DEFENSE:
		lpObj->SetOpIgnoreDefense += wEffectValue;
		break;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}

// 4-9. ȿ�� ������ ���� ��ġ�� ����ϱ� ���� �Լ�
int CItemSocketOptionSystem::CalcEffectValue(int iInputValue, WORD wEffectValue, BYTE btEffectValueType )
{
	int iReturnValue = 0;

	if( wEffectValue == 0 )
	{
		return 0;
	}

	switch( btEffectValueType )
	{
	case SEED_OPTION_TYPE_VALUE:
		iReturnValue = wEffectValue;
		break;
	case SEED_OPTION_TYPE_RATE:
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
		iReturnValue = ( iInputValue * wEffectValue ) / 100;
#else
		iReturnValue = ( iInputValue * 30 ) / 100;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
		break;
	case SEED_OPTION_TYPE_PER_LEVEL:
		iReturnValue = ( iInputValue / wEffectValue );
		break;
	case SEED_OPTION_TYPE_PER_HP:
		iReturnValue = ( iInputValue / wEffectValue );
		break;
	case SEED_OPTION_TYPE_PER_MP:
		iReturnValue = ( iInputValue / wEffectValue );
		break;
	default:
		return -1;
	}

	return iReturnValue;
}

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
// 4-10. �䱸��ġ ���� �ɼ��� �����ϱ� ���� �Լ�
void CItemSocketOptionSystem::SetRequireStatOption(CItem* lpItemdata)
{
	if( lpItemdata == NULL )
	{
		return;
	}

	if( lpItemdata->IsItem() == FALSE || IsSocketItem( lpItemdata ) == false )
	{
		return;
	}

	BYTE SeedIndex = 0;
	BYTE SphereLevel = 0;
	BYTE SeedSphereIndex = 0;
	CSeedSphereItem SeedSphere;

	int iReduceValue = 0;

	for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
	{
		if( lpItemdata->m_SocketOption[i] == SOCKETSLOT_NONE || lpItemdata->m_SocketOption[i] == SOCKETSLOT_EMPTY )
		{
			continue;
		}

		SeedSphereIndex = lpItemdata->m_SocketOption[i];
		SeedIndex = SeedSphereIndex % MAX_SEED_COUNT;
		SphereLevel = ( ( SeedSphereIndex - SeedIndex ) / MAX_SEED_COUNT ) + 1;

		if( MakeSeedSphere( &SeedSphere, SeedIndex, SphereLevel ) == false )
		{
			continue;
		}

		if( SeedIndex == SEED_EFFECT_TYPE_REQUIRE_STRENGTH_DEC )
		{
			iReduceValue = CalcEffectValue( lpItemdata->m_HJOpStrength, SeedSphere.m_SeedOptionValue, SeedSphere.m_SeedOptionValueType );
			lpItemdata->m_HJOpStrength += iReduceValue;
		}
		else if( SeedIndex == SEED_EFFECT_TYPE_REQUIRE_DEXTERITY_DEC )
		{
			iReduceValue = CalcEffectValue( lpItemdata->m_HJOpDexterity, SeedSphere.m_SeedOptionValue, SeedSphere.m_SeedOptionValueType );
			lpItemdata->m_HJOpDexterity += iReduceValue;
		}
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
		else if( SeedIndex == SEED_EFFECT_TYPE_ITEM_DURABILITY_IMPROVE )
		{
			lpItemdata->m_ImproveDurabilityRate += SeedSphere.m_SeedOptionValue;
		}
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	}
}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704

//////////////////////////////////////////////////////////////////////////
// 5. ���� �����ۿ� ���� ������ �����ϱ� ���� �Լ���.
// 5-1. �ش� �����ۿ� ���� ������ ����
void CItemSocketOptionSystem::MakeSocketSlot(CItem* lpItemData, BYTE btSocketSlotCount)
{
	lpItemData->MakeSocketSlot( btSocketSlotCount );
}

// 5-2. ���� ���� ���� Ȯ���� ���� ���� ������ ����
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
BYTE CItemSocketOptionSystem::GetMakeSocketSlotCount(int iItemCode)
#else
BYTE CItemSocketOptionSystem::GetMakeSocketSlotCount()
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
{
	int iSocketSlotCount = 0;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	// ������ Ÿ�Կ� ���� ���� ���� ������ Ʋ������.
	// �ش� �������� ��� �������� Ȯ���Ѵ�.
	if( ItemAttribute[iItemCode].HaveItemInfo == true )
	{
		if( ItemAttribute[iItemCode].TwoHand == false )
		{
			iSocketSlotCount = m_MakeSocketSlotCountPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
		}
		else
		{
			iSocketSlotCount = m_TwoHandWeaponMakeSocketSlotCountPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
		}
	}
#else
	iSocketSlotCount = m_MakeSocketSlotCountPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

	return (BYTE)iSocketSlotCount;
}

//////////////////////////////////////////////////////////////////////////
// 6. ���� ����� ������ Ȯ���� ���� �Լ���.
// 6-1. ����� ���� ������ �ʱ�ȭ
void CItemSocketOptionSystem::ClearUserData(LPOBJECTSTRUCT lpObj)
{
	int iIndex = 0;

	if( lpObj == NULL )
	{
		return;
	}

	if( lpObj->Type != OBJTYPE_CHARACTER )
	{
		return;
	}
	
	for( iIndex = 0 ; iIndex < MAX_SEEDSPHERE_OPTION_DATA ; iIndex++ )
	{
		lpObj->m_SeedOptionList[iIndex].Clear();
	}

	for( iIndex = 0 ; iIndex < MAX_BONUS_OPTION_DATA ; iIndex++ )
	{
		lpObj->m_BonusOptionList[iIndex].Clear();
	}

	for( iIndex = 0 ; iIndex < MAX_SET_OPTION_DATA ; iIndex++ )
	{
		lpObj->m_SetOptionList[iIndex].Clear();
	}
}

//////////////////////////////////////////////////////////////////////////
// 7. �õ彺�Ǿ ���� �����ۿ� ����/�ı� �ϱ� ���� �Լ���.
// 7-1. �õ彺�Ǿ ���Ͽ� ���� (ī���� ������ ���� ���� �����ۿ� �õ彺�Ǿ �����Ѵ�.)
bool CItemSocketOptionSystem::SetSocketSlot(CItem* lpTargetItem, CItem* lpSeedSphere, BYTE btSocketSlotIndex)
{
	if( lpTargetItem == NULL || lpSeedSphere == NULL )
	{
		return false;
	}

	if( lpTargetItem->IsItem() == FALSE || lpSeedSphere->IsItem() == FALSE )
	{
		return false;
	}

	if( IsEnableSocketItem( lpTargetItem ) == false )
	{
		return false;
	}

	if( btSocketSlotIndex > MAX_SOCKET_SLOT )
	{
		return false;
	}

	// �ش� ���� ������ ��� �ִ��� Ȯ���Ѵ�.
	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] != SOCKETSLOT_EMPTY )
	{
		return false;
	}

	BYTE btItemIndex = 0;
	BYTE btSeedIndex = 0;
	BYTE btSphereLevel = 0;
	BYTE btSeedSphereIndex = 0;

	btItemIndex = lpSeedSphere->m_Type % MAX_ITEM_INDEX;

	// �õ� �ε����� ���´�.
	btSeedIndex = ( btItemIndex - 99 );
	// ���Ǿ� ������ ���´�.
	btSphereLevel = ( ( btSeedIndex - 1 ) / 6 ) + 1;
	// �õ彺�Ǿ� �ε����� ���´�.
	btSeedSphereIndex = ( btSphereLevel * 50 ) + btSeedIndex;

	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;

	SetBonusSocketOption( lpTargetItem );

	return true;
}

bool CItemSocketOptionSystem::SetSocketSlot(CItem* lpTargetItem, BYTE btSeedIndex, BYTE btSphereLevel, BYTE btSocketSlotIndex)
{
	if( lpTargetItem == NULL )
	{
		return false;
	}

	if( lpTargetItem->IsItem() == FALSE )
	{
		return false;
	}

	if( IsEnableSocketItem( lpTargetItem ) == false )
	{
		return false;
	}

	if( btSocketSlotIndex > MAX_SOCKET_SLOT )
	{
		return false;
	}

	// �ش� ���� ������ ��� �ִ��� Ȯ���Ѵ�.
	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] != SOCKETSLOT_EMPTY )
	{
		return false;
	}

	BYTE btSeedSphereIndex = 0;

	// �õ彺�Ǿ� �ε����� ���´�.
	btSeedSphereIndex = ( btSphereLevel * 50 ) + btSeedIndex;

	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;

	SetBonusSocketOption( lpTargetItem );

	return true;
}

bool CItemSocketOptionSystem::SetSocketSlot(CItem* lpTargetItem, BYTE btSeedSphereIndex, BYTE btSocketSlotIndex)
{
	if( lpTargetItem == NULL )
	{
		return false;
	}

	if( lpTargetItem->IsItem() == FALSE )
	{
		return false;
	}

	if( IsEnableSocketItem( lpTargetItem ) == false )
	{
		return false;
	}

	if( btSocketSlotIndex > MAX_SOCKET_SLOT )
	{
		return false;
	}

	// �ش� ���� ������ ��� �ִ��� Ȯ���Ѵ�.
	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] != SOCKETSLOT_EMPTY )
	{
		return false;
	}

	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;

	SetBonusSocketOption( lpTargetItem );

	return true;
}

// 7-2. �õ彺�Ǿ� ���� �Ŀ� ���ʽ� ���� �ɼ��� ���� �Ǵ��� Ȯ��
bool CItemSocketOptionSystem::SetBonusSocketOption(CItem* lpItemData)
{
	if( lpItemData == NULL )
	{
		return false;
	}

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	if( lpItemData->IsItem() == FALSE || IsEnableSocketItem( lpItemData ) == false )
#else
	if( lpItemData->IsItem() == FALSE || lpItemData->IsEnableSetSocketItem() == false )
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	{
		return false;
	}

	BYTE SeedSphereType[MAX_SOCKET_SLOT];

	// �켱 �ش� ������ ������ �õ彺�Ǿ� Ÿ���� ���´�.
	for( int iSlotIndex = 0 ; iSlotIndex < MAX_SOCKET_SLOT ; iSlotIndex++ )
	{
		if( lpItemData->m_SocketOption[iSlotIndex] != SOCKETSLOT_NONE && lpItemData->m_SocketOption[iSlotIndex] != SOCKETSLOT_EMPTY )
		{
			SeedSphereType[iSlotIndex] = GetSocketType( lpItemData->m_SocketOption[iSlotIndex] );
		}
	}

	BYTE btItemType = 0;
	WORD wItemIndex = 0;
	BYTE btSocketSlotCount = 0;

	wItemIndex = lpItemData->m_Type % MAX_ITEM_INDEX;
	btItemType = ( lpItemData->m_Type - wItemIndex ) / MAX_ITEM_INDEX;

	for( int iBonusIndex = 0 ; iBonusIndex < MAX_BONUS_OPTION ; iBonusIndex++ )
	{
		if( btItemType >= m_BonusSocketOptionData[iBonusIndex].ItemTypeBegin && btItemType <= m_BonusSocketOptionData[iBonusIndex].ItemTypeEnd )
		{
			for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
			{
				if( m_BonusSocketOptionData[iBonusIndex].SocketSlotType[i] == SeedSphereType[i]
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
					&& ( SeedSphereType[i] != SEED_TYPE_NONE )
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
				  )
				{
					btSocketSlotCount++;
				}

				if( btSocketSlotCount >= m_BonusSocketOptionData[iBonusIndex].SocketSlotCount )
				{
					if( rand() % 100 < m_BonusSocketOptionData[iBonusIndex].ExtractRate )
					{
						lpItemData->m_BonusSocketOption = m_BonusSocketOptionData[iBonusIndex].OptionIndex;
#ifdef ADD_SOCKET_OPTION_LOG_20080715
						LogAddTD( "[SocketItem] Set Bonus Socket Option Success - OptionIndex : %d, ItemCode : %d, SocketOption[%d,%d,%d,%d,%d]",
							lpItemData->m_BonusSocketOption, lpItemData->m_Type, 
							lpItemData->m_SocketOption[0],
							lpItemData->m_SocketOption[1],
							lpItemData->m_SocketOption[2],
							lpItemData->m_SocketOption[3],
							lpItemData->m_SocketOption[4] );
#endif // ADD_SOCKET_OPTION_LOG_20080715
						return true;
					}
					else
					{
						lpItemData->m_BonusSocketOption = SOCKETSLOT_NONE;
						return false;
					}
				}
			}
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
			btSocketSlotCount = 0;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		}
	}

	lpItemData->m_BonusSocketOption = SOCKETSLOT_NONE;

	return false;
}

// 7-3. �õ彺�Ǿ ���Ͽ��� ����
bool CItemSocketOptionSystem::ClearSocketSlot(CItem* lpTargetItem, BYTE btSocketSlotIndex)
{
	if( lpTargetItem == NULL )
	{
		return false;
	}

	if( IsEnableSocketItem( lpTargetItem ) == false )
	{
		return false;
	}

	if( btSocketSlotIndex > MAX_SOCKET_SLOT )
	{
		return false;
	}

	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] == SOCKETSLOT_NONE || lpTargetItem->m_SocketOption[btSocketSlotIndex] == SOCKETSLOT_EMPTY )
	{
		return false;
	}

	lpTargetItem->m_SocketOption[btSocketSlotIndex] = SOCKETSLOT_EMPTY;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	SetBonusSocketOption( lpTargetItem );
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704

	return true;
}

#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415