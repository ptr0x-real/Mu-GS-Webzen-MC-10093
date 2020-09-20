// QuestInfo.cpp: implementation of the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Include\Readscript.h"
#include "Include\Prodef.h"
#include "Include\SProdef.h"
#include "Common\winutil.h"
#include "Include\GMProtocol.h"
#include "Include\Public.h"

#include "User.h"
#include "protocol.h"
#include "QuestInfo.h"
#include "PartyClass.h"
#include "gObjMonster.h"
#include "DSProtocol.h"

#include "Gamemain.h"

#ifdef SCRIPT_DECODE_WORK	
#include "Common\WZScriptEncode.h"
#endif

#ifdef AUTH_GAMESERVER
#include "Common\WzMemScript.h"
#endif

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
#include "QuestUtil.h"
#endif

extern ITEM_ATTRIBUTE   ItemAttribute[MAX_ITEM];
extern PartyClass		gParty;


BYTE	QuestBitMask[8];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef NEW_SKILL_FORSKYLAND

CQuestInfo	g_QuestInfo;

CQuestInfo::CQuestInfo()
{
	QuestBitMask[0] = 0xFC;
	QuestBitMask[1] = 0xFC;
	QuestBitMask[2] = 0xF3;
	QuestBitMask[3] = 0xF3;

	QuestBitMask[4] = 0xCF;
	QuestBitMask[5] = 0xCF;
	QuestBitMask[6] = 0x3F;
	QuestBitMask[7] = 0x3F;

	Init();
}

CQuestInfo::~CQuestInfo()
{

}
void CQuestInfo::Init()
{
	m_QuestCount = 0;
	memset(QuestInfo, -1, sizeof(QuestInfo));	

	for( int i = 0; i < MAX_QUEST; i++ )
	{
		QuestInfo[i].QuestConditionCount = 0;
		QuestInfo[i].QuestSubInfoCount = 0;
	}
}

BOOL CQuestInfo::LoadQuestInfo(char* filename)
{	
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("load error %s", filename);
		return FALSE;
	}

	Init();

	SMDToken Token;	
	int n=0;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;

			if( Type == 0 )
			{	// 퀘스트 정보
				Token = (*GetToken)();
				int QuestIndex = (int)TokenNumber;
				QuestInfo[QuestIndex].QuestIndex = QuestIndex;

				Token = (*GetToken)();	
				QuestInfo[QuestIndex].QuestStartType = (int)TokenNumber;

				Token = (*GetToken)();	
				QuestInfo[QuestIndex].QuestStartSubType = (int)TokenNumber;

				Token = (*GetToken)();	
				strcpy(QuestInfo[QuestIndex].Name, TokenString);
				
				QuestInfo[QuestIndex].QuestSubInfoCount = 0;

				while(1)
				{
					int index = QuestInfo[QuestIndex].QuestSubInfoCount;

					Token = (*GetToken)();	
					if(Token==NAME && strcmp("end",TokenString)==NULL) 
						break;
					
					QuestInfo[QuestIndex].QuestSubInfo[index].QuestType = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedType = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedSubType = (int)TokenNumber;

#ifdef SECOND_QUEST
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].ItemLevel = (int)TokenNumber;					
#endif

					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedNumber = (int)TokenNumber;

					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedTargetMinLevel = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedTargetMaxLevel = (int)TokenNumber;

					
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedDropRate = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardType = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardSubType = (int)TokenNumber;					
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardCount = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].LinkConditionIndex = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextBeforeReg = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextAfterReg = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextCompleteQuest = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextQuestClear = (int)TokenNumber;

					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[0] = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[1] = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[2] = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[3] = (int)TokenNumber;
					
#ifdef DARKLORD_WORK
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[4] = (int)TokenNumber;
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// quset 사용 클래스 추가
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[5] = (int)TokenNumber;
#endif
					
					QuestInfo[QuestIndex].QuestSubInfoCount++;
				}

				LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d", 
					   QuestInfo[QuestIndex].Name, QuestIndex, QuestInfo[QuestIndex].QuestSubInfoCount);

				m_QuestCount++;					   
			}
			else if( Type == 1 )
			{	// 퀘스트 요구조건
				Token = (*GetToken)();
				int QuestIndex = (int)TokenNumber;
				
				while(1)
				{
					int index = QuestInfo[QuestIndex].QuestConditionCount;
					
					Token = (*GetToken)();	
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					QuestInfo[QuestIndex].QuestCondition[index].Index = (int)TokenNumber;

					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestCondition[index].NeedQuestIndex = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestCondition[index].MinLevel = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestCondition[index].MaxLevel = (int)TokenNumber;

#ifdef SECOND_QUEST
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestCondition[index].ReqStr = (int)TokenNumber;
#endif

					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestCondition[index].NeedZen = (int)TokenNumber;
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestCondition[index].StartContext = (int)TokenNumber;

					QuestInfo[QuestIndex].QuestConditionCount++;
				}
			}
			else
			{
				MsgBox("Quest Info Read Fail");
				fclose(SMDFile);
				return FALSE;
			}
		}
	}
	fclose(SMDFile);

	InitQuestItem();
	return TRUE;
}

#ifdef AUTH_GAMESERVER
BOOL CQuestInfo::LoadQuestInfo(char* Buffer, int iSize)
{	
	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);
	
	Init();

	SMDToken Token;	
	int n=0;

	while( true )
	{		
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{			
			int Type = WzMemScript.GetNumber();

			if( Type == 0 )
			{	// 퀘스트 정보
				Token = WzMemScript.GetToken();
				int QuestIndex = WzMemScript.GetNumber();
				QuestInfo[QuestIndex].QuestIndex = QuestIndex;

				Token = WzMemScript.GetToken();	
				QuestInfo[QuestIndex].QuestStartType = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();	
				QuestInfo[QuestIndex].QuestStartSubType = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();					
				strcpy(QuestInfo[QuestIndex].Name, WzMemScript.GetString());
				
				QuestInfo[QuestIndex].QuestSubInfoCount = 0;

				while(1)
				{
					int index = QuestInfo[QuestIndex].QuestSubInfoCount;

					Token = WzMemScript.GetToken();	
					if(Token==NAME && strcmp("end",WzMemScript.GetString())==NULL) 
						break;
					
					QuestInfo[QuestIndex].QuestSubInfo[index].QuestType = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedType = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedSubType = WzMemScript.GetNumber();

#ifdef SECOND_QUEST
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ItemLevel = WzMemScript.GetNumber();					
#endif

					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedNumber = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedTargetMinLevel = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedTargetMaxLevel = WzMemScript.GetNumber();

					
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedDropRate = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardType = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardSubType = WzMemScript.GetNumber();					
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardCount = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].LinkConditionIndex = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextBeforeReg = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextAfterReg = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextCompleteQuest = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextQuestClear = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[0] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[1] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[2] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[3] = WzMemScript.GetNumber();

#ifdef DARKLORD_WORK
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[4] = WzMemScript.GetNumber();
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// quset 사용 클래스 추가
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[5] = WzMemScript.GetNumber();
#endif

					QuestInfo[QuestIndex].QuestSubInfoCount++;
				}

				LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d", 
					   QuestInfo[QuestIndex].Name, QuestIndex, QuestInfo[QuestIndex].QuestSubInfoCount);

				m_QuestCount++;					   
			}
			else if( Type == 1 )
			{	// 퀘스트 요구조건
				Token = WzMemScript.GetToken();
				int QuestIndex = WzMemScript.GetNumber();
				
				while(1)
				{
					int index = QuestInfo[QuestIndex].QuestConditionCount;
					
					Token = WzMemScript.GetToken();	
					if(Token==NAME && strcmp("end",WzMemScript.GetString())==NULL) break;

					QuestInfo[QuestIndex].QuestCondition[index].Index = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].NeedQuestIndex = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].MinLevel = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].MaxLevel = WzMemScript.GetNumber();

#ifdef SECOND_QUEST
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].ReqStr = WzMemScript.GetNumber();
#endif

					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].NeedZen = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].StartContext = WzMemScript.GetNumber();

					QuestInfo[QuestIndex].QuestConditionCount++;
				}
			}
			else
			{
				MsgBox("Quest Info Read Fail");
				fclose(SMDFile);
				return FALSE;
			}
		}
	}
	InitQuestItem();
	return TRUE;
}
#endif

#ifdef SCRIPT_DECODE_WORK
BOOL CQuestInfo::LoadQuestInfoDecode(char* filename)
{	
	Init();

	CWZScriptEncode		Encoder;
	if( !Encoder.Open(filename) )
	{
		MsgBox("load error %s", filename);
		return FALSE;
	}

	WZSMDToken Token;
	
	int n=0;

	while( true )
	{
		Token = Encoder.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Type = Encoder.GetNumber();

			if( Type == 0 )
			{	// 퀘스트 정보
				Token = Encoder.GetToken();
				int QuestIndex = Encoder.GetNumber();
				QuestInfo[QuestIndex].QuestIndex = QuestIndex;

				Token = Encoder.GetToken();	
				QuestInfo[QuestIndex].QuestStartType = Encoder.GetNumber();

				Token = Encoder.GetToken();	
				QuestInfo[QuestIndex].QuestStartSubType = Encoder.GetNumber();

				Token = Encoder.GetToken();	
				strcpy(QuestInfo[QuestIndex].Name, Encoder.GetString());
				
				QuestInfo[QuestIndex].QuestSubInfoCount = 0;

				while(1)
				{
					int index = QuestInfo[QuestIndex].QuestSubInfoCount;

					Token = Encoder.GetToken();	
					if(Token==NAME && strcmp("end",Encoder.GetString())==NULL) 
						break;
					
					QuestInfo[QuestIndex].QuestSubInfo[index].QuestType = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedType = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedSubType = Encoder.GetNumber();

#ifdef SECOND_QUEST
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ItemLevel = Encoder.GetNumber();					
#endif

					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedNumber = Encoder.GetNumber();

					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedTargetMinLevel = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedTargetMaxLevel = Encoder.GetNumber();

					
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].NeedDropRate = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardType = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardSubType = Encoder.GetNumber();					
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RewardCount = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].LinkConditionIndex = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextBeforeReg = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextAfterReg = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextCompleteQuest = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].ContextQuestClear = Encoder.GetNumber();

					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[0] = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[1] = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[2] = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[3] = Encoder.GetNumber();

#ifdef DARKLORD_WORK
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[4] = Encoder.GetNumber();
#endif
					QuestInfo[QuestIndex].QuestSubInfoCount++;
				}

				LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d", 
					   QuestInfo[QuestIndex].Name, QuestIndex, QuestInfo[QuestIndex].QuestSubInfoCount);

				m_QuestCount++;					   
			}
			else if( Type == 1 )
			{	// 퀘스트 요구조건
				Token = Encoder.GetToken();
				int QuestIndex = Encoder.GetNumber();
				
				while(1)
				{
					int index = QuestInfo[QuestIndex].QuestConditionCount;
					
					Token = Encoder.GetToken();	
					if(Token==NAME && strcmp("end",Encoder.GetString())==NULL) break;

					QuestInfo[QuestIndex].QuestCondition[index].Index = Encoder.GetNumber();

					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].NeedQuestIndex = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].MinLevel = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].MaxLevel = Encoder.GetNumber();

#ifdef SECOND_QUEST
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].ReqStr = Encoder.GetNumber();
#endif

					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].NeedZen = Encoder.GetNumber();
					Token = Encoder.GetToken();	QuestInfo[QuestIndex].QuestCondition[index].StartContext = Encoder.GetNumber();

					QuestInfo[QuestIndex].QuestConditionCount++;
				}
			}
			else
			{
				MsgBox("Quest Info Read Fail");
				fclose(SMDFile);
				return FALSE;
			}
		}
	}
	

	InitQuestItem();
	return TRUE;
}
#endif

void CQuestInfo::InitQuestItem()
{
	int questcount = GetQeustCount();
	int	foundquest = 0;

	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO	lpSubInfo;

	for( int i = 0; i < MAX_QUEST; i++ )
	{
		lpQuestInfo = GetQuestInfo(i);
		if( !lpQuestInfo )			
			continue;

		for( int j = 0; j < lpQuestInfo->QuestSubInfoCount; j++ )
		{
			lpSubInfo = &lpQuestInfo->QuestSubInfo[j];

			if( lpSubInfo )
			{
				if( lpSubInfo->QuestType == 1  )
				{	// 퀘스트 타입이 Item드랍일때만
					ITEM_ATTRIBUTE *p = &ItemAttribute[lpSubInfo->NeedType*MAX_ITEM_INDEX+lpSubInfo->NeedSubType];
					p->QuestItem = TRUE;
					LogAddTD("[Quest] [SetQuestItem] %s", p->Name);
				}
			}
		}
		
		foundquest++;
		if( foundquest == questcount )	
			break;
	}
}

BOOL CQuestInfo::IsQuest(int QuestIndex)
{
	if( QuestIndex < 0 || QuestIndex > MAX_QUEST )
		return FALSE;

	if( QuestInfo[QuestIndex].QuestIndex == -1 )
		return FALSE;
	
	return TRUE;
}

int	CQuestInfo::GetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	if( QuestIndex < 0 || QuestIndex > MAX_QUEST )
		return 0;

	int index = QuestIndex/4;
	int shift = (QuestIndex%4)*2;

	return (lpObj->m_Quest[index]>>shift)&0x03;
}

BYTE CQuestInfo::GetQuestStateBYTE(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	if( QuestIndex < 0 || QuestIndex > MAX_QUEST )
		return FALSE;

	int index = QuestIndex/4;
	return lpObj->m_Quest[index];
}

BYTE CQuestInfo::SetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex, int State)
{
	if( QuestIndex < 0 || QuestIndex > MAX_QUEST )
		return 0xff;		// 퀘스트가 없다

	int QuestState = GetQuestState(lpObj, QuestIndex);

	if( QuestState == QS_NOTHING || QuestState == QS_NONE )
	{	// 퀘스트를 진행할려고 한다면, 퀘스트 진행 가능 조건을 검사한다
		BYTE result = QuestRunConditionCheck(lpObj, QuestIndex);
		if(  result != 0 )
		{	// 조건에 맞지 않다
			return result;
		}			

		QuestAccept(lpObj, QuestIndex);

		LogAddTD("[Quest] AcceptQuest [%s][%s] (%s)", 
			     lpObj->AccountID, lpObj->Name, GetQuestInfo(QuestIndex)->Name);


		State = QS_ACCEPT;
	}
	else if( QuestState == QS_ACCEPT )
	{	// 클리어 할려고 한다
		BYTE result = QuestClearConditionCheck(lpObj, QuestIndex);
		if(  result != 0 )
		{	// 조건에 맞지 않다
			return result;
		}
		QuestClear(lpObj, QuestIndex);
		State = QS_CLEAR;

		LogAddTD("[Quest] ClearQuest [%s][%s] (%s)", 
			     lpObj->AccountID, lpObj->Name, GetQuestInfo(QuestIndex)->Name);
	}	
	else
	{
		return 0xff;
	}

	int index = QuestIndex/4;
	int shift = (QuestIndex%4)*2;
	
	(lpObj->m_Quest[index]) &= QuestBitMask[shift];
	(lpObj->m_Quest[index]) |= ((State&0x3)<<shift);	

	return 0;
}

BYTE CQuestInfo::ReSetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	int State = QS_NOTHING;
	int index = QuestIndex/4;
	int shift = (QuestIndex%4)*2;
	
	(lpObj->m_Quest[index]) &= QuestBitMask[shift];
	(lpObj->m_Quest[index]) |= ((State&0x3)<<shift);	

	return 0;
}

LPQUEST_INFO CQuestInfo::GetQuestInfo(int QuestIndex)
{
	if( QuestIndex < 0 || QuestIndex > MAX_QUEST )
		return NULL;

	if( QuestInfo[QuestIndex].QuestIndex == -1 )
	{
		return NULL;
	}

	return &QuestInfo[QuestIndex];
}

LPQUEST_SUB_INFO CQuestInfo::GetSubquestInfo(LPOBJECTSTRUCT lpObj, LPQUEST_INFO lpQuestInfo, int subquestindex)
{
	if( !lpQuestInfo )
		return NULL;	// 퀘스트가 없다

	if( subquestindex < 0 || subquestindex > MAX_QUEST_SUBINFO )
		return NULL;

	LPQUEST_SUB_INFO lpSubInfo = &lpQuestInfo->QuestSubInfo[subquestindex];

	int Class = lpObj->Class;
	int ChangeUP = lpObj->ChangeUP;	

	int requireclass = lpSubInfo->RequireClass[Class];
	if( requireclass == 0 ) return NULL;
	if( requireclass > 1 )
	{	// 전직전용이면
		if( requireclass != ChangeUP+1 )
			return NULL;
	}
	return lpSubInfo;
}

LPQUEST_CONDITION CQuestInfo::GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex)
{	
	if( !lpQuestInfo )
		return NULL;	// 퀘스트가 없다

	if( conditionindex < 0 || conditionindex > MAX_QUEST_CONDITION )
		return NULL;

	return &lpQuestInfo->QuestCondition[conditionindex];
}

BYTE CQuestInfo::QuestClearConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo;
	lpQuestInfo = GetQuestInfo(QuestIndex);

	if( !lpQuestInfo )
		return 0xff;	// 퀘스트가 없다

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;

	BOOL	bFoundSubQuest = FALSE;

	for( int subquest = 0; subquest < subquestcount; subquest++ )
	{
		lpSubInfo = GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if( lpSubInfo )
		{
			bFoundSubQuest = TRUE;
			if( lpSubInfo->QuestType == 1 )
			{
				
#ifdef SECOND_QUEST
				int itemcount = gObjGetItemCountInIventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);
#else
				int itemcount = gObjGetItemCountInIventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType);
#endif

				if( itemcount < lpSubInfo->NeedNumber )
				{	// 조건이 만족하지 않았다.
					return 1;
				}
			}
		}		
	}

	if( !bFoundSubQuest )
		return 0xff;

	return FALSE;
}

// 조건 검사
BYTE CQuestInfo::QuestRunConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo;
	lpQuestInfo = GetQuestInfo(QuestIndex);

	if( !lpQuestInfo )
		return 0xff;	// 퀘스트가 없다

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;

	BOOL	bFoundSubQuest = FALSE;

	for( int subquest = 0; subquest < subquestcount; subquest++ )
	{
		lpSubInfo = GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if( lpSubInfo )
		{
			bFoundSubQuest = TRUE;
			for( int condition = 0; condition < concount; condition++ )
			{	
				lpCondition = GetQuestCondition(lpQuestInfo, condition);

				if( lpCondition && lpCondition->Index == -1 )
				{	// 먼저 모든 조건에 해당되는 것을 검사한다
					if( !CompareCondition(lpObj, lpCondition) )
					{
						return lpCondition->StartContext;
					}
				}
				else if( lpCondition && lpCondition->Index == lpSubInfo->LinkConditionIndex )
				{	// 연결 되는 요구조검을 검사한다
					if( !CompareCondition(lpObj, lpCondition) )
					{
						return lpCondition->StartContext;
					}
				}
#ifdef QUEST_BUG_FIX
				else if( !lpCondition )
				{
					return 0xff;
				}
#endif				
			}
		}		
	}

	if( !bFoundSubQuest )
		return 0xff;

	return FALSE;
}

BOOL CQuestInfo::CompareCondition(LPOBJECTSTRUCT lpObj, LPQUEST_CONDITION lpCondition)
{
	if( lpCondition->NeedQuestIndex != -1 )
	{	// 그전에 클리어한 퀘스트가 클리어 되지 않았다
		if( GetQuestState(lpObj, lpCondition->NeedQuestIndex) != QS_CLEAR )
			return FALSE;
	}

	if( lpCondition->MinLevel != 0 )
	{
		if( lpCondition->MinLevel > lpObj->Level )
		{	// 최소 레벨에 맞지 않는다
			return FALSE;
		}
	}

	if( lpCondition->MaxLevel != 0 )
	{
		if( lpCondition->MaxLevel < lpObj->Level )
		{	// 최대 레벨에 맞지 않는다
			return FALSE;
		}
	}

	if( lpCondition->NeedZen > lpObj->Money )
	{	// 돈이 부족하다
		return FALSE;
	}

#ifdef SECOND_QUEST
	if( lpCondition->ReqStr > (lpObj->Strength+lpObj->AddStrength) )
	{
		return FALSE;
	}
#endif

	return TRUE;
}

BOOL CQuestInfo::NpcTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	int questcount = GetQeustCount();
	int	foundquest = 0;
	LPQUEST_INFO lpQuestInfo;

	int queststate = -1;
	int questindex = -1;	

	for( int i = 0; i < MAX_QUEST; i++ )
	{
		lpQuestInfo = GetQuestInfo(i);
		if( !lpQuestInfo )			
			continue;

		if( lpQuestInfo->QuestStartType == 2 )
		{	// 퀘스트 발동 타입이 NPC클릭이면
			if( lpNpc->Class == lpQuestInfo->QuestStartSubType )
			{	// 퀘스트 NPC면
				if( queststate == -1 )
				{	// 설정된 퀘스트가 없으면 퀘스트를 지정한다
					questindex = lpQuestInfo->QuestIndex;
					queststate = GetQuestState(lpObj, lpQuestInfo->QuestIndex);
				}
				else if( queststate == QS_CLEAR )
				{	// 중복으로 체크된 퀘스트가 있는데 
					// 이전에 체크된 퀘스트가 클리어 되어 있는 상태면, 다음 퀘스트를 진행시킨다
#ifdef SECOND_QUEST
					for( int j = 0; j < lpQuestInfo->QuestSubInfoCount; j++ )
					{
						LPQUEST_SUB_INFO lpSubInfo = GetSubquestInfo(lpObj, lpQuestInfo, j);
						if( lpSubInfo )
						{	// 서브정보가 있는 퀘스트만 인정한다
							questindex = lpQuestInfo->QuestIndex;
							queststate = GetQuestState(lpObj, lpQuestInfo->QuestIndex);
						}
					}
#else
					questindex = lpQuestInfo->QuestIndex;
					queststate = GetQuestState(lpObj, lpQuestInfo->QuestIndex);
#endif
				}
			}
		}

		foundquest++;
		if( foundquest == questcount )		
			break;
	}

	if( queststate != -1 )
	{
		// 몬스터 킬이 완료조건인 퀘스트면 킬카운트 정보를 먼저 보낸다.
#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
		BOOL bSendResult = QuestMonsterKillInfoSend( questindex, lpObj->m_Index );
		if( bSendResult == FALSE )
		{
			// 몬스터 킬카운트 다시 보내는 중 -> NPC의 퀘스트창을 열지 못하게 한다.		
			return TRUE;
		}
#endif		
		GCSendQuestInfo(lpObj->m_Index, questindex);
		
		lpObj->m_IfState.type   = I_QUEST;	
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

	return FALSE;
}

BOOL CQuestInfo::MonsterItemDrop(LPOBJECTSTRUCT lpObj)
{
	// 이까지 오면 무조건 떨어 진다
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

	if( MaxHitUser == -1 )
		return FALSE;

	int partycount = gParty.GetPartyCount(gObj[MaxHitUser].PartyNumber);
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 퀘스트를 진행하는 사람이 개인인지 파티원인지에 따라 처리가 달라짐
	if( partycount > 0 )
	{
		BOOL bResult = MonsterItemDropForParty( lpObj, &gObj[MaxHitUser] );
		return bResult;
	}		
#else
	if( partycount > 0 )
		return FALSE;	// 파티를 한 상태에서는 아이템을 얻을수 없다
#endif

	
	LPOBJECTSTRUCT lpTarget = &gObj[MaxHitUser];

	int questcount = GetQeustCount();
	int	foundquest = 0;

	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO	lpSubInfo;

	int type, level, x, y;
	float dur=0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;


	for( int i = 0; i < MAX_QUEST; i++ )
	{
		lpQuestInfo = GetQuestInfo(i);
		if( !lpQuestInfo )			
			continue;

		for( int j = 0; j < lpQuestInfo->QuestSubInfoCount; j++ )
		{
			lpSubInfo = GetSubquestInfo(lpTarget, lpQuestInfo, j);

			if( lpSubInfo )
			{
				if( lpSubInfo->QuestType == 1  )
				{	// 퀘스트 타입이 Item드랍일때만
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// NeedTargetMinLevel이 -1이면 
										// NeedTargetMaxLevel은 몬스터 인덱스로 특정 몬스터를 지정할 수 있다.
					if( ( lpSubInfo->NeedTargetMinLevel > 0 
							&& lpObj->Level >= lpSubInfo->NeedTargetMinLevel 
							&& lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
						|| ( lpSubInfo->NeedTargetMinLevel == -1 && lpObj->Class == lpSubInfo->NeedTargetMaxLevel ) )
#else
					if( lpObj->Level >= lpSubInfo->NeedTargetMinLevel && 
						lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
#endif
					{	// 퀘스트 대상 몬스터가 없을때나 퀘스트 대상 몬스터가 맞을때
						if( GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == QS_ACCEPT )
						{	// 해당 퀘스트 상태가 진행 중일때만
							// 현재 퀘스트 아이템이 몇개가 있는지 확인한다
							if( rand()%1000 < lpSubInfo->NeedDropRate )
							{
#ifdef SECOND_QUEST
								int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType, lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);
#else
								int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType, lpSubInfo->NeedSubType);
#endif

								if( itemcount >= lpSubInfo->NeedNumber )
								{	// 이미 필요한 만큼 가지고 있다면
									continue;
								}

								dur  = 0;
								x    = lpObj->X;
								y    = lpObj->Y;

#ifdef SECOND_QUEST
								level= lpSubInfo->ItemLevel;	
#else
								level= 0;	
#endif
								type = ItemGetNumberMake(lpSubInfo->NeedType, lpSubInfo->NeedSubType);
								ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);

								LogAddTD("[Quest] Quest Item Drop [%s]: [%s][%s] (%s) (%d,%d)", 
										 lpObj->Name, lpTarget->AccountID, lpTarget->Name, 
										 lpQuestInfo->Name, lpSubInfo->NeedType, lpSubInfo->NeedSubType);
								return TRUE;							
							}
						}
					}
				}
			}
		}
		
		foundquest++;
		if( foundquest == questcount )	
			break;
	}
		
	return FALSE;
}

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 퀘스트를 파티를 맺어 수행할 경우 처리
BOOL CQuestInfo::MonsterItemDropForParty( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpMaxHitObj )
{
	int questcount = GetQeustCount();
	int	foundquest = 0;

	LPQUEST_INFO		lpQuestInfo;
	LPQUEST_SUB_INFO	lpSubInfo;

	int type, level, x, y;
	float dur = 0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
		
	int iPartyNum = lpMaxHitObj->PartyNumber;
	int iPartyUserIndex;
	LPOBJECTSTRUCT lpPartyObj;

	if( iPartyNum < 0 )
		return FALSE;

	for( int iPatyUserCount = 0; iPatyUserCount < MAX_PARTYUSER; iPatyUserCount++ )
	{
		iPartyUserIndex = gParty.m_PartyS[iPartyNum].Number[iPatyUserCount];
		if( iPartyUserIndex < 0 )
			continue;

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// 퀘스트 아이템 파티드랍때 파티원간의 맵도 체크한다.
		if( gObj[iPartyUserIndex].MapNumber != lpMaxHitObj->MapNumber )
			continue;
#endif

		lpPartyObj = ( LPOBJECTSTRUCT )&gObj[iPartyUserIndex];	
		
		for( int iQuestCount = 0; iQuestCount < MAX_QUEST; iQuestCount++ )
		{
			lpQuestInfo = GetQuestInfo( iQuestCount );
			if( !lpQuestInfo )
				continue;

			for( int j = 0; j < lpQuestInfo->QuestSubInfoCount; j++ )
			{
				lpSubInfo = GetSubquestInfo( lpPartyObj, lpQuestInfo, j );

				if( !lpSubInfo )
					continue;

				if( lpSubInfo->QuestType == 1  )	// 퀘스트 타입이 Item 드랍
				{
					// 특정 몬스터를 잡는 퀘스트
					if( ( lpSubInfo->NeedTargetMinLevel > 0 
							&& lpObj->Level >= lpSubInfo->NeedTargetMinLevel 
							&& lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
						|| ( lpSubInfo->NeedTargetMinLevel == -1 && lpObj->Class == lpSubInfo->NeedTargetMaxLevel ) )					
					{
						if( GetQuestState( lpPartyObj, lpQuestInfo->QuestIndex ) == QS_ACCEPT )
						{	// 해당 퀘스트 상태가 진행 중일때, 개수제한 없이 떨어뜨린다.
							if( rand() % 1000 < lpSubInfo->NeedDropRate )
							{									
								dur		= 0;
								x		= lpMaxHitObj->X;
								y		= lpMaxHitObj->Y;
								level	= lpSubInfo->ItemLevel;										
								type	= ItemGetNumberMake( lpSubInfo->NeedType, lpSubInfo->NeedSubType );

								// 아이템 권한은 최대데미지 적용 유저에게.
								ItemSerialCreateSend( lpMaxHitObj->m_Index, lpMaxHitObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpMaxHitObj->m_Index );

								LogAddTD( "[Quest] Quest Item Drop(Party) [%s]: [%s][%s] (%s) (%d,%d)", 
										 lpObj->Name, lpMaxHitObj->AccountID, lpMaxHitObj->Name, 
										 lpQuestInfo->Name, lpSubInfo->NeedType, lpSubInfo->NeedSubType );
								return TRUE;							
							}
						}					
					}					
				}
			}
			
			foundquest++;
			if( foundquest == questcount )	
				break;
		}
	}

	return FALSE;
}
#endif	// THIRD_CHANGEUP_SYSTEM_20070507

BOOL CQuestInfo::QuestAccept(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo;
	lpQuestInfo = GetQuestInfo(QuestIndex);

	if( !lpQuestInfo )
		return 0xff;	// 퀘스트가 없다

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;

	int NeedZen = 0;

	for( int subquest = 0; subquest < subquestcount; subquest++ )
	{
		lpSubInfo = GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if( lpSubInfo )
		{
			for( int condition = 0; condition < concount; condition++ )
			{	
				lpCondition = GetQuestCondition(lpQuestInfo, condition);
#ifdef MODIFY_QUEST_SYSTEM_20070525	// 퀘스트 시스템 버그수정	// 여기 수정해야함
				if( lpCondition && lpCondition->Index == -1 )
				{	// 모든 조건에 해당되는 요구정보 처리
					NeedZen += lpCondition->NeedZen;					
				}
				else if( lpCondition && lpCondition->Index == lpSubInfo->LinkConditionIndex )
				{	// 연결 되는 요구조건 처리
					NeedZen += lpCondition->NeedZen;
				}						
#else
				NeedZen += lpCondition->NeedZen;	
#endif
			}
#ifdef MODIFY_QUEST_SYSTEM_20070525	// 퀘스트 완료조건이 몬스터킬카운터 - 유저정보 설정
			if( lpSubInfo->QuestType == QUEST_TYPE_MONSTER_KILLCOUNT )
			{
				if( lpObj->m_iQuestMonKillDBIndex == -1 )
					lpObj->m_iQuestMonKillDBIndex = QuestIndex;

				for( int i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
				{
					if( lpObj->m_QuestMonKillInfo[i].iQuestMonIndex == -1 )
					{
						lpObj->m_QuestMonKillInfo[i].iQuestMonIndex	= lpSubInfo->NeedType;
						lpObj->m_QuestMonKillInfo[i].iKillCount		= 0;
						break;
					}
				}
				LogAddTD( "[Quest] Quest Accept - MonsterKillCount Set (%s) : [%s][%s]", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name );
			}
#endif			
		}		
	}

	lpObj->Money -= NeedZen;
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	return TRUE;
}

BOOL CQuestInfo::QuestClear(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo;
	lpQuestInfo = GetQuestInfo(QuestIndex);

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[18] != AUTH_CHECK_18 )
		DestroyGIocp();
#endif

	if( !lpQuestInfo )
		return 0xff;	// 퀘스트가 없다

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;

	for( int subquest = 0; subquest < subquestcount; subquest++ )
	{
		lpSubInfo = GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if( lpSubInfo )
		{
			if( lpSubInfo->RewardType == QRT_CHANGEUP )
			{	// 전직
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				
				GCSendQuestPrize(lpObj->m_Index, QRT_STATUP, lpSubInfo->RewardCount);		

				// 전직 상태 처리를 한다				
				lpObj->ChangeUP = 1;
				lpObj->DbClass |= 0x01;

				// 바뀐 정보를 보여준다
				gObjMakePreviewCharSet(lpObj->m_Index);	
				
				BYTE Class = ((lpObj->Class<<5)&0xE0);
				Class |= (lpObj->ChangeUP<<4)&0x10;
				GCSendQuestPrize(lpObj->m_Index, QRT_CHANGEUP, Class);

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), ChangeUp", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->LevelUpPoint, lpSubInfo->RewardCount);
			}
			else if( lpSubInfo->RewardType == QRT_STATUP )
			{	// 스탯포인트 상승
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QRT_STATUP, lpSubInfo->RewardCount);

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d)", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->LevelUpPoint, lpSubInfo->RewardCount);
			}
#ifdef SECOND_QUEST
			else if( lpSubInfo->RewardType == QRT_PLUSSTAT )
			{	// 스탯포인트 상승
				int addleveluppoint = lpObj->Level - 220;
				if( addleveluppoint <= 0 )
				{
					addleveluppoint = 0;
				}

				lpObj->LevelUpPoint += addleveluppoint;
				lpObj->PlusStatQuestClear = 1;

				GCSendQuestPrize(lpObj->m_Index, QRT_PLUSSTAT, addleveluppoint);

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), PlusStat", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->LevelUpPoint, addleveluppoint);
			}
			else if( lpSubInfo->RewardType == QRT_USINGCOMBOSKILL )
			{	// 콤보 스킬 사용				
				GCSendQuestPrize(lpObj->m_Index, QRT_USINGCOMBOSKILL, 0);	
				lpObj->ComboSkillquestClear = 1;

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] ComboSkill", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name);
			}
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3차 전직을 했다.
			else if( lpSubInfo->RewardType == QRT_THIRD_CHANGEUP )
			{
				// 전직 상태 처리를 한다
				lpObj->ThirdChangeUp = 1;
				lpObj->DbClass |= 0x02;

				// 바뀐 정보를 보여준다
				gObjMakePreviewCharSet( lpObj->m_Index );	
				
				BYTE Class = ( ( lpObj->Class << 5 ) & 0xE0 );
				Class |= (( lpObj->ChangeUP << 4 ) & 0x10 );	
				Class |= (( lpObj->ThirdChangeUp << 3 ) & 0x08 );
					 
				GCSendQuestPrize( lpObj->m_Index, QRT_THIRD_CHANGEUP, Class );

#ifdef MODIFY_LOG_THIRD_CHANGEUP_20070801	// 전직 번호 수정
				LogAddTD( "[Quest] Quest Clear (%s) : [%s][%s] Class:%d(%d), 3rd ChangeUp", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->DbClass, Class );
#else
				LogAddTD( "[Quest] Quest Clear (%s) : [%s][%s] Class:%d, 3rd ChangeUp", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   Class );
#endif
			}
#endif
			else if( lpSubInfo->RewardType >= 0 && lpSubInfo->RewardType < QRT_STATUP)
			{	// 아이템
			}

			if( lpSubInfo->QuestType == 1 )
			{
				gObjDelteItemCountInInventory(lpObj->m_Index, lpSubInfo->NeedType, 
											  lpSubInfo->NeedSubType, lpSubInfo->NeedNumber);
			}
			
#ifdef MODIFY_QUEST_SYSTEM_20070525	// 몬스터 킬카운트 퀘스트 완료 - 데이터 초기화
			if( lpSubInfo->QuestType == QUEST_TYPE_MONSTER_KILLCOUNT )
			{
				lpObj->m_iQuestMonKillDBIndex	= -1;
				for( int i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
				{
					lpObj->m_QuestMonKillInfo[i].iQuestMonIndex =  -1;
					lpObj->m_QuestMonKillInfo[i].iKillCount		=  -1;
				}
				
				LogAddTD( "[Quest] Quest Clear - MonsterKillCount Reset (%s) : [%s][%s]", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name );
			}
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 퀘스트 스크립트 RewardSub Type 처리
			if( lpSubInfo->RewardSubType == QRT_STATUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize( lpObj->m_Index, QRT_STATUP, lpSubInfo->RewardCount );	

#ifdef MODIFY_LOG_THIRD_CHANGEUP_20070801
				LogAddTD( "[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), Class:%d PlusStat", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->LevelUpPoint, lpSubInfo->RewardCount, lpObj->DbClass );
#else
				LogAddTD( "[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), Class:%d PlusStat", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->LevelUpPoint, lpSubInfo->RewardCount, lpObj->Class );
#endif
			}
#endif
		}
	}

	return TRUE;
}


char sQuestString[4][50] = 
{
	"None", "Accept", "Clear", "None"
};

void CQuestInfo::QuestInfoSave(LPOBJECTSTRUCT lpObj)
{
	int questcount = GetQeustCount();
	int	foundquest = 0;

	LPQUEST_INFO lpQuestInfo;	

	for( int i = 0; i < MAX_QUEST; i++ )
	{
		lpQuestInfo = GetQuestInfo(i);
		if( !lpQuestInfo )			
			continue;

		LogAddTD("[Quest] QuestSave [%s][%s] index(%d) name(%s) state(%s)", 
			     lpObj->AccountID, lpObj->Name, 
				 i, lpQuestInfo->Name, sQuestString[GetQuestState(lpObj, i)]);
		
		foundquest++;
		if( foundquest == questcount )	
			break;
	}
}

#ifdef MODIFY_QUEST_SYSTEM_20070525	// CheckQuestMonsterKill 함수 처리
BOOL CQuestInfo::CheckQuestMonsterKill( LPOBJECTSTRUCT lpMonsterObj, LPOBJECTSTRUCT lpObj )
{	
	if( lpObj->Connected < 3 && lpObj->Live )
		return FALSE;

	// 개인인지 파티원인지 확인
	int iPartyNum = lpObj->PartyNumber;
	if( iPartyNum < 0 )
	{
		// 개인 플레이
		if( lpObj->m_iQuestMonKillDBIndex == -1 )
			return FALSE;

		for( int i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
		{				
			if( lpMonsterObj->Class == lpObj->m_QuestMonKillInfo[i].iQuestMonIndex )
			{
				if( lpObj->m_QuestMonKillInfo[i].iKillCount <= MAX_QUEST_MONSTER_KILL_COUNT )
				{
					lpObj->m_QuestMonKillInfo[i].iKillCount++;	
					break;
				}
			}
		}
	}
	else
	{
		// 파티 플레이
		int iPartyNum = lpObj->PartyNumber;
		int iPartyUserIndex;
		LPOBJECTSTRUCT lpPartyObj;
		int iDis = 0;

		for( int iPatyUserCount = 0; iPatyUserCount < MAX_PARTYUSER; iPatyUserCount++ )
		{
			iPartyUserIndex = gParty.m_PartyS[iPartyNum].Number[iPatyUserCount];
			if( iPartyUserIndex < 0 )
				continue;

			lpPartyObj = ( LPOBJECTSTRUCT )&gObj[iPartyUserIndex];	
			if( lpPartyObj->Connected < 3 && lpPartyObj->Live )
				continue;

			if( lpObj->MapNumber != lpPartyObj->MapNumber ) 
				continue;
			
			iDis = gObjCalDistance( lpObj, lpPartyObj );
			if( iDis > 25  )				
				continue;

			if( lpPartyObj->m_iQuestMonKillDBIndex == -1 )
				continue;

			for( int i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
			{				
				if( lpMonsterObj->Class == lpPartyObj->m_QuestMonKillInfo[i].iQuestMonIndex )
				{
					if( lpPartyObj->m_QuestMonKillInfo[i].iKillCount <= MAX_QUEST_MONSTER_KILL_COUNT )
					{
						lpPartyObj->m_QuestMonKillInfo[i].iKillCount++;							
						break;
					}
				}				
			}
			
		}
	}

	return TRUE;
}

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
BOOL CQuestInfo::QuestMonsterKillInfoSend( int QuestIndex, int iIndex )
{
	PMSG_ANS_QUEST_MONKILL_INFO pResult;
	PHeadSubSetB( ( LPBYTE )&pResult, 0xA4, 0x00, sizeof( pResult ) );
	pResult.btResult		= 0x00;
	pResult.btQuestIndex	= QuestIndex;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	// 진행중인 퀘스트인지 확인하다.
	int iCurrQuestState = GetQuestState( lpObj, QuestIndex );
	if( iCurrQuestState != QS_ACCEPT )
		return TRUE;		// 특별히 정보를 보내지는 않는다.
		
	// 퀘스트를 체크한다.
	LPQUEST_INFO lpQuestInfo;
	lpQuestInfo = GetQuestInfo( QuestIndex );

	if( !lpQuestInfo )
		return TRUE;

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;

	BOOL	bFoundSubQuest = FALSE;

	for( int subquest = 0; subquest < subquestcount; subquest++ )
	{
		lpSubInfo = GetSubquestInfo( lpObj, lpQuestInfo, subquest );

		if( lpSubInfo )
		{
			bFoundSubQuest = TRUE;
			if( lpSubInfo->QuestType == 2 )	// 완료 조건이 몬스터 킬카운트
			{			
				if( lpObj->m_bQuestMonKillDBLoad == FALSE )
				{
					// 퀘스트 몬스터 킬 정보를 아직 받지 못했다면 정보를 요청한다.
					g_QuestUtil.GDReqQuestMonsterKillInfo( lpObj );
					return FALSE;
				}
				else if( lpObj->m_bQuestMonKillDBLoad == TRUE && lpObj->m_iQuestMonKillDBIndex == -1 )
				{
					// 정보를 재요청한다. 잘 될 때까지. ->임의로 접속을 끊는 방법도 생각할 수 있음.
					lpObj->m_bQuestMonKillDBLoad = FALSE;
					g_QuestUtil.GDReqQuestMonsterKillInfo( lpObj );

					LogAddTD( "[Quest] Error - Invalid MonsterKillInfo [%s][%s] (QuestIndex:%d/DBIndex:%d)",
							lpObj->AccountID,
							lpObj->Name,
							QuestIndex,
							lpObj->m_iQuestMonKillDBIndex 
							);
					return FALSE;
				}
				else
				{
					pResult.btResult = 0x01;	// 성공! 데이터를 보낸다.				

					// 정보는 그대로를 보내준다.
					pResult.btQuestIndex = lpObj->m_iQuestMonKillDBIndex;

					int iSendCount = 0;
					for( int i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
					{						
						pResult.iKillCountInfo[iSendCount] = lpObj->m_QuestMonKillInfo[i].iQuestMonIndex;
						++iSendCount;
						pResult.iKillCountInfo[iSendCount] = lpObj->m_QuestMonKillInfo[i].iKillCount;
						++iSendCount;
					}				
					break;
				}
			}
		}
	}

	if( pResult.btResult == 0x01 )
	{
		// 보낼 데이터가 있을 때에만 보낸다.
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
		// 로그
	}	

	return TRUE;
}
#endif	// MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
#endif	// MODIFY_QUEST_SYSTEM_20070525

#ifdef THIRD_CHANGEUP_SYSTEM_20070507
void CQuestInfo::CheckQuestMapEnterOnWerwolf( int iIndex )
{	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	if( lpObj == NULL )
		return;

	PMSG_ANS_ENTER_ON_WERWOLF pResult;
	PHeadSubSetB( ( LPBYTE )&pResult, 0xD0, 0x07, sizeof( pResult ) );
	pResult.btResult = 0;	

	int iNeedZen	= 3000000;	// 300만젠
	int iNeedLevel	= 350;		// 350레벨

	if( lpObj->Level < 350 || lpObj->Money < iNeedZen )
	{
		// 못들어간다.
		pResult.btResult = 0;
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
		return;
	}
	
	// 발가스의 병영 퀘스트 수행중이거나 완료한 유저는 출입 가능	
	int iQuestState1 = GetQuestState( lpObj, 5 );		
	if( iQuestState1 == QS_ACCEPT || iQuestState1 == QS_CLEAR )
	{
		// 유저와 유저가 속한 파티원을 '발가스의 병영'으로 이동
		LPOBJECTSTRUCT lpPartyObj;
		int iPartyNum = lpObj->PartyNumber;
		int iPartyUserIndex;
		int	iDis;
		BOOL bSuccess = FALSE;		

		if( iPartyNum >= 0 )
		{
			// 파티인경우 '나'는 가장 늦게 들어간다. (맵 체크때문)
			for( int n = 0; n < MAX_PARTYUSER; n++ )
			{
				iPartyUserIndex = gParty.m_PartyS[iPartyNum].Number[n];
				if( iPartyUserIndex < 0 )
					continue;
				
				if( iIndex == iPartyUserIndex )	// '나'는 빼고.
					continue;

				lpPartyObj = ( LPOBJECTSTRUCT )&gObj[iPartyUserIndex];		
				
				// 입장레벨 확인
				if( lpPartyObj->Level < iNeedLevel )
					continue;

				// 같은 맵에 존재하고
				if( lpObj->MapNumber == lpPartyObj->MapNumber )
				{
					iDis = gObjCalDistance( lpObj, lpPartyObj );
					if( iDis < 10 )		// 거리계산
					{
						bSuccess = gObjMoveGate( iPartyUserIndex, GATE_BARRACKS_OF_BALGAS );
						if( !bSuccess )
						{
							pResult.btResult = 0;
							DataSend( iPartyUserIndex, ( LPBYTE )&pResult, pResult.h.size );
						}							
					}
				}
			}
		}

		// 개인인 경우 또는 파티원 중 '나'
		bSuccess = gObjMoveGate( iIndex, GATE_BARRACKS_OF_BALGAS );
		if( bSuccess )
		{
			lpObj->Money -= iNeedZen;							
			GCMoneySend( lpObj->m_Index, lpObj->Money );
		}
		else
		{			
			pResult.btResult = 0;
			DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
		}
	}
}

void CQuestInfo::CheckQuestMapEnterOnGateKeeper( int iIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	if( lpObj == NULL )
		return;

	PMSG_ANS_ENTER_ON_GATEKEEPER pResult;
	PHeadSubSetB( ( LPBYTE )&pResult, 0xD0, 0x08, sizeof( pResult ) );
	pResult.btResult = 0;
	
	// 레벨 확인
	int iNeedLevel	= 350;	// 350레벨
	if( lpObj->Level < iNeedLevel )
	{
		pResult.btResult = 0;
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );	
		return;
	}

	// '어둠속으로' 퀘스트 수행중인 유저에게 출입 허가
	if( lpObj->m_iQuestMonKillDBIndex != 6 )	
	{
		pResult.btResult = 0;
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );	
		return;
	}

	// 유저와 유저가 속한 파티원을 '발가스의 안식처'로 이동
	LPOBJECTSTRUCT lpPartyObj;
	int iPartyNum = lpObj->PartyNumber;
	int iPartyUserIndex;
	int	iDis;	
	BOOL bSuccess = FALSE;

	if( iPartyNum >= 0 )
	{
		// 파티인경우 '나'는 가장 늦게 들어간다. (맵 체크때문)
		for( int n = 0; n < MAX_PARTYUSER; n++ )
		{
			iPartyUserIndex = gParty.m_PartyS[iPartyNum].Number[n];
			if( iPartyUserIndex < 0 )
				continue;

			if( iIndex == iPartyUserIndex )	// '나'는 제일 마지막에 이동
				continue;

			lpPartyObj = ( LPOBJECTSTRUCT )&gObj[iPartyUserIndex];	
			
			// 입장레벨 확인
			if( lpPartyObj->Level < iNeedLevel )
				continue;
			
			// 같은 맵에 존재하고
			if( lpObj->MapNumber == lpPartyObj->MapNumber ) 
			{
				iDis = gObjCalDistance( lpObj, lpPartyObj );
				if( iDis < 10 )		// 거리계산
				{
					bSuccess = gObjMoveGate( iPartyUserIndex, GATE_RESTINGPLACE_OF_BALGAS );
					if( !bSuccess )
					{
						pResult.btResult = 0;
						DataSend( iPartyUserIndex, ( LPBYTE )&pResult, pResult.h.size );
						return;						
					}
				}
			}
		}
	}

	// 개인인 경우 또는 파티원 중 '나'
	bSuccess = gObjMoveGate( iIndex, GATE_RESTINGPLACE_OF_BALGAS );
	if( !bSuccess )
	{	
		pResult.btResult = 0;
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
		return;
	}
}

void CQuestInfo::QuestSuccessCommand( int iIndex, int iQuestIndex )
{
	// 테스트모드에서만 사용한다.
#if ( TESTSERVER == 0 )
	return;
#endif

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	int iState		= QS_CLEAR;	// 클리어로 설정한다.

	int index = iQuestIndex / 4;
	int shift = ( iQuestIndex % 4 ) * 2;			
	( lpObj->m_Quest[index] ) &= QuestBitMask[shift];
	( lpObj->m_Quest[index] ) |= ( ( iState & 0x3 ) << shift );				

	lpObj->m_iQuestMonKillDBIndex = -1;
	for( int i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
	{
		lpObj->m_QuestMonKillInfo[i].iQuestMonIndex = -1;
		lpObj->m_QuestMonKillInfo[i].iKillCount = -1;
	}

	GCServerMsgStringSend( "퀘스트 성공", lpObj->m_Index, 1 );
}
#endif	// THIRD_CHANGEUP_SYSTEM_20070507

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// 퀘스트 아이템을 습득할 수 있는지 확인
BOOL CQuestInfo::GetQuestItem( int iIndex, short nType, short nLevel )
{
	if( gObj[iIndex].m_iQuestMonKillDBIndex != -1 )
	{
		// 아이템을 얻는 퀘스트가 아니다.
		return FALSE;
	}
	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	LPQUEST_INFO		lpQuestInfo;
	LPQUEST_SUB_INFO	lpSubInfo;
	int iSubquestcount;
	int iQuestState = QS_NOTHING;
	
	for( int iQuestIndex = 0; iQuestIndex < m_QuestCount; iQuestIndex++ )
	{
		iQuestState = GetQuestState( lpObj, iQuestIndex );
		if( iQuestState == QS_ACCEPT )
		{
			lpQuestInfo = GetQuestInfo( iQuestIndex );
			if( !lpQuestInfo )
				return FALSE;
			
			iSubquestcount = lpQuestInfo->QuestSubInfoCount;
			for( int subquest = 0; subquest < iSubquestcount; subquest++ )
			{
				lpSubInfo = GetSubquestInfo( lpObj, lpQuestInfo, subquest );
				if( !lpSubInfo )
					continue;
			
				// 아이템 습득 퀘스트가 아니라면 패스.
				if( lpSubInfo->QuestType != 1 )
					return FALSE;

				// 수행중인 퀘스트의 아이템인가
				if( nType == MAKE_ITEMNUM( lpSubInfo->NeedType, lpSubInfo->NeedSubType ) 
						&& nLevel == lpSubInfo->ItemLevel )
				{
					// 수량 확인
					int iItemCount = gObjGetItemCountInIventory( iIndex, nType );
					if( iItemCount >= lpSubInfo->NeedNumber )
					{
						LogAdd( "[Quest] Too many has quest items [%s][%s] (%d/%d)", 
								lpObj->AccountID, lpObj->Name, nType, iItemCount );
						return FALSE;
					}
					else
						return TRUE;
				}				
			}
			
			// 현재 퀘스트와 맞지 않는다.
			return FALSE;
		}
	}
	return FALSE;
}
	
#endif	// MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704

#endif