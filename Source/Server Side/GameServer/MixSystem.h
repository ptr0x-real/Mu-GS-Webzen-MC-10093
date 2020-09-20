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
// 0.00.01	기존의 조합 시스템을 클래스 화(20070518, RKFKA)
// 0.00.02	ADD_FRUIT_N_WING_OF_CHAOS_MIX_20050502 삭제(20070521,RKFKA)
// 0.00.03	ChaosBox.h 삭제 -> CMixSystem 클래스로 코드 전환 (20070521, RKFKA)
// 0.00.04	#define DEVILSQUARE_CHAOSMONEY_MODIFY 삭제 (20070521, RKFKA)
// 0.00.05	#define CHAOS_MIX_UPGRADE 삭제 (20070521, RKFKA)
// 0.00.06	3차날개 조합 추가 (20070525, RKFKA)
// 0.00.07	3차 날개 옵션 확률 적용 (20070531, RKFKA)
//////////////////////////////////////////////////////////////////////////


#ifdef MODIFY_MIX_SYSTEM_20070518

#include "user.h"

// 3차날개 옵션3 확률
// -1: 해당 안됨(생명자동회복) 0:추가 공격력 1:추가 마력 2:추가 방어력
//int g_iThirdWingOpt3RateTable[3][5] = {
//	//기사	법사	요정	마검	다크
//	0,	-1,	0,	0,	0,	// 추가 공격력
//	-1,	1,	-1,	1,	0,	// 추가 마력
//	2,	2,	2,	-1,	2,	// 추가 방어력
//	-1,	-1,	-1,	-1,	-1,
//};

class CMixSystem  
{
private:
	int CHAOS_GEM_ITEMINDEX;		// 혼돈의보석
	int BLESS_GEM_ITEMINDEX;		// 축복의보석
	int SOUL_GEM_ITEMINDEX;			// 영혼의보석
	int CREATURE_GEM_ITEMINDEX;		// 창조의보석

	int BLESS_GEM_BUNDLE_ITEMINDEX;	// 축복의보석 묶음
	int SOUL_GEM_BULDLE_ITEMINDEX;	// 영혼의보석 묶음

	int CONDOR_FEATHER_ITEMINDEX;	// 콘도르의깃털
	int CONDOR_FLAME_ITEMINDEX;		// 콘도르의불꽃

	// 3차 날개
	int WING_OF_STORM_ITEMINDEX;	// 폭풍의날개
	int WING_OF_ETERNAL_ITEMINDEX;	// 시공의날개
	int WING_OF_ILLUSION_ITEMINDEX;	// 환영의날개
	int WING_OF_RUIN_ITEMINDEX;		// 파멸의날개
	int CAPE_OF_EMPEROR_ITEMINDEX;	// 제왕의망토
#ifdef ADD_SUMMONER_THIRD_WING_MIX_20071203	// 신규캐릭터 3차 날개 조합
	int WING_OF_DIMENSION_ITEMINDEX;	// 차원의 날개
#endif
	
	// 3차날개 조합 정보
	int m_i3rdWingOpt_LuckRate;		// 3차 날개에 행운 옵션이 붙을 확률
	
public:
	CMixSystem();
	virtual ~CMixSystem();

	BOOL ChaosBoxCheck( LPOBJECTSTRUCT lpObj );
	BOOL ChaosBoxInit( LPOBJECTSTRUCT lpObj );		// 조합창 초기화	

	// 카오스 아이템 조합, 1차 날개 조합
	BOOL ChaosBoxItemDown( LPOBJECTSTRUCT lpObj );	// 카오스박스 믹스 후 아이템들 처리
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	int  ChaosBoxMix( LPOBJECTSTRUCT lpObj, BOOL& Result2 ,int &iWingCharmItemNum);	
#else
	int  ChaosBoxMix( LPOBJECTSTRUCT lpObj, BOOL& Result2);	
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129	
	void DefaultChaosMix( LPOBJECTSTRUCT lpObj );	

	// 악마의 광장
	// 악마의 광장 초대권을 받을수 있는 아이템만 올려져 있는지 체크
	BOOL CheckDevilSquareItem( LPOBJECTSTRUCT lpObj, int& eventitemcount, int& itemlevel );
	// 데빌 스퀘어 믹스
	BOOL DevilSquareEventChaosMix( LPOBJECTSTRUCT lpObj, BOOL bCheckType, int iItemLevel );
	void DevilSquareItemChaosMix( LPOBJECTSTRUCT lpObj );		// 카오스 데빌스퀭어 아이템 조합
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	// 악마의 광장에서 검사된 아이템 로그 출력
	void LogPlusItemLevelChaosItem( LPOBJECTSTRUCT lpObj, int iPlusMixLevel );
#else
	void LogPlusItemLevelChaosItem( LPOBJECTSTRUCT lpObj );
#endif

	// +9이상 아이템에 대한 조합
	// +10, 11 구분한다 mixType == 1 +10 조합 mixType == 2 +11조합
	BOOL PlusItemLevelChaosMix( LPOBJECTSTRUCT lpObj, int mixType = 0 );

	// 디노란트 조합
	BOOL PegasiaChaosMix( LPOBJECTSTRUCT lpObj );	

	// 스탯 열매 조합
	BOOL CircleChaosMix( LPOBJECTSTRUCT lpObj );	

	// 2차 날개/망토 조합
	BOOL WingChaosMix( LPOBJECTSTRUCT lpObj );
	
	// 블러드캐슬
	void BloodCastleItemChaosMix( LPOBJECTSTRUCT lpObj );

#ifdef DARKLORD_WORK
	// 다크로드 펫 조합
	void DarkHorseChaosMix( LPOBJECTSTRUCT lpObj );
	void DarkSpiritChaosMix( LPOBJECTSTRUCT lpObj );
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	// 공성 아이템 조합
	void BlessPotionChaosMix( LPOBJECTSTRUCT lpObj );
	void SoulPotionChaosMix( LPOBJECTSTRUCT lpObj );
	void LifeStoneChaosMix( LPOBJECTSTRUCT lpObj );
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
	// 성주 조합
	void CastleSpecialItemMix( LPOBJECTSTRUCT lpObj );
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
	void HiddenTreasureBoxItemMix( LPOBJECTSTRUCT lpObj );
#endif
	
#ifdef ADD_ITEM_FENRIR_01_20051110 
	// 펜릴 조합
	void Fenrir_01Level_Mix( LPOBJECTSTRUCT lpObj );		// 1 단계 - 뿔피리조각 조합	
	void Fenrir_02Level_Mix( LPOBJECTSTRUCT lpObj );		// 2 단계 - 부러진 뿔피리 조합	
	void Fenrir_03Level_Mix( LPOBJECTSTRUCT lpObj );		// 3 단계 - 펜릴의 뿔피리 조합	
	void Fenrir_04Upgrade_Mix( LPOBJECTSTRUCT lpObj );	// 4 단계 - 펜릴의 뿔피리 업그레이드
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	// SD 물약 조합
	void ShieldPotionLv1_Mix( LPOBJECTSTRUCT lpObj );	// 작은 SD 회복 물약 조합	
	void ShieldPotionLv2_Mix( LPOBJECTSTRUCT lpObj );	// 중간 SD 회복 물약 조합	
	void ShieldPotionLv3_Mix( LPOBJECTSTRUCT lpObj );	// 큰 SD 회복 물약 조합
#endif

#ifdef ADD_THIRD_WING_20070525	// 3차 날개 조합 함수 선언
	// 조합 가능한 아이템인가?
	BOOL IsMixPossibleItem( int pItemType );

	// 날개 아이템인지 검사
	int CheckWingItem( int nType );
	int Check2ndWingItem( int nType );
	int Check3rdWingItem( int nType );

	// 특정 조건을 만족하는 아이템인가? (주어진 수치 이상이어야 한다.)
	int CheckLevelCondition( CItem* pItem, short pLevel = 0, BYTE pOpt1 = 0, BYTE pOpt2 = 0, BYTE pOpt3 = 0, BYTE pSetOption = 0, BYTE pExOpt = 0 );

	// 3차 날개 조합
	void ThirdWingLevel1_Mix( LPOBJECTSTRUCT lpObj );	// 3차 날개 조합 1단계
	void ThirdWingLevel2_Mix( LPOBJECTSTRUCT lpObj );	// 3차 날개 조합 2단계
	void ThirdWingMixFail( LPOBJECTSTRUCT lpObj );		// 3차 날개 조합 실패
	void ThirdWingMixFailItemPanalty( CItem* pItem );	// 3차 날개 조합 실패했을 때 아이템에 대한 패널티
#endif

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	// 카오스카드 조합을 추가
	void ChaosCardMix(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	void CherryBlossomMix(LPOBJECTSTRUCT lpObj);
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312
	
	// 세트 아이템 조합
	void SetItemChaosMix(LPOBJECTSTRUCT lpObj);		// 사용하지 않는다
	
	//void CBUPR_ItemRequestRecv( LPCB_PMSG_REQ_PRESENT_RESULT lpMsg);
	void CBUPS_ItemRequest( int aIndex, char *AccountId, char *Name );
	
	// 카오스캐슬
	// 카오스 아이템 로그 출력
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
	// 1. 소켓 아이템 조합
	// 1-1. 시드를 아이템으로부터 추출
	void MixExtractSeed(LPOBJECTSTRUCT lpObj);
	// 1-2. 시드와 스피어 합성
	void MixCompositeSeedSphere(LPOBJECTSTRUCT lpObj);
	// 1-3. 시드스피어를 소켓 아이템에 적용
	void MixSetSeedSphere(LPOBJECTSTRUCT lpObj, BYTE btSocketSlotIndex);
	// 1-4. 소켓 아이템에서 시드스피어를 삭제
	void MixRemoveSeedSphere(LPOBJECTSTRUCT lpObj, BYTE btSocketSlotIndex);
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

};

extern void ChaosEventProtocolCore( BYTE protoNum, BYTE *aRecv, int aLen );
extern CMixSystem g_MixSystem;

#endif
#endif // !defined(AFX_MIXSYSTEM_H__B5BE9A91_3E8F_40EC_9DA6_3B505649CE30__INCLUDED_)
