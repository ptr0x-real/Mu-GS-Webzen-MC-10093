// MasterLevelSystem.h: interface for the CMasterLevelSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERLEVELSYSTEM_H__2F3543A9_CA19_4471_842B_FB5451ABCD02__INCLUDED_)
#define AFX_MASTERLEVELSYSTEM_H__2F3543A9_CA19_4471_842B_FB5451ABCD02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912

const int ML_DEFAULT_ADD_POINT			= 1;	// 마스터레벨 포인트 기본 증가값

// 죽었을 때 경험치 손실 값(1/1000)
const int ML_DIE_DECEXP_RATE_COMMON		= 7;	// 일반인
const int ML_DIE_DECEXP_RATE_PKLEVEL1	= 20;	// 살인자 1단계
const int ML_DIE_DECEXP_RATE_PKLEVEL2	= 30;	// 살인자 2단계
const int ML_DIE_DECEXP_RATE_PKLEVEL3	= 40;	// 살인자 3단계

// 죽었을 때 젠 손실 값(1/100)
const int ML_DIE_DECZEN_RATE			= 4;

class CMasterLevelSystem  
{
public:
	CMasterLevelSystem();
	virtual ~CMasterLevelSystem();

private:

	INT64		m_i64MasterLevelExpTlb[MAX_MASTER_LEVEL +1];	// 마스터레벨 경험치 테이블

	int			m_iMinMonsterKillLevel;							// 마스터레벨 유저가 경험치를 얻을 수 있는 최소 몬스터 레벨
	float		m_fAddExpRate;									// 마스터레벨 추가 경험치(%)


public:	
	//////////////////////////////////////////////////////////////////////////
	// 마스터레벨 기본 처리
	//////////////////////////////////////////////////////////////////////////	
	VOID	LoadData();																// 파일에서 정보 읽기
	VOID	SetMasterLevelExpTlb();													// 마스터레벨 경험치 테이블값 설정
	VOID	gObjNextMLExpCal( LPOBJECTSTRUCT lpObj );								// 다음 레벨로 가기위한 최대 경험치
	BOOL	MasterLevelUp( LPOBJECTSTRUCT lpObj, INT64 iAddExp, bool bEventMapReward = false);						// 마스터레벨업 처리 (여기서 경험치 추가)

	BOOL	IsMasterLevelUser( LPOBJECTSTRUCT lpObj );								// 마스터레벨 유저인가? TRUE/FALSE
	BOOL	CheckMLGetExp( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj );		// 마스터레벨 유저가 경험치를얻을 수 있는 조건인지 체크

	INT64	GetMasterLevelExpTlbInfo( int iMasterLevel );							// 해당 마스터레벨의 최대 경험치를 돌려준다.
	INT		GetDieDecExpRate( LPOBJECTSTRUCT lpObj );								// 사망 시 경험치 손실 값(1/1000)
	INT		GetDieDecZenRate( LPOBJECTSTRUCT lpObj );								// 사망 시 젠 손실 값(1/1000)


	//////////////////////////////////////////////////////////////////////////
	// DS <-> GS 처리
	//////////////////////////////////////////////////////////////////////////
	VOID GDReqMasterLevelInfo( LPOBJECTSTRUCT lpObj );		// GD [0x30] 마스터레벨 정보 요청
	VOID DGAnsMasterLevelInfo( LPBYTE aRecv );				// DG [0x30] 마스터레벨 정보 요청 결과 	
	VOID GDReqMasterLevelInfoSave( LPOBJECTSTRUCT lpObj );	// GD [0x31] 마스터레벨 정보 저장
	

	//////////////////////////////////////////////////////////////////////////
	// Client <-> GS 처리
	//////////////////////////////////////////////////////////////////////////
	VOID GCMasterLevelInfo( LPOBJECTSTRUCT lpObj );			// GC [0xF3][0x50] 마스터레벨 정보
	VOID GCMasterLevelUpInfo( LPOBJECTSTRUCT lpObj );		// GC [0xF3][0x51] 마스터레벨이 올랐다. 



};

extern CMasterLevelSystem g_MasterLevelSystem;
#endif

#endif // !defined(AFX_MASTERLEVELSYSTEM_H__2F3543A9_CA19_4471_842B_FB5451ABCD02__INCLUDED_)
