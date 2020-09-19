// MonsterItemMng.cpp: implementation of the CMonsterItemMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterItemMng.h"

#include "Common\winutil.h"
#include "Common\zzzItem.h"

#include "MonsterAttr.h"

#include "GameMain.h"

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#include "CashShop.h"
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
#include "CashShop.h"
#endif

extern CMonsterAttr	gMAttr;
extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMonsterItemMng g_MonsterItemMng;

CMonsterItemMng::CMonsterItemMng()
{	
}

CMonsterItemMng::~CMonsterItemMng()
{

	Clear();
}

void CMonsterItemMng::Init()
{
	ZeroMemory(m_iMonsterInvenItemCount, sizeof(m_iMonsterInvenItemCount));

	int i;
	for( i = 0; i < MAX_MONSTERLEVEL; i++ )
		m_MonsterInvenItems[i] = new CItem[MAX_MONSTERGETITEMCOUNT];

	LPMONSTER_ATTRIBUTE		lpm;

	cexDrop.LoadConfig("..\\data\\DropTable.cfg");

#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	for( i = 0; i < MAX_MONSTERATTRIBUTE; i++ )
#else
	for( i = 0; i < 200; i++ )
#endif
	{
		lpm = gMAttr.GetAttr(i);
		if( lpm )
		{
			strcpy(MonsterName, lpm->m_Name);

			if( lpm->m_Level )
				gObjGiveItemSearch(lpm->m_Level, lpm->m_MaxItemLevel);

			/*
			if( lpm->m_Level >= 43 )
						{
							for( int count = 0; count < m_iMonsterInvenItemCount[lpm->m_Level]; count++ )
							{
								if( m_MonsterInvenItems[lpm->m_Level][count].m_Level >= 4 )
									LogAdd("found [%d] %s, %s, %d", 
											count, MonsterName, 
											ItemAttribute[m_MonsterInvenItems[lpm->m_Level][count].m_Type].Name,
											m_MonsterInvenItems[lpm->m_Level][count].m_Level);
							}
							
						}*/
			
			
		}
	}
}

void CMonsterItemMng::Clear()
{
	ZeroMemory(m_iMonsterInvenItemCount, sizeof(m_iMonsterInvenItemCount));

	for( int i = 0; i < MAX_MONSTERLEVEL; i++ )
		delete []m_MonsterInvenItems[i];
}


#ifdef PRINT_MAGICBOOK_ITEMINSERT_LOG

struct MONITEM {
	INT iMagicBook;
	INT iETC;
	INT iHolyGem1;			// 축석 
	INT iHolyGem2;			// 영석 
	INT iHolyGem3;			// 혼석 
	INT iHolyGem4;			// 생석 
	INT iHolyGem5;			// 창석 
	MONITEM() {
		iMagicBook = 0;
		iHolyGem1 = 0;		// 축석 
		iHolyGem2 = 0;		// 영석 
		iHolyGem3 = 0;		// 혼석 
		iHolyGem4 = 0;		// 생석
		iHolyGem5 = 0;		// 창석 
		iETC = 0;
	}
};
MONITEM g_MONITEM[200];

#endif


BYTE CMonsterItemMng::InsertItem(int monsterlevel, int type, int index, int itemlevel, int op1, int op2, int op3)
{

#ifdef PRINT_MAGICBOOK_ITEMINSERT_LOG
	if (type == 14 && index == 13) {		// 축석
		g_MONITEM[monsterlevel].iHolyGem1++;
	}
	else if (type == 14 && index == 14) {	// 축석
		g_MONITEM[monsterlevel].iHolyGem2++;
	}
	else if (type == 12 && index == 15) {	// 영석
		g_MONITEM[monsterlevel].iHolyGem3++;
	}
	else if (type == 14 && index == 16) {	// 생석
		g_MONITEM[monsterlevel].iHolyGem4++;
	}
	else if (type == 14 && index == 22) {	// 창석
		g_MONITEM[monsterlevel].iHolyGem5++;
	}
	else if (type == 15) {
		g_MONITEM[monsterlevel].iMagicBook++;
	}
	else {
		g_MONITEM[monsterlevel].iETC++;
	}
#endif


	if( monsterlevel > MAX_MONSTERLEVEL-1 || monsterlevel < 0 )
		return 0xFF;

	int itemcount = m_iMonsterInvenItemCount[monsterlevel];

	if( itemcount >= MAX_MONSTERGETITEMCOUNT )
		return 0xFF;

	CItem* item = &m_MonsterInvenItems[monsterlevel][itemcount];
	int item_type;	
	
	item_type    = MAKE_ITEMNUM(type, index);

	item->m_Level = itemlevel;
		
	item->Convert( item_type, op1, op2, op3);

	if( type == 13 && index == 10 )
	{
		item->m_Durability = (float)100+(rand()%100);
	}
	else
	{	// 아닐 경우에는 기본 내구도를 넣어 준다
		item->m_Durability = item->m_BaseDurability;
	}

	itemcount++;
	m_iMonsterInvenItemCount[monsterlevel] = itemcount;
	
	//LogAdd("insert [%d] %s\t%d\t%s\t%d", itemcount-1, MonsterName, monsterlevel, ItemAttribute[MAKE_ITEMNUM(type, index)].Name, itemlevel);
	

	//LogAdd("Item List :monsterlevel:%d type:%d index:%d level:%d %s", monsterlevel, type, index, itemlevel, ItemAttribute[MAKE_ITEMNUM(type, index)].Name);
	return 0;
}

CItem* CMonsterItemMng::GetItem(int monsterlevel)
{
	if( monsterlevel > MAX_MONSTERLEVEL-1 || monsterlevel < 0 )
		return NULL;

	int itemcount = m_iMonsterInvenItemCount[monsterlevel];

	if( itemcount <= 0 )
		return NULL;

	int itemindex = rand()%itemcount;

	return &m_MonsterInvenItems[monsterlevel][itemindex];
}

void CMonsterItemMng::gObjGiveItemSearch(int monsterlevel, int maxlevel)
{
	int result;	
	int incount=0;
	int type;
	int index;

	if( monsterlevel > MAX_MONSTERLEVEL-1 ) 
	{
		LogAdd("error-L3 : Monster Level Overflow~ %s %d", __FILE__, __LINE__);
		return;
	}

#ifdef UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828	// 구슬 스크롤 드랍
	int BallTable[MAX_BALLTABLE_SIZE] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 21, 22, 23, 24, 35, 44, 45, 46, 47, 48};
#else	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828		
#ifdef DARKLORD_WORK
	// 파이어스크림 법서 추가, MAX_BALLTABLE_SIZE  17로 변경
	#ifdef MODIFY_ITEM_DROP_FIRESCREAMSCROLL_20060830 
	int BallTable[MAX_BALLTABLE_SIZE] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 21, 22, 23, 24, 35};	
	#else
	int BallTable[16] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 21, 22, 23, 24};	
	#endif	
#else
	int	BallTable[12] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19};	
#endif
#endif	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828
	
	while(1)
	{
#ifdef NEW_FORSKYLAND3
		if( (rand()%20) == 0 )	// 법서 나올 확률 1/100
#else
		if( (rand()%40) == 0 )	// 법서 나올 확률 1/40
#endif
		{
#ifdef NEW_FORSKYLAND3
			if( rand()%2 )
			{	// 법서
				type = 15;
	#ifdef MODIFY_MAGIC_SCROLL_DROPBUG_FIX_20070207
		#ifdef MODIFY_MAGIC_SCROLL_DROPBUG_FIX_02_20070319
				index = rand() % ( g_MaxItemIndexOfEachItemType[type] + 1 );
		#else
				index = rand() % g_MaxItemIndexOfEachItemType[type] + 1;
		#endif // MODIFY_MAGIC_SCROLL_DROPBUG_FIX_02_20070319
	#else
		#if defined(MODIFY_ITEM_DROP_MAXINDEX_20060830) && defined(FOR_JAPAN)
				index = g_MaxItemIndexOfEachItemType[type] + 1;
		#else

			#ifdef MODIFY_SUMMONER_BUGFIX_1_20071221	// 법서 드랍확률 수정(법서 종류(index) 최대 50개)
				index = rand()%50;
			#else	// MODIFY_SUMMONER_BUGFIX_1_20071221
				index = rand()%MAX_ITEM_INDEX;
			#endif	// MODIFY_SUMMONER_BUGFIX_1_20071221

		#endif // MODIFY_ITEM_DROP_MAXINDEX_20060830
	#endif MODIFY_MAGIC_SCROLL_DROPBUG_FIX_20070207
			}
			else
			{	// 구슬류 
				type = 12;
#ifdef DARKLORD_WORK
		#ifdef MODIFY_ITEM_DROP_MAXINDEX_20060830
				index = BallTable[rand()%MAX_BALLTABLE_SIZE]; 
		#else
				index = BallTable[rand()%16];
		#endif // MODIFY_ITEM_DROP_MAXINDEX_20060830
#else
				index = BallTable[rand()%12];
#endif
			}
#else
			type = 15;
			index = rand()%MAX_ITEM_INDEX;
#endif		
		}
		else
		{	
			type = rand()%MAX_ITEM_TYPE;
			
#ifdef ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808

//			int iMaxItemIndex = g_MaxItemIndexOfEachItemType[type] <=0 ? MAX_ITEM_INDEX : g_MaxItemIndexOfEachItemType[type];

			// type의 무결성을 위해 위 예외처리 하지 않음.

		#ifdef MODIFY_ITEM_DROP_MAXINDEX_20060830
			int iMaxItemIndex = g_MaxItemIndexOfEachItemType[type] + 1;
		#else
			int iMaxItemIndex = g_MaxItemIndexOfEachItemType[type];
		#endif
			
			index = rand()%iMaxItemIndex;
#else
			index = rand()%MAX_ITEM_INDEX;
#endif

			if ( type == 15 || ((type == 12) && (index != 15))) {
#ifdef MODIFY_SPEAR_DROP_BUGFIX_20080901
				if(type == 12 && index != 70)
					continue;
#else	// MODIFY_SPEAR_DROP_BUGFIX_20080901
				continue;
#endif	// MODIFY_SPEAR_DROP_BUGFIX_20080901
			}
		}

#ifdef FOR_BLOODCASTLE_TEST
		if( (type == 13 && index == 16) ||
			(type == 13 && index == 17) ||
			(type == 13 && index == 18)
			)
		{	// 대천사의서와 블러드본은 따로 떨어지는 곳이 있다.
			continue;
		}
#endif
		
#ifdef NEW_SKILL_FORSKYLAND
		if( type == 13 && index == 3 )
		{	// 페가시아의 뿔은 몬스터에게서 떨어 질수 없다
			continue;
		}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
		// 펜릴 아이템들은 따로 드랍 할 수 있도록 한다.
		if( (type == 13 && index == 32) 
			|| (type == 13 && index == 33) 
			|| (type == 13 && index == 34) 
			|| (type == 13 && index == 35) 
			|| (type == 13 && index == 36)
			|| (type == 13 && index == 37) 
			)
		{
			continue;
		}

#endif // ADD_ITEM_FENRIR_01_20051110
		
#ifdef ADD_SHIELD_POINT_01_20060403
// CODEREVIEW_SHIELD3 : 복합 물약 & SD 회복 물약은 따로 드랍 한다.
		if( (type == 14 && index == 35 )
			|| (type == 14 && index == 36 )
			|| (type == 14 && index == 37 )
			|| (type == 14 && index == 38 )
			|| (type == 14 && index == 39 )
			|| (type == 14 && index == 40 )
			)
		{
			continue;
		}
#endif // ADD_SHIELD_POINT_01_20060403

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
		if( IsCashItem( MAKE_ITEMNUM( type, index ) ) == TRUE )
		{
			continue;
		}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
		// 프리미엄 아이템은 드랍 불가
		if(	IsPremiumItem( MAKE_ITEMNUM( type, index ) ) == TRUE )
		{
			continue;
		}
#endif // PREMIUMITEM_20090106
		
#ifndef FOR_KOREA
	#ifdef ADD_PREMIUMITEM_PANDA
		if( (type == 13 && index == 76 )		// 팬더변신반지
			|| (type == 13 && index == 80 )		// 팬더펫
		)
		{
			continue;
		}
	#endif // ADD_PREMIUMITEM_PANDA
#endif // NOT_FOR_KOREA

#ifdef MODIFY_BLOCK_PUMPKIN_OF_LUCK_DROP_20061120
		if( ( type == 14 && index == 45 )
			|| ( type == 14 && index == 46 )
			|| ( type == 14 && index == 47 )
			|| ( type == 14 && index == 48 )
			|| ( type == 14 && index == 49 )
			|| ( type == 14 && index == 50 )
		  )
		{
			continue;
		}
#endif // MODIFY_BLOCK_PUMPKIN_OF_LUCK_DROP_20061120
		
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		// 산타크로스 변신 반지는 그냥 드롭되지 않는다.
		if( type == 13 && index == 41 )
		{
			continue;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117		
		
		// type : 13, 14, 12일때 레벨에 상관없이..
		if( ((type==13) && (index<8)) ||			// 얼음반지
			((type == 14) && ((index == 9)	||		// 술
			 (index == 10) || (index == 13)	||		// 마을귀환문서, 축석
			 (index == 14) || (index == 16)	||		// 영석, 생석
			 (index == 17) || (index == 18) ||		// 악마의눈, 악마의열쇠
			 (index == 22)) )						// 창석
			|| ((type==12) && (index==15) ||		// 혼석
			((type==13) && (index==14))				// 로크의 깃털
			
#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
			|| ( (type==14) && (index==31) )		// 수호의 보석
#endif
			) 
		  )
		{
#ifdef FOR_NEW_TESTSERVER			
//			int perc;
//			if( gTestServer )				
			int perc = rand()%8;		// 기본확률 1/9
//			else
//				perc = rand()%11;		// 기본확률 1/9
#else
			int perc = rand()%11;		// 기본확률 1/9
#endif
			BOOL bCheckDevil = FALSE;

			//-------------------------------------
			// 아래 아이템들은 별도로 확률이 조정됨
			if( type==12 && index==15 )
			{
				if( monsterlevel >= 13 && monsterlevel <= 66 )
				{
					perc = rand()%7;	// 4/7
					if( perc < 3 )					
						perc = 0;
				}	
				else
					perc = 1;
			}

#if defined ( UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828 ) && !defined( DEVILSQARE_COMBINE_ITEM_DROP_20090114 )
			if( (type==14 && index==17 )
				|| (type==14 && index==18 ))
			{
				perc = 1;
			}
#endif // UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828

// 해외는 악마의 눈, 열쇠 드랍 되게 수정
#ifdef	DEVILSQARE_COMBINE_ITEM_DROP_20090114 
			if( type==14 && index==17 )
			{	// 악마의 눈
				if (gEyesOfDevilSquareDropRate <= 0)
					gEyesOfDevilSquareDropRate = 1;
				perc = rand()%gEyesOfDevilSquareDropRate;
				bCheckDevil = TRUE;
				if( !gDevilSquareEvent )
					perc = 1;
			}

			if( type==14 && index==18 )
			{	// 악마의 열쇠
				perc = rand()%gKeyOfDevilSquareDropRate;
				bCheckDevil = TRUE;
				if( !gDevilSquareEvent )
					perc = 1;
			}	
#endif	// DEVILSQARE_COMBINE_ITEM_DROP20090114
			//-------------------------------------

			if( perc ==0 )
			{
				if( bCheckDevil == TRUE )
				{	// 악마의 광장 아이템의 드랍은 다르게 설정한다
					int devilitemlevel = 0;

#ifndef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7광장 재료는 gEventMonsterItemDrop() 에서 드랍					
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 광장7 입장권 재료 아이템 드롭
					if( monsterlevel >= 90 )		// 자이칸 이상
					{
						if( rand() % 1000 < 10 )	// 10/1000확률
						{
							devilitemlevel = 7;
						}
					}
					else
#endif
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
					if( rand()%5 != 0 )
					{	// 0짜리 아이템
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . 임시 -> 광장 조합 아이템 드롭 추가
						if( monsterlevel < 3)
						{
							devilitemlevel = 1; //DEVILSQUARE FIX (Normal es +0)
						}
						else if( monsterlevel < 36)
						{
							devilitemlevel = 1;
						}
						else if( monsterlevel < 47)
						{
							devilitemlevel = 2;
						}
						else if( monsterlevel < 60)
						{
							devilitemlevel = 3;
						}
						else if( monsterlevel < 70)
						{
							devilitemlevel = 4;
						}
						else if( monsterlevel < 80)
						{
							devilitemlevel = 5;
						}
						else
						{
							devilitemlevel = 6;
						}
#else						
						if( monsterlevel <= 15)
						{
							devilitemlevel = 1; //DEVILSQUARE FIX (Normal es +0)
						}
						else if( monsterlevel <= 40)
						{
							devilitemlevel = 1;
						}
						else if( monsterlevel <= 54)
						{
							devilitemlevel = 2;
						}
						else if( monsterlevel <= 66)
						{
							devilitemlevel = 3;
						}
						else
						{
							devilitemlevel = 4;
						}
#endif
#ifndef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7광장 재료는 gEventMonsterItemDrop() 에서 드랍
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
					}
#endif	
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123					
						
						if( devilitemlevel != 0 )
						{	// 앞으로는 0짜리 악마의 광장 아이템은 떨어 지지 않는다
							if( InsertItem(monsterlevel, type, index, devilitemlevel) != 0xFF )
							{
								///*if( monsterlevel >= 50 )
								//{
								//LogAdd("(%d)Devil Square Item 추가(level:%d) : type:%d index:%d level:%d %s", lpObj->Class, monsterlevel, type, index, devilitemlevel, lpObj->Name);
								//}
								//*/
								incount++;								

								if( incount > MAX_MONSTERGETITEMCOUNT-1 ) break;
							}
						}
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7광장 재료는 gEventMonsterItemDrop() 에서 드랍
					}
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
				//	}
#else
				}
#endif
				}	
				else
				{
					// 악마의 광장 외 일반 아이템들
					if( zzzItemLevel(type, index, monsterlevel) == TRUE )		// 아이템의 레벨제한이 해당몬스터의 레벨보다 높으면 추가되지 않는다.
					{
						if( InsertItem(monsterlevel, type, index, 0) != 0xFF )
						{
							//*if( monsterlevel >= 50 )
							//{
							/*
							LogAdd("Item 추가(level:%d) : %s type:%d index:%d level:%d", 
																monsterlevel, 
																ItemAttribute[MAKE_ITEMNUM(type,index)].Name,
																type, 
																index, 
																result);*/
							
							//}
							//*/
							incount++;							
							if( incount > MAX_MONSTERGETITEMCOUNT-1 ) break;
						}
					}
				}
			}
		}
		else
		{
			// type : 13, 14, 12 을 제외한 나머지 아이템들은 +수치를 랜덤하게 받는다 (몬스터 레벨에 영향받음)
			result = GetLevelItem(type, index, monsterlevel);
			if( result >= 0  )
			{
			
				if( (type == 13 && index == 10) ||		// 변신반지
					(type == 12 && index == 11) )		// 소환구슬
				{
					if( InsertItem(monsterlevel, type, index, result) != 0xFF )
					{
						//if( monsterlevel >= 50 )
						{
							/*
							LogAdd("Item 추가(level:%d) : %s type:%d index:%d level:%d", 
																monsterlevel, 
																ItemAttribute[MAKE_ITEMNUM(type,index)].Name
																type, 
																index, 
																result);*/
							
						}
						//*/
						incount++;						
						if( incount > MAX_MONSTERGETITEMCOUNT-1 ) break;
					}
				}
				else if( result <= maxlevel )		// 랜덤하게 +된 아이템 레벨이 해당 몬스터의 최고 아이템 레벨을 넘지 못하도록
				{
					if( type == 12)					// ITEM_WING의 경우 소환구슬을 제외한 경우는 +수치 0
					{
						if( index != 11 ) result = 0;
					}
					if( type == 12 && index == 11 )
					{

					}
					else if( result > maxlevel ) result = maxlevel;		// 혹시라도 +레벨이 최고 아이템 레벨을 넘을 경우 고정

					if( (type == 4 && index == 7) || (type == 4 && index == 15) )	// 석궁,화살 스킬 없이
					{
						result = 0;
					}
					
					if( InsertItem(monsterlevel, type, index, result) != 0xFF )
					{
						if( monsterlevel >= 43 && result >= 4 )
						{
						/*
						LogAdd("Item 추가(%s,level:%d) : %s type:%d index:%d level:%d", 
															MonsterName,
															monsterlevel, 
															ItemAttribute[MAKE_ITEMNUM(type,index)].Name,
															type, 
															index, 
															result);*/
						
						}
						//*/
						incount++;						
						if( incount > MAX_MONSTERGETITEMCOUNT-1 ) break;
					}
				}
			}

		}
		if( m_iMonsterInvenItemCount[monsterlevel] >= MAX_MONSTERGETITEMCOUNT )
			break;
	}

}