// ProbabilityItemBag.cpp: implementation of the CProbabilityItemBag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProbabilityItemBag.h"

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

#ifdef SCRIPT_DECODE_WORK	
#include "..\\common\\WZScriptEncode.h"
#endif

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
#include "..\\Common\\SetItemOption.h"
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];
extern CDirPath		gDirPath;

#ifdef ADD_NEWPVP_PKFIELD
#include "LargeRand.h"
const static int s_nMaxDropRate = 10000 * 100;
#endif // ADD_NEWPVP_PKFIELD

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProbabilityItemBag::CProbabilityItemBag()
{
	DropMapInfo = NULL;
}

CProbabilityItemBag::~CProbabilityItemBag()
{
	delete[] DropMapInfo;
}

void CProbabilityItemBag::ProbabilityItemBagInit(char * name)
{	
	m_bLoad = FALSE;
	m_sEventName[0]		= NULL;
	m_iEventItemType	= -1;
	m_iEventItemLevel	= 0;
	m_iDropZen			= 0;
	m_iEventItemDropRate = 0;
	m_iItemDropRate		= 0;
	m_iExItemDropRate	= 0;
	
	m_iBagObjectCount	= 0;	

	m_iRateKindCount	= 0;

	if(g_TerrainManager.Size() < 1)
		g_TerrainManager.Load();

	if(DropMapInfo == NULL)
		DropMapInfo = new CItemBagDropMapInfo[g_TerrainManager.Size()];
	
//#ifdef SCRIPT_DECODE_WORK
//	LoadItemDecode(gDirPath.GetNewPath(name));	
//#else
	LoadItem(gDirPath.GetNewPath(name));	
//#endif
}

void CProbabilityItemBag::LoadItem(char *script_file)
{
	m_bLoad = FALSE;
	if((SMDFile=fopen(script_file,"r")) == NULL)	
	{
		LogAdd(lMsg.Get(453), script_file);
		return ;
	}	

	SMDToken Token;
	int nDropRatePerItemCnt = 0;
	int nRateCnt			= 0;

	m_iBagObjectCount = 0;
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			if( Type == 0 )
			{	// ��ӵ� ������ ���� ����(��, ���� ����)
				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					int	MapIndex = (int)TokenNumber;
					if( !CHECK_LIMIT(MapIndex, g_TerrainManager.Size()) )
					{
						MsgBox("ExEvent ItemBag LoadFail [%s]", script_file);
						return;
					}

					Token = (*GetToken)(); DropMapInfo[MapIndex].m_bIsDrop = (int)TokenNumber;
					Token = (*GetToken)(); DropMapInfo[MapIndex].m_MinMonsterLevel = (int)TokenNumber;
					Token = (*GetToken)(); DropMapInfo[MapIndex].m_MaxMonsterLevel = (int)TokenNumber;
				}
				
			}
			else if( Type == 1 )
			{	// �̺�Ʈ ������ ����
				while(true)
				{
					int type, index;
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					strcpy(m_sEventName, TokenString);
					Token = (*GetToken)(); m_iDropZen = (int)TokenNumber;

					Token = (*GetToken)(); type = (int)TokenNumber;
					Token = (*GetToken)(); index = (int)TokenNumber;
					m_iEventItemType = MAKE_ITEMNUM(type, index);

					Token = (*GetToken)(); m_iEventItemLevel = (int)TokenNumber;

					Token = (*GetToken)(); m_iEventItemDropRate = (int)TokenNumber;
					Token = (*GetToken)(); m_iItemDropRate = (int)TokenNumber;
					Token = (*GetToken)(); m_iExItemDropRate = (int)TokenNumber;

					char msg[255];
					wsprintf(msg, "[%s] Eventitemnum = %d,EventItemLevel = %d, EventItemDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d",
						     m_sEventName, m_iEventItemType, m_iEventItemLevel, m_iEventItemDropRate, 
							 m_iItemDropRate, m_iExItemDropRate);
					LogAddTD(msg);
				}
			}
			else if( Type == 2 )
			{
				nRateCnt = 0;

				while(true)
				{					
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;
					if( nRateCnt == 0 )
					{
						m_iEventItemDropRateEx[nRateCnt] = (int)TokenNumber;
					}
					else
					{
						m_iEventItemDropRateEx[nRateCnt] = m_iEventItemDropRateEx[nRateCnt-1] + (int)TokenNumber;
					}
					
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
					if( nRateCnt > MAX_ITEMRATE_COUNT-1 ) break;
				}

				m_iRateKindCount = nRateCnt;
			}
#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
			else if ( Type == 3 )
			{
				nRateCnt = m_iRateKindCount;

				while(true)
				{					
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;
					if( nRateCnt == 0 )
					{
						m_iEventItemDropRateEx[nRateCnt] = (int)TokenNumber;
					}
					else
					{
						m_iEventItemDropRateEx[nRateCnt] = m_iEventItemDropRateEx[nRateCnt-1] + (int)TokenNumber;
					}
					
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

						BagObject[m_iBagObjectCount].m_isSetItem = (int)1;	// ��Ʈ������

						BagObject[m_iBagObjectCount].m_minLevel = (int)0;
						BagObject[m_iBagObjectCount].m_maxLevel = (int)0;
						BagObject[m_iBagObjectCount].m_isskill	= (int)0;
						BagObject[m_iBagObjectCount].m_isluck	= (int)0;
						BagObject[m_iBagObjectCount].m_isoption = (int)0;
						BagObject[m_iBagObjectCount].m_isexitem = (int)0;

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
					if( nRateCnt > MAX_ITEMRATE_COUNT-1 ) break;
				}

				m_iRateKindCount = nRateCnt;
			}
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329			
		}
	}

	fclose(SMDFile);	
	LogAdd(lMsg.Get(454), script_file);

	m_bLoad = TRUE;
}

BYTE CProbabilityItemBag::GetLevel(int n)
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

BOOL CProbabilityItemBag::IsEnableEventItemDrop(int aIndex)
{
	int iMapNumber = gObj[aIndex].MapNumber;
	
	if( !DropMapInfo[iMapNumber].m_bIsDrop )
	{	// ��Ӽ� ���� ���̴�
		return FALSE;
	}

	int iLevel = gObj[aIndex].Level;

	if( iLevel < DropMapInfo[iMapNumber].m_MinMonsterLevel || 
		iLevel > DropMapInfo[iMapNumber].m_MaxMonsterLevel )
	{	// ��� ���� ������ ���� �ʴ�
		return FALSE;
	}
	return TRUE;	
}

int CProbabilityItemBag::GetDropItemNumber(int nDropItemRate)
{
	int nRateCnt = m_iRateKindCount;
	int iDropItemNum = 0;
	int ItemRateIndexPos = GetBagCount() -1;
	while(nRateCnt--)
	{
		// ������ ��ȣ �˻�
		if(nRateCnt == 0)
		{
			iDropItemNum = ( rand() % m_iDropRatePerItemCount[0] );
			break;
		}
		else if(nDropItemRate >= 10000 - m_iEventItemDropRateEx[nRateCnt] && nDropItemRate < 10000 - m_iEventItemDropRateEx[nRateCnt-1])
		{
			// ������ ���
			iDropItemNum = ItemRateIndexPos - ( rand() % m_iDropRatePerItemCount[nRateCnt] );
			break;
		}	
		
		
		ItemRateIndexPos -= m_iDropRatePerItemCount[nRateCnt];
	}

	return iDropItemNum;
}

#ifdef MODIFY_EVENTITEMBAG_BASIC_SET_20070823
char* CProbabilityItemBag::GetEventName()
{
	return m_sEventName;
}
#endif

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	//�������ָӴ� ���Ϳ��� �������� �巴
BOOL CProbabilityItemBag::DropNewYearLuckyBagEventItem(int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY)
{
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
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	
			
			LogAddTD("[ NewYearLuckyBagMonsterEvent ] NewYearLuckyBagDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, m_iDropZen );		
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand(0);
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[ NewYearLuckyBagMonsterEvent ] NewYearLuckyBagDrop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );

	}
	return TRUE;
}
#endif	//UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131

#ifdef UPDATE_GM_FUNCTION_20070228		// GM ���� ���ڿ��� ������ ���
BOOL CProbabilityItemBag::DropGMPresentBoxEventItem(int aIndex, BYTE btMapNumber, UCHAR cX /* = 0 */, UCHAR cY /* = 0 */)
{
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
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, lpObj->X, lpObj->Y );	
			
			LogAddTD("[ GMPresentBox ] GMPresentBoxItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, m_iDropZen );		
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand(0);
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		if( iType == MAKE_ITEMNUM( 13, 20 ) )
		{	// �������� ���� 0
			iLevel   = 0;

			// �������ǹ��� �������� 255
			fDur	= 255;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[ GMPresentBox ] GMPresentBoxItemDrop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );

	}
	return TRUE;
}
#endif	// UPDATE_GM_FUNCTION_20070228

#ifdef PCBANG_POINT_SYSTEM_20070206						// ȥ�� ���ڿ��� �������� ��� ó��
// �ʷ�ȥ�� ����
BOOL CProbabilityItemBag::DropBoxOfGreenChaosItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )		
{
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
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	
			
			LogAddTD("[PCBangPointSystem] DropBoxOfGreenChaosItem [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, m_iDropZen );		
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand( 0 );
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[PCBangPointSystem] DropBoxOfGreenChaosItem [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );

	}
	return TRUE;
}

// ����ȥ�� ����
BOOL CProbabilityItemBag::DropBoxOfRedChaosItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )		
{
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
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	
			
			LogAddTD("[PCBangPointSystem] DropBoxOfRedChaosItem [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, m_iDropZen );		
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand( 0 );
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[PCBangPointSystem] DropBoxOfRedChaosItem [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );

	}	
	return TRUE;
}

// ����ȥ�� ����
BOOL CProbabilityItemBag::DropBoxOfPurpleChaosItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )		
{
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
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	
			
			LogAddTD("[PCBangPointSystem] DropBoxOfPurpleChaosItem [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, m_iDropZen );		
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand( 0 );
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[PCBangPointSystem] DropBoxOfPurpleChaosItem [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );

	}	
	return TRUE;
}	
#endif	// PCBANG_POINT_SYSTEM_20070206

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	//ȯ�� ��� ���� ������ ����
BOOL CProbabilityItemBag::DropIllusionTempleGhostItem(int aIndex, BYTE btMapNumber, UCHAR cX/* = 0 */, UCHAR cY/* = 0 */)
{
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
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	
			
			LogAddTD("[ ILLUSIONTempleEvent ] ILLUSIONTempleEventDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, m_iDropZen );		
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand(0);
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) ||
			(iType == MAKE_ITEMNUM(14, 22)))	// â��
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[ ILLUSIONTempleEvent ] ILLUSIONTempleEventDrop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );

	}
	return TRUE;
}
#endif	//ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef MODIFY_EVENTITEMBAG_BASIC_SET_20070823
BOOL CProbabilityItemBag::DropEventItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if( !m_bLoad )					// ������ ��ũ��Ʈ�� �ε���� �ʾҴ�
		return FALSE;
	
	float fDur			= 0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption		= 0;
	

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	
			
			LogAddTD("[%s] Drop Zen [%s][%s] [%d Zen]", GetEventName(), lpObj->AccountID, lpObj->Name, m_iDropZen );		
			return FALSE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329	// ��Ʈ�������� ���
		if(BagObject[iDropItemNum].m_isSetItem == 1)
		{
			DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
			return TRUE;
		}
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}
		
#ifdef UPDATE_PROBABILITYITEMBAG_ADDOPTION_VALUE_20090526
		if(BagObject[iDropItemNum].m_isoption == 1)
		{
			if( BagObject[iDropItemNum].m_isoption )
			{
				if( ( rand() % 5 ) < 1 )
				{
					iOption3 = 3;
				}
				else 
				{
					iOption3 = ( rand() % 3 );
				}
			}
		}
		else if( BagObject[iDropItemNum].m_isoption > 1 )
		{
			// addoption ���� ���� option���� �־��ش�.
			iOption3 = BagObject[iDropItemNum].m_isoption / 4;
		}
#else	// UPDATE_PROBABILITYITEMBAG_ADDOPTION_VALUE_20090526
		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}
#endif	// UPDATE_PROBABILITYITEMBAG_ADDOPTION_VALUE_20090526

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand( 0 );
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ����, â���� ������ ����
		if( ( iType == MAKE_ITEMNUM( 12, 15 ) ) ||
			( iType == MAKE_ITEMNUM( 14, 13 ) ) ||
			( iType == MAKE_ITEMNUM( 14, 14 ) ) ||
			( iType == MAKE_ITEMNUM( 14, 22 ) ))
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		if( ( iType == MAKE_ITEMNUM( 13, 0 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 1 ) ) ||				
			( iType == MAKE_ITEMNUM( 13, 2 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 8 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 9 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 12) ) ||
			( iType == MAKE_ITEMNUM( 13, 13 ) )
#if GAME_VERSION >= G_V_S2_2
			|| ( iType == MAKE_ITEMNUM( 13, 38 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 39 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 40 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 41 ) )
#endif
			)
		{	// �������� ���� 0
			iLevel   = 0;

			// �������� 255
			fDur	= 255;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				GetEventName(),
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );

	}	
	return TRUE;
}
#endif // MODIFY_EVENTITEMBAG_BASIC_SET_20070823

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
void CProbabilityItemBag::DropEventSetItem(int aIndex, int itemnum, BYTE btMapNumber, int X, int Y)
{
	int	SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand, option2rand, option3rand, optionc;	
	int	Option1 = 0, Option2 = 0, Option3 = 0;

	
	option1rand = 6;				// �Ϲ����� �ɼ�1 (��ų) ���� Ȯ�� 6/100
	option2rand = 4;				// �Ϲ����� �ɼ�2 (���) ���� Ȯ�� 4/100
	option3rand = (rand()%100);		// �Ϲ����� �ɼ�3 (�߰�����Ʈ) ���� Ȯ�� ����/100
	optionc		= (rand()%3);

	// ��ų������ Ȯ���� ���� �����.	
	if((rand()%100) < option2rand ) Option2 = 1;

	switch( optionc )
	{
	case 0 :
		if( option3rand < 4 ) Option3 = 3;	// �ɼ�
		break;
	case 1 :
		if( option3rand < 8 ) Option3 = 2;	// �ɼ�
		break;
	case 2 :
		if( option3rand < 12 ) Option3 = 1;	// �ɼ�
		break;
	}

	Option1 = 1;
	ItemSerialCreateSend(aIndex, btMapNumber, X, Y, itemnum, 
						 0, (BYTE)0, Option1, Option2, Option3, aIndex, 0, SetOption);

#ifdef UPDATE_SET_ITEM_DROP_LOG_20080420	// �̺�Ʈ �� ��Ʈ ������ �α�
	int tmpSetOption = 0;

	if( SetOption & 0x01)
	{
		tmpSetOption = 1;
	}
	else if( SetOption & 0x02 )
	{
		tmpSetOption = 2;
	}

	LogAddTD( "[%s] Drop Set Item [%s][%s] : (%d)(%d/%d) Itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",
				GetEventName(),
				gObj[aIndex].AccountID, gObj[aIndex].Name,  
				btMapNumber, X, Y,
				itemnum, Option1, Option2, Option3, SetOption, 
				gSetItemOption.GetSetOptionName(itemnum, tmpSetOption));
#else	// UPDATE_SET_ITEM_DROP_LOG_20080420
	LogAddTD( "[%s] Drop Set Item [%s][%s] : (%d)(%d/%d) Itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
				GetEventName(),
				gObj[aIndex].AccountID, gObj[aIndex].Name,  
				btMapNumber, X, Y,
				itemnum, Option1, Option2, Option3, SetOption );
#endif	// UPDATE_SET_ITEM_DROP_LOG_20080420
}
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
BOOL CProbabilityItemBag::DropCherryBlossomEventItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney)
{
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
		int nDropItemRate = rand() % 10000;
		if( m_iRateKindCount == 0 || nDropItemRate < 10000 - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			// �� ���
			if (btMapNumber == 255) {
				*pMoney = m_iDropZen;

				//���ɰ��� ���տ��� zen����� ��� ���з� �����Ѵ�.
				//208/03/31 riverstyx
				return FALSE;
			}
			else
				MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	

			LogAddTD("[%s] Drop Zen [%s][%s] [%d Zen]", GetEventName(), lpObj->AccountID, lpObj->Name, m_iDropZen );
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumber(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}

		if (btMapNumber == 255) {
			X	= 0;
			Y	= 0;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329	// ��Ʈ�������� ���
		if(BagObject[iDropItemNum].m_isSetItem == 1)
		{
			DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
			return TRUE;
		}
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand( 0 );
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}		

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			GetEventName(),
			lpObj->AccountID, lpObj->Name,  
			btMapNumber, X, Y,
			ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );
	}
	return TRUE;
}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_RAKLION_20080408
BOOL CProbabilityItemBag::DropRaklionSelupanItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
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
		iDropItemNum = ( rand() % GetBagCount() );
		fDur  = 0;
		
		if( cX == 0 && cY == 0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}			
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );
		
		if( iType == -1 )
		{
			return FALSE;
		}
		
		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}
		
		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}
		
		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}
		
		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand(0);
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;
			
			// ������ 0�̴�.
			iLevel	= 0;
		}
		
		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[ RAKLION ][ Reward ] Selupan Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name,  
			btMapNumber, X, Y,
			ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );
	}
	
	return TRUE;
}
#endif // ADD_RAKLION_20080408

#ifdef ADD_NEWPVP_PKFIELD
int CProbabilityItemBag::GetDropItemNumberEx(int nDropItemRate)
{
	int nRateCnt = m_iRateKindCount;
	int iDropItemNum = 0;
	int ItemRateIndexPos = GetBagCount() -1;
	while(nRateCnt--)
	{
		// ������ ��ȣ �˻�
		if(nRateCnt == 0)
		{
			iDropItemNum = ( GetLargeRand() % m_iDropRatePerItemCount[0] );
			break;
		}
		else if(nDropItemRate >= s_nMaxDropRate - m_iEventItemDropRateEx[nRateCnt] && nDropItemRate < s_nMaxDropRate - m_iEventItemDropRateEx[nRateCnt-1])
		{
			// ������ ���
			iDropItemNum = ItemRateIndexPos - ( GetLargeRand() % m_iDropRatePerItemCount[nRateCnt] );
			break;
		}

		ItemRateIndexPos -= m_iDropRatePerItemCount[nRateCnt];
	}
	
	return iDropItemNum;
}

BOOL CProbabilityItemBag::DropEventItemEx( int aIndex, BYTE btMapNumber, UCHAR cX /*= 0*/, UCHAR cY /*= 0*/)
{
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
		int nDropItemRate = GetLargeRand() % s_nMaxDropRate;
		if( m_iRateKindCount == 0 || nDropItemRate < s_nMaxDropRate - m_iEventItemDropRateEx[m_iRateKindCount-1] )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop( m_iDropZen, cX, cY );	

			LogAddTD("[NewPVP] [%s] Drop Zen [%s][%s] [%d Zen]", GetEventName(), lpObj->AccountID, lpObj->Name, m_iDropZen );
			return TRUE;
		}

		// ����� ������ �˻�
		int nRateCnt = m_iRateKindCount;
		int iDropItemNum = GetDropItemNumberEx(nDropItemRate);		

		if( cX==0 && cY==0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}

		if (btMapNumber == 255) {
			X	= 0;
			Y	= 0;
		}
		
		// ����
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329	// ��Ʈ�������� ���
		if(BagObject[iDropItemNum].m_isSetItem == 1)
		{
			DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
			return TRUE;
		}
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

		if( BagObject[iDropItemNum].m_isskill )
		{	// ��ų�� ������ ������
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// ����� ������ ������
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand( 0 );
			// �׼���Ʈ �������� ����� ���� �ʴ´�.
			iOption2 = 0;
			iOption1 = 1;

			// ������ 0�̴�.
			iLevel	= 0;
		}

		// ȥ��, �༮, ������ ������ ����
		if( (iType == MAKE_ITEMNUM(12, 15)) ||
			(iType == MAKE_ITEMNUM(14, 13)) ||
			(iType == MAKE_ITEMNUM(14, 14)) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}		

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[NewPVP] [%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			GetEventName(),
			lpObj->AccountID, lpObj->Name,  
			btMapNumber, X, Y,
			ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );
	}
	return TRUE;
}
#endif // ADD_NEWPVP_PKFIELD