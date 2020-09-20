// KanturuBattleOfMaya.h: interface for the CKanturuBattleOfMaya class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLEOFMAYA_H__1B86D3A8_95C2_4B5E_B6CF_4E1F3FDFEAD5__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUBATTLEOFMAYA_H__1B86D3A8_95C2_4B5E_B6CF_4E1F3FDFEAD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"
#include "KanturuMaya.h"

enum KANTURU_MAYA_HAND_AI_GROUP_NUMBER
{
	MAYA_LEFT_HAND_GROUP_NUMBER		= 0,	// ���� �޼հ��� ������ ���� �޼��� �׷��ȣ
	MAYA_RIGHT_HAND_GROUP_NUMBER	= 1,	// ���� �����հ��� ������ ���� �������� �׷��ȣ
	MAYA_HANDS_GROUP_NUMBER			= 2,	// ���� ��հ��� ������ ���� ����� �׷��ȣ
	MAX_MAYA_HAND_AI_GROUP_NUMBER	= 3		
};

#define MAX_AI_STATE		4

class CKanturuBattleOfMaya  
{

private:
	// DATA LOAD
	BOOL						m_bFileDataLoad;

	// STATE
	int							m_iBattleOfMayaState;		// ������ ��ü�� ���� ����
	int							m_iSceneState;				// ������ ���� ���⿡ ���� ����

	// STATE INFO
	CKanturuStateInfo			m_BattleTimeInfo[MAX_BATTLE_STATE_BATTLE_OF_MAYA];
	int							m_BattleTimeInfoCount;
	CKanturuStateInfo			m_StateInfo[MAX_BATTLE_OF_MAYA_STATE];
	int							m_StateInfoCount;	

	// INFORMATION
	BOOL						m_bEntrancePermit;			// �������� ���� ����/�Ұ�
	
	// SUCCESS CHECK
	BOOL						m_bIsSucccess;				// 1�� �������� �����ߴ°�?
	BOOL						m_bIsSceneSuccess;			// �� ���º� ����

	// MONSTER
	CKanturuMaya				m_KanturuMaya;
	int							m_iMayaHandDieTimeCounter;	// ���� ���� �׾��� �� �ð� üũ�� ���� ����
	
	int							m_iMayaLeftHandObjIndex;		// ���� �޼� Object Index
	int							m_iMayaRightHandObjIndex;		// ���� ������ Object Index

	int							m_iMayaHandAIChangeTime[MAX_AI_STATE];		// ���� �� AI Change Time
	int							m_iMayaHandAIChangeTimeCount;
	int							m_iMayaHandAIAppliedTime;		// ���� �� AI ���� �ð�
	int							m_iMayaHandCurrentAI;			// ���� �� ���� AI

	int							m_iMayaHandGroupNumber[MAX_MAYA_HAND_AI_GROUP_NUMBER];	// ���� �� AI Group Number
	int							m_iMayaHAndGroupNumberCount;

	int							m_iMayaIceStormUsingRate;		// ���� ���̽����� ��ų ��� Ȯ��
	
public:
	CKanturuBattleOfMaya();
	virtual ~CKanturuBattleOfMaya();
	
	void	ResetAllData();
	BOOL	LoadData( LPSTR lpszFileName );
	void	Run();
	
	void	SetState( int iBattleOfMayaState );
	void	SetNextState( int iCurrentState );

	void	SetState_NONE();
	void	SetState_STANBY1();
	void	SetState_NOTIFY1();
	void	SetState_START1();	
	void	SetState_MAYA1();
	void	SetState_END_MAYA1();
	void	SetState_ENDCYCLE_MAYA1();
	void	SetState_STANBY2();
	void	SetState_START2();
	void	SetState_MAYA2();
	void	SetState_END_MAYA2();
	void	SetState_ENDCYCLE_MAYA2();
	void	SetState_STANBY3();
	void	SetState_START3();
	void	SetState_MAYA3();
	void	SetState_END_MAYA3();
	void	SetState_ENDCYCLE_MAYA3();
	void	SetState_END();
	void	SetState_ENDCYCLE();

	void	ProcState_NONE();                          
	void	ProcState_STANBY1();  
	void	ProcState_NOTIFY1();
	void	ProcState_START1();	                          
	void	ProcState_MAYA1();                          
	void	ProcState_END_MAYA1();                        
	void	ProcState_ENDCYCLE_MAYA1();               
	void	ProcState_STANBY2();                          
	void	ProcState_START2();                          
	void	ProcState_MAYA2();               
	void	ProcState_END_MAYA2();               
	void	ProcState_ENDCYCLE_MAYA2();                
	void	ProcState_STANBY3();                          
	void	ProcState_START3();                          	       
	void	ProcState_MAYA3();                          
	void	ProcState_END_MAYA3();	 
	void	ProcState_ENDCYCLE_MAYA3();                
	void	ProcState_END();                          
	void	ProcState_ENDCYCLE();    

	// BATTLE OF MAYA INFORMATION	
	void	SetBattleOfMayaState( int iState )				{ m_iBattleOfMayaState = iState; }
	int		GetBattleOfMayaState()							{ return m_iBattleOfMayaState; }	
	void	SetBattleSceneState( int iScene )				
	{ 
		m_iSceneState		= iScene; 
		m_bIsSceneSuccess	= FALSE;
	}
	int		GetBattleSceneState()							{ return m_iSceneState; }
	
	void	SetSuccessValue( BOOL bIsSuccess )				{ m_bIsSucccess	= bIsSuccess; }
	BOOL	GetSuccessValue()								{ return m_bIsSucccess; }
	void	SetSceneSuccessValue( BOOL bIsSuccess )			{ m_bIsSceneSuccess	= bIsSuccess; }
	BOOL	GetSceneSuccessValue()							{ return m_bIsSceneSuccess; }

	void	SetEntrancePermit( BOOL bEntrancePermit )		{ m_bEntrancePermit = bEntrancePermit; }
	BOOL	GetEntrancePermit()							{ return m_bEntrancePermit; }
	
	// Schedule
	BOOL	CheckStateTime(); 
	int		GetRemainTime();
	void	CheckMayaBattleUser();

	// MONSTER AI
	void	SetAIMonsterGroup( int iGroupNumber );
	void	ResetAIMonster();
	void	ChangeAI( int iGroupNumber );
	
};

#endif // !defined(AFX_KANTURUBATTLEOFMAYA_H__1B86D3A8_95C2_4B5E_B6CF_4E1F3FDFEAD5__INCLUDED_)
