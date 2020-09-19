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
	INT iHolyGem1;			// �༮ 
	INT iHolyGem2;			// ���� 
	INT iHolyGem3;			// ȥ�� 
	INT iHolyGem4;			// ���� 
	INT iHolyGem5;			// â�� 
	MONITEM() {
		iMagicBook = 0;
		iHolyGem1 = 0;		// �༮ 
		iHolyGem2 = 0;		// ���� 
		iHolyGem3 = 0;		// ȥ�� 
		iHolyGem4 = 0;		// ����
		iHolyGem5 = 0;		// â�� 
		iETC = 0;
	}
};
MONITEM g_MONITEM[200];

#endif


BYTE CMonsterItemMng::InsertItem(int monsterlevel, int type, int index, int itemlevel, int op1, int op2, int op3)
{

#ifdef PRINT_MAGICBOOK_ITEMINSERT_LOG
	if (type == 14 && index == 13) {		// �༮
		g_MONITEM[monsterlevel].iHolyGem1++;
	}
	else if (type == 14 && index == 14) {	// �༮
		g_MONITEM[monsterlevel].iHolyGem2++;
	}
	else if (type == 12 && index == 15) {	// ����
		g_MONITEM[monsterlevel].iHolyGem3++;
	}
	else if (type == 14 && index == 16) {	// ����
		g_MONITEM[monsterlevel].iHolyGem4++;
	}
	else if (type == 14 && index == 22) {	// â��
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
	{	// �ƴ� ��쿡�� �⺻ �������� �־� �ش�
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

#ifdef UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828	// ���� ��ũ�� ���
	int BallTable[MAX_BALLTABLE_SIZE] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 21, 22, 23, 24, 35, 44, 45, 46, 47, 48};
#else	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828		
#ifdef DARKLORD_WORK
	// ���̾ũ�� ���� �߰�, MAX_BALLTABLE_SIZE  17�� ����
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
		if( (rand()%20) == 0 )	// ���� ���� Ȯ�� 1/100
#else
		if( (rand()%40) == 0 )	// ���� ���� Ȯ�� 1/40
#endif
		{
#ifdef NEW_FORSKYLAND3
			if( rand()%2 )
			{	// ����
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

			#ifdef MODIFY_SUMMONER_BUGFIX_1_20071221	// ���� ���Ȯ�� ����(���� ����(index) �ִ� 50��)
				index = rand()%50;
			#else	// MODIFY_SUMMONER_BUGFIX_1_20071221
				index = rand()%MAX_ITEM_INDEX;
			#endif	// MODIFY_SUMMONER_BUGFIX_1_20071221

		#endif // MODIFY_ITEM_DROP_MAXINDEX_20060830
	#endif MODIFY_MAGIC_SCROLL_DROPBUG_FIX_20070207
			}
			else
			{	// ������ 
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

			// type�� ���Ἲ�� ���� �� ����ó�� ���� ����.

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
		{	// ��õ���Ǽ��� ���庻�� ���� �������� ���� �ִ�.
			continue;
		}
#endif
		
#ifdef NEW_SKILL_FORSKYLAND
		if( type == 13 && index == 3 )
		{	// �䰡�þ��� ���� ���Ϳ��Լ� ���� ���� ����
			continue;
		}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
		// �渱 �����۵��� ���� ��� �� �� �ֵ��� �Ѵ�.
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
// CODEREVIEW_SHIELD3 : ���� ���� & SD ȸ�� ������ ���� ��� �Ѵ�.
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
		// �����̾� �������� ��� �Ұ�
		if(	IsPremiumItem( MAKE_ITEMNUM( type, index ) ) == TRUE )
		{
			continue;
		}
#endif // PREMIUMITEM_20090106
		
#ifndef FOR_KOREA
	#ifdef ADD_PREMIUMITEM_PANDA
		if( (type == 13 && index == 76 )		// �Ҵ����Ź���
			|| (type == 13 && index == 80 )		// �Ҵ���
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
		// ��Ÿũ�ν� ���� ������ �׳� ��ӵ��� �ʴ´�.
		if( type == 13 && index == 41 )
		{
			continue;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117		
		
		// type : 13, 14, 12�϶� ������ �������..
		if( ((type==13) && (index<8)) ||			// ��������
			((type == 14) && ((index == 9)	||		// ��
			 (index == 10) || (index == 13)	||		// ������ȯ����, �༮
			 (index == 14) || (index == 16)	||		// ����, ����
			 (index == 17) || (index == 18) ||		// �Ǹ��Ǵ�, �Ǹ��ǿ���
			 (index == 22)) )						// â��
			|| ((type==12) && (index==15) ||		// ȥ��
			((type==13) && (index==14))				// ��ũ�� ����
			
#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
			|| ( (type==14) && (index==31) )		// ��ȣ�� ����
#endif
			) 
		  )
		{
#ifdef FOR_NEW_TESTSERVER			
//			int perc;
//			if( gTestServer )				
			int perc = rand()%8;		// �⺻Ȯ�� 1/9
//			else
//				perc = rand()%11;		// �⺻Ȯ�� 1/9
#else
			int perc = rand()%11;		// �⺻Ȯ�� 1/9
#endif
			BOOL bCheckDevil = FALSE;

			//-------------------------------------
			// �Ʒ� �����۵��� ������ Ȯ���� ������
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

// �ؿܴ� �Ǹ��� ��, ���� ��� �ǰ� ����
#ifdef	DEVILSQARE_COMBINE_ITEM_DROP_20090114 
			if( type==14 && index==17 )
			{	// �Ǹ��� ��
				if (gEyesOfDevilSquareDropRate <= 0)
					gEyesOfDevilSquareDropRate = 1;
				perc = rand()%gEyesOfDevilSquareDropRate;
				bCheckDevil = TRUE;
				if( !gDevilSquareEvent )
					perc = 1;
			}

			if( type==14 && index==18 )
			{	// �Ǹ��� ����
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
				{	// �Ǹ��� ���� �������� ����� �ٸ��� �����Ѵ�
					int devilitemlevel = 0;

#ifndef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7���� ���� gEventMonsterItemDrop() ���� ���					
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// ����7 ����� ��� ������ ���
					if( monsterlevel >= 90 )		// ����ĭ �̻�
					{
						if( rand() % 1000 < 10 )	// 10/1000Ȯ��
						{
							devilitemlevel = 7;
						}
					}
					else
#endif
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
					if( rand()%5 != 0 )
					{	// 0¥�� ������
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . �ӽ� -> ���� ���� ������ ��� �߰�
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
#ifndef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7���� ���� gEventMonsterItemDrop() ���� ���
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
					}
#endif	
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123					
						
						if( devilitemlevel != 0 )
						{	// �����δ� 0¥�� �Ǹ��� ���� �������� ���� ���� �ʴ´�
							if( InsertItem(monsterlevel, type, index, devilitemlevel) != 0xFF )
							{
								///*if( monsterlevel >= 50 )
								//{
								//LogAdd("(%d)Devil Square Item �߰�(level:%d) : type:%d index:%d level:%d %s", lpObj->Class, monsterlevel, type, index, devilitemlevel, lpObj->Name);
								//}
								//*/
								incount++;								

								if( incount > MAX_MONSTERGETITEMCOUNT-1 ) break;
							}
						}
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7���� ���� gEventMonsterItemDrop() ���� ���
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
					// �Ǹ��� ���� �� �Ϲ� �����۵�
					if( zzzItemLevel(type, index, monsterlevel) == TRUE )		// �������� ���������� �ش������ �������� ������ �߰����� �ʴ´�.
					{
						if( InsertItem(monsterlevel, type, index, 0) != 0xFF )
						{
							//*if( monsterlevel >= 50 )
							//{
							/*
							LogAdd("Item �߰�(level:%d) : %s type:%d index:%d level:%d", 
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
			// type : 13, 14, 12 �� ������ ������ �����۵��� +��ġ�� �����ϰ� �޴´� (���� ������ �������)
			result = GetLevelItem(type, index, monsterlevel);
			if( result >= 0  )
			{
			
				if( (type == 13 && index == 10) ||		// ���Ź���
					(type == 12 && index == 11) )		// ��ȯ����
				{
					if( InsertItem(monsterlevel, type, index, result) != 0xFF )
					{
						//if( monsterlevel >= 50 )
						{
							/*
							LogAdd("Item �߰�(level:%d) : %s type:%d index:%d level:%d", 
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
				else if( result <= maxlevel )		// �����ϰ� +�� ������ ������ �ش� ������ �ְ� ������ ������ ���� ���ϵ���
				{
					if( type == 12)					// ITEM_WING�� ��� ��ȯ������ ������ ���� +��ġ 0
					{
						if( index != 11 ) result = 0;
					}
					if( type == 12 && index == 11 )
					{

					}
					else if( result > maxlevel ) result = maxlevel;		// Ȥ�ö� +������ �ְ� ������ ������ ���� ��� ����

					if( (type == 4 && index == 7) || (type == 4 && index == 15) )	// ����,ȭ�� ��ų ����
					{
						result = 0;
					}
					
					if( InsertItem(monsterlevel, type, index, result) != 0xFF )
					{
						if( monsterlevel >= 43 && result >= 4 )
						{
						/*
						LogAdd("Item �߰�(%s,level:%d) : %s type:%d index:%d level:%d", 
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