// MixSystem.h: interface for the CMixSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIXSYSTEM_H__B5BE9A91_3E8F_40EC_9DA6_3B505649CE30__INCLUDED_)
#define AFX_MIXSYSTEM_H__B5BE9A91_3E8F_40EC_9DA6_3B505649CE30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////////
//
// History
// 0.00.01	������ ���� �ý����� Ŭ���� ȭ(20070518, RKFKA)
// 0.00.02	ADD_FRUIT_N_WING_OF_CHAOS_MIX_20050502 ����(20070521,RKFKA)
// 0.00.03	ChaosBox.h ���� -> CMixSystem Ŭ������ �ڵ� ��ȯ (20070521, RKFKA)
// 0.00.04	#define DEVILSQUARE_CHAOSMONEY_MODIFY ���� (20070521, RKFKA)
// 0.00.05	#define CHAOS_MIX_UPGRADE ���� (20070521, RKFKA)
// 0.00.06	3������ ���� �߰� (20070525, RKFKA)
// 0.00.07	3�� ���� �ɼ� Ȯ�� ���� (20070531, RKFKA)
//////////////////////////////////////////////////////////////////////////


#ifdef MODIFY_MIX_SYSTEM_20070518

#include "user.h"

// 3������ �ɼ�3 Ȯ��
// -1: �ش� �ȵ�(�����ڵ�ȸ��) 0:�߰� ���ݷ� 1:�߰� ���� 2:�߰� ����
//int g_iThirdWingOpt3RateTable[3][5] = {
//	//���	����	����	����	��ũ
//	0,	-1,	0,	0,	0,	// �߰� ���ݷ�
//	-1,	1,	-1,	1,	0,	// �߰� ����
//	2,	2,	2,	-1,	2,	// �߰� ����
//	-1,	-1,	-1,	-1,	-1,
//};

class CMixSystem  
{
private:
	int CHAOS_GEM_ITEMINDEX;		// ȥ���Ǻ���
	int BLESS_GEM_ITEMINDEX;		// �ູ�Ǻ���
	int SOUL_GEM_ITEMINDEX;			// ��ȥ�Ǻ���
	int CREATURE_GEM_ITEMINDEX;		// â���Ǻ���

	int BLESS_GEM_BUNDLE_ITEMINDEX;	// �ູ�Ǻ��� ����
	int SOUL_GEM_BULDLE_ITEMINDEX;	// ��ȥ�Ǻ��� ����

	int CONDOR_FEATHER_ITEMINDEX;	// �ܵ����Ǳ���
	int CONDOR_FLAME_ITEMINDEX;		// �ܵ����ǺҲ�

	// 3�� ����
	int WING_OF_STORM_ITEMINDEX;	// ��ǳ�ǳ���
	int WING_OF_ETERNAL_ITEMINDEX;	// �ð��ǳ���
	int WING_OF_ILLUSION_ITEMINDEX;	// ȯ���ǳ���
	int WING_OF_RUIN_ITEMINDEX;		// �ĸ��ǳ���
	int CAPE_OF_EMPEROR_ITEMINDEX;	// �����Ǹ���
#ifdef ADD_SUMMONER_THIRD_WING_MIX_20071203	// �ű�ĳ���� 3�� ���� ����
	int WING_OF_DIMENSION_ITEMINDEX;	// ������ ����
#endif
	
	// 3������ ���� ����
	int m_i3rdWingOpt_LuckRate;		// 3�� ������ ��� �ɼ��� ���� Ȯ��
	
public:
	CMixSystem();
	virtual ~CMixSystem();

	BOOL ChaosBoxCheck( LPOBJECTSTRUCT lpObj );
	BOOL ChaosBoxInit( LPOBJECTSTRUCT lpObj );		// ����â �ʱ�ȭ	

	// ī���� ������ ����, 1�� ���� ����
	BOOL ChaosBoxItemDown( LPOBJECTSTRUCT lpObj );	// ī�����ڽ� �ͽ� �� �����۵� ó��
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	int  ChaosBoxMix( LPOBJECTSTRUCT lpObj, BOOL& Result2 ,int &iWingCharmItemNum);	
#else
	int  ChaosBoxMix( LPOBJECTSTRUCT lpObj, BOOL& Result2);	
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129	
	void DefaultChaosMix( LPOBJECTSTRUCT lpObj );	

	// �Ǹ��� ����
	// �Ǹ��� ���� �ʴ���� ������ �ִ� �����۸� �÷��� �ִ��� üũ
	BOOL CheckDevilSquareItem( LPOBJECTSTRUCT lpObj, int& eventitemcount, int& itemlevel );
	// ���� ������ �ͽ�
	BOOL DevilSquareEventChaosMix( LPOBJECTSTRUCT lpObj, BOOL bCheckType, int iItemLevel );
	void DevilSquareItemChaosMix( LPOBJECTSTRUCT lpObj );		// ī���� ���������� ������ ����
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	// �Ǹ��� ���忡�� �˻�� ������ �α� ���
	void LogPlusItemLevelChaosItem( LPOBJECTSTRUCT lpObj, int iPlusMixLevel );
#else
	void LogPlusItemLevelChaosItem( LPOBJECTSTRUCT lpObj );
#endif

	// +9�̻� �����ۿ� ���� ����
	// +10, 11 �����Ѵ� mixType == 1 +10 ���� mixType == 2 +11����
	BOOL PlusItemLevelChaosMix( LPOBJECTSTRUCT lpObj, int mixType = 0 );

	// ����Ʈ ����
	BOOL PegasiaChaosMix( LPOBJECTSTRUCT lpObj );	

	// ���� ���� ����
	BOOL CircleChaosMix( LPOBJECTSTRUCT lpObj );	

	// 2�� ����/���� ����
	BOOL WingChaosMix( LPOBJECTSTRUCT lpObj );
	
	// ����ĳ��
	void BloodCastleItemChaosMix( LPOBJECTSTRUCT lpObj );

#ifdef DARKLORD_WORK
	// ��ũ�ε� �� ����
	void DarkHorseChaosMix( LPOBJECTSTRUCT lpObj );
	void DarkSpiritChaosMix( LPOBJECTSTRUCT lpObj );
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	// ���� ������ ����
	void BlessPotionChaosMix( LPOBJECTSTRUCT lpObj );
	void SoulPotionChaosMix( LPOBJECTSTRUCT lpObj );
	void LifeStoneChaosMix( LPOBJECTSTRUCT lpObj );
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
	// ���� ����
	void CastleSpecialItemMix( LPOBJECTSTRUCT lpObj );
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
	void HiddenTreasureBoxItemMix( LPOBJECTSTRUCT lpObj );
#endif
	
#ifdef ADD_ITEM_FENRIR_01_20051110 
	// �渱 ����
	void Fenrir_01Level_Mix( LPOBJECTSTRUCT lpObj );		// 1 �ܰ� - ���Ǹ����� ����	
	void Fenrir_02Level_Mix( LPOBJECTSTRUCT lpObj );		// 2 �ܰ� - �η��� ���Ǹ� ����	
	void Fenrir_03Level_Mix( LPOBJECTSTRUCT lpObj );		// 3 �ܰ� - �渱�� ���Ǹ� ����	
	void Fenrir_04Upgrade_Mix( LPOBJECTSTRUCT lpObj );	// 4 �ܰ� - �渱�� ���Ǹ� ���׷��̵�
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	// SD ���� ����
	void ShieldPotionLv1_Mix( LPOBJECTSTRUCT lpObj );	// ���� SD ȸ�� ���� ����	
	void ShieldPotionLv2_Mix( LPOBJECTSTRUCT lpObj );	// �߰� SD ȸ�� ���� ����	
	void ShieldPotionLv3_Mix( LPOBJECTSTRUCT lpObj );	// ū SD ȸ�� ���� ����
#endif

#ifdef ADD_THIRD_WING_20070525	// 3�� ���� ���� �Լ� ����
	// ���� ������ �������ΰ�?
	BOOL IsMixPossibleItem( int pItemType );

	// ���� ���������� �˻�
	int CheckWingItem( int nType );
	int Check2ndWingItem( int nType );
	int Check3rdWingItem( int nType );

	// Ư�� ������ �����ϴ� �������ΰ�? (�־��� ��ġ �̻��̾�� �Ѵ�.)
	int CheckLevelCondition( CItem* pItem, short pLevel = 0, BYTE pOpt1 = 0, BYTE pOpt2 = 0, BYTE pOpt3 = 0, BYTE pSetOption = 0, BYTE pExOpt = 0 );

	// 3�� ���� ����
	void ThirdWingLevel1_Mix( LPOBJECTSTRUCT lpObj );	// 3�� ���� ���� 1�ܰ�
	void ThirdWingLevel2_Mix( LPOBJECTSTRUCT lpObj );	// 3�� ���� ���� 2�ܰ�
	void ThirdWingMixFail( LPOBJECTSTRUCT lpObj );		// 3�� ���� ���� ����
	void ThirdWingMixFailItemPanalty( CItem* pItem );	// 3�� ���� ���� �������� �� �����ۿ� ���� �г�Ƽ
#endif

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	// ī����ī�� ������ �߰�
	void ChaosCardMix(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	void CherryBlossomMix(LPOBJECTSTRUCT lpObj);
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312
	
	// ��Ʈ ������ ����
	void SetItemChaosMix(LPOBJECTSTRUCT lpObj);		// ������� �ʴ´�
	
	//void CBUPR_ItemRequestRecv( LPCB_PMSG_REQ_PRESENT_RESULT lpMsg);
	void CBUPS_ItemRequest( int aIndex, char *AccountId, char *Name );
	
	// ī����ĳ��
	// ī���� ������ �α� ���
	void LogDQChaosItem( LPOBJECTSTRUCT lpObj );
	void LogChaosItem( LPOBJECTSTRUCT lpObj, char* sLogType );

#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702
	//BOOL	IsChaosMixCham(int iCnt, int iItemNum);
	//INT		ChaosMixChamItemCnt(int iCnt, int iItemNum);
	void ChaosMixCharmItemUsed(LPOBJECTSTRUCT lpObj);
	BOOL IsDeleteItem(const int iItemNum);
#endif // ADD_CHAOSMIXCHARM_ITEM_20080702
	
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
	//////////////////////////////////////////////////////////////////////////
	// 1. ���� ������ ����
	// 1-1. �õ带 ���������κ��� ����
	void MixExtractSeed(LPOBJECTSTRUCT lpObj);
	// 1-2. �õ�� ���Ǿ� �ռ�
	void MixCompositeSeedSphere(LPOBJECTSTRUCT lpObj);
	// 1-3. �õ彺�Ǿ ���� �����ۿ� ����
	void MixSetSeedSphere(LPOBJECTSTRUCT lpObj, BYTE btSocketSlotIndex);
	// 1-4. ���� �����ۿ��� �õ彺�Ǿ ����
	void MixRemoveSeedSphere(LPOBJECTSTRUCT lpObj, BYTE btSocketSlotIndex);
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

};

extern void ChaosEventProtocolCore( BYTE protoNum, BYTE *aRecv, int aLen );
extern CMixSystem g_MixSystem;

#endif
#endif // !defined(AFX_MIXSYSTEM_H__B5BE9A91_3E8F_40EC_9DA6_3B505649CE30__INCLUDED_)
