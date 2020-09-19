// GamblingSystem.cpp: implementation of the CGamblingSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GamblingSystem.h"

#include "..\\common\\winutil.h"
#include "..\\Include\\ReadScript.h"
#include "..\\common\\zzzitem.h"

#include "..\\Include\\public.h"
#include "..\\Include\\ProDef.h"
#include "..\\Include\\SproDef.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Common\\ZzzPath.h"
#include "SProtocol.h"
#include "User.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "Gamemain.h"

#include "DirPath.h"
#include "ItemBag.h"

#include "LargeRand.h"

#include "..\\Common\\SetItemOption.h"

extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];
extern CDirPath		gDirPath;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGamblingItemBag::CGamblingItemBag()
{
}

CGamblingItemBag::~CGamblingItemBag()
{
}

void CGamblingItemBag::GamblingItemBagInit(char * name)
{	
	m_bLoad = FALSE;
	m_iBagObjectCount = 0;
	m_iRateKindCount = 0;

	for( int i=0; i<ADD_OPTION_COUNT; ++i )
		m_iAddOption[i] = 0;

	for( int i=0; i<GAMBILNG_ITEM_TYPE_COUNT; ++i )
		m_iGamblingItemGetRate[i] = 0;

	for( int i=0; i<ATTACH_OPTION_COUNT; ++i )
		m_iGamblingOptionCountRate[i];

	for( int i=0; i<EX_OPTION_COUNT; ++i )
	{
		m_iGamblingExOptionRate[i];
		m_iNormalExOptionRate[i];
	}

	for( int i=0; i<ITEM_GROUP_MAX; ++i )
		m_iDropRatePerItemCount[i] = 0;
	
	
	LoadItem(gDirPath.GetNewPath(name));	
}

void CGamblingItemBag::LoadItem( char *script_file )
{
	m_bLoad = FALSE;

	m_iBagObjectCount = 0;

	if((SMDFile=fopen(script_file,"r")) == NULL)	
	{
		LogAdd(lMsg.Get(453), script_file);
		return ;
	}	
	
	SMDToken Token;
	int nDropRatePerItemCnt = 0;
	int nRateCnt			= 0;
	int iCount				= 0;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			if( Type == 0 )
			{	// �����ۿ� ��ų, ���, �߰��ɼ�, ���ɼ��� ���� Ȯ ��
				iCount = 0;

				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					if( iCount > ITEM_GROUP_MAX )
					{
						MsgBox("Load Script Error %s", script_file);
						break;		// �迭�� ������ �Ѿ��.
					}

					m_stGamblingRate[iCount].iSkillRate		= (int)TokenNumber;
					Token = (*GetToken)();	m_stGamblingRate[iCount].iLuckRate		= (int)TokenNumber;
					Token = (*GetToken)();	m_stGamblingRate[iCount].iAddOptionRate	= (int)TokenNumber;
					Token = (*GetToken)();	m_stGamblingRate[iCount].iExcellentRate	= (int)TokenNumber;
					Token = (*GetToken)();	m_stGamblingRate[iCount].iSelectedRate	= (int)TokenNumber;

					++iCount;
				}
				
			}
			else if( Type == 1 )
			{	// �� �߰� �ɼ��� ���� Ȯ��
				iCount = 0;

				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					if( iCount > ADD_OPTION_COUNT )
					{
						MsgBox("Load Script Error %s", script_file);
						break;		// �迭�� ������ �Ѿ��.
					}

					m_iAddOption[iCount]	= (int)TokenNumber;

					++iCount;
				}
			}
			else if( Type == 2 )
			{	// �� Ÿ�Ժ� �׺� ������ ���� Ȯ��
				iCount = 0;

				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;
					
					if( iCount > GAMBILNG_ITEM_TYPE_COUNT )
					{
						MsgBox("Load Script Error %s", script_file);
						break;		// �迭�� ������ �Ѿ��.
					}
					
					m_iGamblingItemGetRate[iCount]	= (int)TokenNumber;
					
					++iCount;
				}
			}
			else if( Type == 3 )
			{	// �׺� �����ۿ� �ɼ��� �������� ���� Ȯ�� ( ������ 10000�� �Ǿ���)
				iCount = 0;

				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;
					
					if( iCount > ATTACH_OPTION_COUNT )
					{
						MsgBox("Load Script Error %s", script_file);
						break;		// �迭�� ������ �Ѿ��.
					}
					
					m_iGamblingOptionCountRate[iCount]	= (int)TokenNumber;
					
					++iCount;
				}
			}
			else if( Type == 4 )
			{	// �� ������Ʈ �ɼ��� ���� Ȯ�� ( ������ 10000�� �Ǿ���)
				iCount = 0;

				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;
					
					if( iCount > EX_OPTION_COUNT )
					{
						MsgBox("Load Script Error %s", script_file);
						break;		// �迭�� ������ �Ѿ��.
					}
					
					m_iGamblingExOptionRate[iCount]	= (int)TokenNumber;
					 Token = (*GetToken)();	m_iNormalExOptionRate[iCount]	= (int)TokenNumber;
					
					++iCount;
				}
			}
			else if( Type >= 5 )
			{	// ������ ����Ʈ
				nDropRatePerItemCnt = 0;

				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL)
					{
						m_iDropRatePerItemCount[nRateCnt] = nDropRatePerItemCnt;
						break;
					}

					BagObject[m_iBagObjectCount].m_type = (int)TokenNumber;					

					Token = (*GetToken)();	BagObject[m_iBagObjectCount].m_index = (int)TokenNumber;

					Token = (*GetToken)();	BagObject[m_iBagObjectCount].m_minLevel = (int)TokenNumber;
					Token = (*GetToken)();	BagObject[m_iBagObjectCount].m_maxLevel = (int)TokenNumber;
					Token = (*GetToken)();	BagObject[m_iBagObjectCount].m_isskill	= (int)TokenNumber;
					Token = (*GetToken)();	BagObject[m_iBagObjectCount].m_isluck	= (int)TokenNumber;
					Token = (*GetToken)();	BagObject[m_iBagObjectCount].m_isoption = (int)TokenNumber;
					Token = (*GetToken)();	BagObject[m_iBagObjectCount].m_isexitem = (int)TokenNumber;

					if(BagObject[m_iBagObjectCount].m_minLevel > BagObject[m_iBagObjectCount].m_maxLevel)
					{
						MsgBox("Load Script Error %s", script_file);
						return;
					}

					nDropRatePerItemCnt++;
					m_iBagObjectCount++;
					if( m_iBagObjectCount > MAX_ITEMBAGEX-1 ) break;
				}
				
				nRateCnt++;
				if( nRateCnt > ITEM_GROUP_MAX-1 ) break;

				m_iRateKindCount = nRateCnt;
			}
		}
	}

	fclose(SMDFile);	
	LogAdd(lMsg.Get(454), script_file);

	m_bLoad = TRUE;
}

BYTE CGamblingItemBag::GetLevel(int n)
{
	if( n < 0 || n > MAX_ITEMBAGEX-1 ) return 0;
	
	if( BagObject[n].m_minLevel == BagObject[n].m_maxLevel )
	{
		return BagObject[n].m_minLevel;
	}
	
	int sub = BagObject[n].m_maxLevel - BagObject[n].m_minLevel + 1;
	int level = BagObject[n].m_minLevel+rand()%sub;
	
	return level;
}

BYTE CGamblingItemBag::GetGamblingSystemItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY, BYTE btItemType )
{
#ifdef ADD_IS_ENABLED_GAMBLING_20090504
	if( g_bGamblingSystem == FALSE )
	{
		return FALSE;
	}
#endif // ADD_IS_ENABLED_GAMBLING_20090504

	if( !m_bLoad )					// ������ ��ũ��Ʈ�� �ε���� �ʾҴ�
	{
		return FALSE;
	}
	
	float fDur		= 0;
	int iType		= 0;
	int iLevel		= 0;
	int X			= 0;
	int Y			= 0;
	int iOption1	= 0;
	int iOption2	= 0;
	int iOption3	= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption	= 0;
	
	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{
		if( m_iGamblingItemGetRate[btItemType] >= GetLargeRand()%1000000 )
		{
			// �׺� �������̴�.
			iLevel = 0;
			fDur = 0;
			iOption1 = 1;
			iOption2 = 0;

			// �߰� �ɼ� ����
			DWORD dwOptionTemp = GetLargeRand()%1000000;
			
			if( dwOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4] )
			{
				iOption3 = 1;
			}
			else if( dwOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4]+m_iAddOption[ADD_OPTION_PLUS_8] )
			{
				iOption3 = 2;
			}
			else if( dwOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4]+m_iAddOption[ADD_OPTION_PLUS_8]
				+m_iAddOption[ADD_OPTION_PLUS_12])
			{
				iOption3 = 3;
			}
			else if( dwOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4]+m_iAddOption[ADD_OPTION_PLUS_8]
				+m_iAddOption[ADD_OPTION_PLUS_12]+m_iAddOption[ADD_OPTION_PLUS_16] )
			{
				iOption3 = 4;
			}


			DWORD dwExOptionTemp = GetLargeRand()%1000000;

			if( dwExOptionTemp <= m_iGamblingOptionCountRate[ATTACH_1_OPTION_RATE] )
			{
				iExOption = GamblingOptionRand( ATTACH_1_OPTION_RATE+1 );
			}
			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[ATTACH_1_OPTION_RATE]
				+ m_iGamblingOptionCountRate[ATTACH_2_OPTION_RATE] )
			{
				iExOption = GamblingOptionRand( ATTACH_2_OPTION_RATE+1 );
			}
			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[ATTACH_1_OPTION_RATE]
				+ m_iGamblingOptionCountRate[ATTACH_2_OPTION_RATE] + m_iGamblingOptionCountRate[ATTACH_3_OPTION_RATE] )
			{
				iExOption = GamblingOptionRand( ATTACH_3_OPTION_RATE+1 );
			}
			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[ATTACH_1_OPTION_RATE]
				+ m_iGamblingOptionCountRate[ATTACH_2_OPTION_RATE] + m_iGamblingOptionCountRate[ATTACH_3_OPTION_RATE]
				+ m_iGamblingOptionCountRate[ATTACH_4_OPTION_RATE] )
			{
				iExOption = GamblingOptionRand( ATTACH_4_OPTION_RATE+1 );
			}

			switch( btItemType )
			{
			case GAMBILNG_ITEM_TYPE_SWORD:
				iType = ItemGetNumberMake( 3, 11 );	// �׺��� ��
				break;
			case GAMBILNG_ITEM_TYPE_STAFF:
				iType = ItemGetNumberMake( 5, 33 );	// �׺��� ������
				break;
			case GAMBILNG_ITEM_TYPE_BOW:
				iType = ItemGetNumberMake( 4, 24 );	// �׺��� Ȱ
				break;
			case GAMBILNG_ITEM_TYPE_SCEPTER:
				iType = ItemGetNumberMake( 2, 18 );	// �׺��� ����
				break;
			case GAMBILNG_ITEM_TYPE_STICK:
				iType = ItemGetNumberMake( 5, 34 );	// �׺��� ��ƽ
				break;
			}
		}
		else	// �׺� �������� �ƴϴ�.
		{
			// BagObject[]������ �ش� ������ �׷��� ������ ã�´�.
			int iCount = 0;
			int iBagPositionStart = 0;
			int iBagPositionEnd = 0;
			int iSelectedGroup = 0;
			
			switch( btItemType )
			{
			case GAMBILNG_ITEM_TYPE_SWORD:	iSelectedGroup = ITEM_GROUP_NORMAL_SWORD_1;	break;
			case GAMBILNG_ITEM_TYPE_STAFF:	iSelectedGroup = ITEM_GROUP_NORMAL_STAFF_1;	break;
			case GAMBILNG_ITEM_TYPE_BOW:	iSelectedGroup = ITEM_GROUP_NORMAL_BOW_1;	break;
			case GAMBILNG_ITEM_TYPE_SCEPTER:	iSelectedGroup = ITEM_GROUP_NORMAL_SCEPTER_1;	break;
			case GAMBILNG_ITEM_TYPE_STICK:	iSelectedGroup = ITEM_GROUP_NORMAL_STICK_1;	break;
			}

			DWORD dwRandTemp = GetLargeRand()%1000000;
			int iSelectedRate = 0;
			for( int i=iSelectedGroup; i<iSelectedGroup+5; ++i )
			{
				iSelectedRate += m_stGamblingRate[i].iSelectedRate;

				if( dwRandTemp <= iSelectedRate )
				{
					iSelectedGroup = i;
					break;
				}
			}
	
			while( TRUE )
			{
				iBagPositionEnd += m_iDropRatePerItemCount[iCount];

				if( iSelectedGroup <= iCount )
					break;
				
				iBagPositionStart += m_iDropRatePerItemCount[iCount];
				
				++iCount;
			}

			iDropItemNum = ( (GetLargeRand() % (iBagPositionEnd-iBagPositionStart)) + iBagPositionStart );
			
			fDur  = 0;
			X   = 0;
			Y   = 0;
			
			// ����
			iLevel	= GetLevel( iDropItemNum );
			iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );
			
			if( iType == -1 )
			{
				return FALSE;
			}

			if( BagObject[iDropItemNum].m_isskill )
			{
				// ��ų�� ������ ������
				if( m_stGamblingRate[iSelectedGroup].iSkillRate >= (GetLargeRand()%1000000) )
					iOption1 = 1;
			}

			if( BagObject[iDropItemNum].m_isluck )
			{	
				// ����� ������ ������
				if( m_stGamblingRate[iSelectedGroup].iLuckRate >= GetLargeRand()%1000000 )
					iOption2 = 1;
			}
			
			if( BagObject[iDropItemNum].m_isoption )
			{	// �߰� �ɼ��� ���� �� �ִٸ�
				if( m_stGamblingRate[iSelectedGroup].iAddOptionRate >= GetLargeRand()%1000000 )
				{
					int iOptionTemp = GetLargeRand()%1000000;

					if( iOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4] )
					{
						iOption3 = 1;
					}
					else if( iOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4]+m_iAddOption[ADD_OPTION_PLUS_8] )
					{
						iOption3 = 2;
					}
					else if( iOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4]+m_iAddOption[ADD_OPTION_PLUS_8]
											+m_iAddOption[ADD_OPTION_PLUS_12])
					{
						iOption3 = 3;
					}
					else if( iOptionTemp <= m_iAddOption[ADD_OPTION_PLUS_4]+m_iAddOption[ADD_OPTION_PLUS_8]
											+m_iAddOption[ADD_OPTION_PLUS_12]+m_iAddOption[ADD_OPTION_PLUS_16])
					{
						iOption3 = 4;
					}
				}
			}
			
			if( BagObject[iDropItemNum].m_isexitem )
			{
				// ������Ʈ�� ���ɼ��� �ִٸ�
				if( m_stGamblingRate[iSelectedGroup].iExcellentRate >= GetLargeRand()%1000000 )	// Ȯ�� üũ
				{
					// �����̶�°� Ȯ����
					iExOption = ExcellentOptionRand();
				
					// �׼���Ʈ �������� ����� ���� �ʴ´�.
#ifdef MODIFY_GAMBLING_ATTACH_LUCK_OPTION_20090901
					//iOption2 = 0;
#else // MODIFY_GAMBLING_ATTACH_LUCK_OPTION_20090901
					iOption2 = 0;
#endif // MODIFY_GAMBLING_ATTACH_LUCK_OPTION_20090901
					iOption1 = 1;
					
					// ������ 0�̴�.
					iLevel	= 0;
				}
			}			
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
	}
	
	return TRUE;
}

BYTE CGamblingItemBag::GamblingOptionRand( int iOptionCount )
{
	BYTE NOption=0;
	int iFailCount = 0;
	int iCount = 0;

	while( TRUE )
	{
		if( m_iGamblingExOptionRate[iCount] >= (GetLargeRand()%1000000) )
		{
			NOption |= 1<<iCount;
		}
		else
			++iFailCount;

		if( iFailCount >= (EX_OPTION_COUNT-iOptionCount) )
		{
			while( TRUE )
			{
				++iCount;

				if( iCount >= EX_OPTION_COUNT )
					break;

				NOption |= 1<<iCount;
			}

			break;
		}

		++iCount;
	}
	
	return NOption;
}

BYTE CGamblingItemBag::ExcellentOptionRand()
{
	BYTE NOption=0;
	int iCount = 0;
	int iSuccessCount = 0;

	while( TRUE )
	{
		if( m_iNormalExOptionRate[iCount] >= (GetLargeRand()%1000000) )
		{
			NOption |= 1<<iCount;
			++iSuccessCount;
		}

		++iCount;

		if( iSuccessCount == 0 )			// �ɼ��� �� �پ����Ƿ� �ٽ� ���� ����
		{
			if( iCount >= EX_OPTION_COUNT )
				iCount = 0;
		}
		else if( iSuccessCount==1 )			// �ɼ� �Ѱ�¥�� ���� �ϼ�
		{
			if( iCount >= EX_OPTION_COUNT )
				break;
		}
		else if( iSuccessCount >= 2 )		// �ɼ� �ΰ� �پ����Ƿ� �ٷ� ���� ����
			break;
	}
		
	return NOption;
}

