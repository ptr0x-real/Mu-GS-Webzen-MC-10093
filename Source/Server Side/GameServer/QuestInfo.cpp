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
			{	// ����Ʈ ����
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// quset ��� Ŭ���� �߰�
					Token = (*GetToken)();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[5] = (int)TokenNumber;
#endif
					
					QuestInfo[QuestIndex].QuestSubInfoCount++;
				}

				LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d", 
					   QuestInfo[QuestIndex].Name, QuestIndex, QuestInfo[QuestIndex].QuestSubInfoCount);

				m_QuestCount++;					   
			}
			else if( Type == 1 )
			{	// ����Ʈ �䱸����
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
			{	// ����Ʈ ����
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// quset ��� Ŭ���� �߰�
					Token = WzMemScript.GetToken();	QuestInfo[QuestIndex].QuestSubInfo[index].RequireClass[5] = WzMemScript.GetNumber();
#endif

					QuestInfo[QuestIndex].QuestSubInfoCount++;
				}

				LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d", 
					   QuestInfo[QuestIndex].Name, QuestIndex, QuestInfo[QuestIndex].QuestSubInfoCount);

				m_QuestCount++;					   
			}
			else if( Type == 1 )
			{	// ����Ʈ �䱸����
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
			{	// ����Ʈ ����
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
			{	// ����Ʈ �䱸����
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
				{	// ����Ʈ Ÿ���� Item����϶���
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
		return 0xff;		// ����Ʈ�� ����

	int QuestState = GetQuestState(lpObj, QuestIndex);

	if( QuestState == QS_NOTHING || QuestState == QS_NONE )
	{	// ����Ʈ�� �����ҷ��� �Ѵٸ�, ����Ʈ ���� ���� ������ �˻��Ѵ�
		BYTE result = QuestRunConditionCheck(lpObj, QuestIndex);
		if(  result != 0 )
		{	// ���ǿ� ���� �ʴ�
			return result;
		}			

		QuestAccept(lpObj, QuestIndex);

		LogAddTD("[Quest] AcceptQuest [%s][%s] (%s)", 
			     lpObj->AccountID, lpObj->Name, GetQuestInfo(QuestIndex)->Name);


		State = QS_ACCEPT;
	}
	else if( QuestState == QS_ACCEPT )
	{	// Ŭ���� �ҷ��� �Ѵ�
		BYTE result = QuestClearConditionCheck(lpObj, QuestIndex);
		if(  result != 0 )
		{	// ���ǿ� ���� �ʴ�
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
		return NULL;	// ����Ʈ�� ����

	if( subquestindex < 0 || subquestindex > MAX_QUEST_SUBINFO )
		return NULL;

	LPQUEST_SUB_INFO lpSubInfo = &lpQuestInfo->QuestSubInfo[subquestindex];

	int Class = lpObj->Class;
	int ChangeUP = lpObj->ChangeUP;	

	int requireclass = lpSubInfo->RequireClass[Class];
	if( requireclass == 0 ) return NULL;
	if( requireclass > 1 )
	{	// ���������̸�
		if( requireclass != ChangeUP+1 )
			return NULL;
	}
	return lpSubInfo;
}

LPQUEST_CONDITION CQuestInfo::GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex)
{	
	if( !lpQuestInfo )
		return NULL;	// ����Ʈ�� ����

	if( conditionindex < 0 || conditionindex > MAX_QUEST_CONDITION )
		return NULL;

	return &lpQuestInfo->QuestCondition[conditionindex];
}

BYTE CQuestInfo::QuestClearConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo;
	lpQuestInfo = GetQuestInfo(QuestIndex);

	if( !lpQuestInfo )
		return 0xff;	// ����Ʈ�� ����

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
				{	// ������ �������� �ʾҴ�.
					return 1;
				}
			}
		}		
	}

	if( !bFoundSubQuest )
		return 0xff;

	return FALSE;
}

// ���� �˻�
BYTE CQuestInfo::QuestRunConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo;
	lpQuestInfo = GetQuestInfo(QuestIndex);

	if( !lpQuestInfo )
		return 0xff;	// ����Ʈ�� ����

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
				{	// ���� ��� ���ǿ� �ش�Ǵ� ���� �˻��Ѵ�
					if( !CompareCondition(lpObj, lpCondition) )
					{
						return lpCondition->StartContext;
					}
				}
				else if( lpCondition && lpCondition->Index == lpSubInfo->LinkConditionIndex )
				{	// ���� �Ǵ� �䱸������ �˻��Ѵ�
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
	{	// ������ Ŭ������ ����Ʈ�� Ŭ���� ���� �ʾҴ�
		if( GetQuestState(lpObj, lpCondition->NeedQuestIndex) != QS_CLEAR )
			return FALSE;
	}

	if( lpCondition->MinLevel != 0 )
	{
		if( lpCondition->MinLevel > lpObj->Level )
		{	// �ּ� ������ ���� �ʴ´�
			return FALSE;
		}
	}

	if( lpCondition->MaxLevel != 0 )
	{
		if( lpCondition->MaxLevel < lpObj->Level )
		{	// �ִ� ������ ���� �ʴ´�
			return FALSE;
		}
	}

	if( lpCondition->NeedZen > lpObj->Money )
	{	// ���� �����ϴ�
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
		{	// ����Ʈ �ߵ� Ÿ���� NPCŬ���̸�
			if( lpNpc->Class == lpQuestInfo->QuestStartSubType )
			{	// ����Ʈ NPC��
				if( queststate == -1 )
				{	// ������ ����Ʈ�� ������ ����Ʈ�� �����Ѵ�
					questindex = lpQuestInfo->QuestIndex;
					queststate = GetQuestState(lpObj, lpQuestInfo->QuestIndex);
				}
				else if( queststate == QS_CLEAR )
				{	// �ߺ����� üũ�� ����Ʈ�� �ִµ� 
					// ������ üũ�� ����Ʈ�� Ŭ���� �Ǿ� �ִ� ���¸�, ���� ����Ʈ�� �����Ų��
#ifdef SECOND_QUEST
					for( int j = 0; j < lpQuestInfo->QuestSubInfoCount; j++ )
					{
						LPQUEST_SUB_INFO lpSubInfo = GetSubquestInfo(lpObj, lpQuestInfo, j);
						if( lpSubInfo )
						{	// ���������� �ִ� ����Ʈ�� �����Ѵ�
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
		// ���� ų�� �Ϸ������� ����Ʈ�� ųī��Ʈ ������ ���� ������.
#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
		BOOL bSendResult = QuestMonsterKillInfoSend( questindex, lpObj->m_Index );
		if( bSendResult == FALSE )
		{
			// ���� ųī��Ʈ �ٽ� ������ �� -> NPC�� ����Ʈâ�� ���� ���ϰ� �Ѵ�.		
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
	// �̱��� ���� ������ ���� ����
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

	if( MaxHitUser == -1 )
		return FALSE;

	int partycount = gParty.GetPartyCount(gObj[MaxHitUser].PartyNumber);
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ����Ʈ�� �����ϴ� ����� �������� ��Ƽ�������� ���� ó���� �޶���
	if( partycount > 0 )
	{
		BOOL bResult = MonsterItemDropForParty( lpObj, &gObj[MaxHitUser] );
		return bResult;
	}		
#else
	if( partycount > 0 )
		return FALSE;	// ��Ƽ�� �� ���¿����� �������� ������ ����
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
				{	// ����Ʈ Ÿ���� Item����϶���
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// NeedTargetMinLevel�� -1�̸� 
										// NeedTargetMaxLevel�� ���� �ε����� Ư�� ���͸� ������ �� �ִ�.
					if( ( lpSubInfo->NeedTargetMinLevel > 0 
							&& lpObj->Level >= lpSubInfo->NeedTargetMinLevel 
							&& lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
						|| ( lpSubInfo->NeedTargetMinLevel == -1 && lpObj->Class == lpSubInfo->NeedTargetMaxLevel ) )
#else
					if( lpObj->Level >= lpSubInfo->NeedTargetMinLevel && 
						lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
#endif
					{	// ����Ʈ ��� ���Ͱ� �������� ����Ʈ ��� ���Ͱ� ������
						if( GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == QS_ACCEPT )
						{	// �ش� ����Ʈ ���°� ���� ���϶���
							// ���� ����Ʈ �������� ��� �ִ��� Ȯ���Ѵ�
							if( rand()%1000 < lpSubInfo->NeedDropRate )
							{
#ifdef SECOND_QUEST
								int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType, lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);
#else
								int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType, lpSubInfo->NeedSubType);
#endif

								if( itemcount >= lpSubInfo->NeedNumber )
								{	// �̹� �ʿ��� ��ŭ ������ �ִٸ�
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

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ����Ʈ�� ��Ƽ�� �ξ� ������ ��� ó��
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

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// ����Ʈ ������ ��Ƽ����� ��Ƽ������ �ʵ� üũ�Ѵ�.
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

				if( lpSubInfo->QuestType == 1  )	// ����Ʈ Ÿ���� Item ���
				{
					// Ư�� ���͸� ��� ����Ʈ
					if( ( lpSubInfo->NeedTargetMinLevel > 0 
							&& lpObj->Level >= lpSubInfo->NeedTargetMinLevel 
							&& lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
						|| ( lpSubInfo->NeedTargetMinLevel == -1 && lpObj->Class == lpSubInfo->NeedTargetMaxLevel ) )					
					{
						if( GetQuestState( lpPartyObj, lpQuestInfo->QuestIndex ) == QS_ACCEPT )
						{	// �ش� ����Ʈ ���°� ���� ���϶�, �������� ���� ����߸���.
							if( rand() % 1000 < lpSubInfo->NeedDropRate )
							{									
								dur		= 0;
								x		= lpMaxHitObj->X;
								y		= lpMaxHitObj->Y;
								level	= lpSubInfo->ItemLevel;										
								type	= ItemGetNumberMake( lpSubInfo->NeedType, lpSubInfo->NeedSubType );

								// ������ ������ �ִ뵥���� ���� ��������.
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
		return 0xff;	// ����Ʈ�� ����

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
#ifdef MODIFY_QUEST_SYSTEM_20070525	// ����Ʈ �ý��� ���׼���	// ���� �����ؾ���
				if( lpCondition && lpCondition->Index == -1 )
				{	// ��� ���ǿ� �ش�Ǵ� �䱸���� ó��
					NeedZen += lpCondition->NeedZen;					
				}
				else if( lpCondition && lpCondition->Index == lpSubInfo->LinkConditionIndex )
				{	// ���� �Ǵ� �䱸���� ó��
					NeedZen += lpCondition->NeedZen;
				}						
#else
				NeedZen += lpCondition->NeedZen;	
#endif
			}
#ifdef MODIFY_QUEST_SYSTEM_20070525	// ����Ʈ �Ϸ������� ����ųī���� - �������� ����
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
		return 0xff;	// ����Ʈ�� ����

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;

	for( int subquest = 0; subquest < subquestcount; subquest++ )
	{
		lpSubInfo = GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if( lpSubInfo )
		{
			if( lpSubInfo->RewardType == QRT_CHANGEUP )
			{	// ����
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				
				GCSendQuestPrize(lpObj->m_Index, QRT_STATUP, lpSubInfo->RewardCount);		

				// ���� ���� ó���� �Ѵ�				
				lpObj->ChangeUP = 1;
				lpObj->DbClass |= 0x01;

				// �ٲ� ������ �����ش�
				gObjMakePreviewCharSet(lpObj->m_Index);	
				
				BYTE Class = ((lpObj->Class<<5)&0xE0);
				Class |= (lpObj->ChangeUP<<4)&0x10;
				GCSendQuestPrize(lpObj->m_Index, QRT_CHANGEUP, Class);

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), ChangeUp", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->LevelUpPoint, lpSubInfo->RewardCount);
			}
			else if( lpSubInfo->RewardType == QRT_STATUP )
			{	// ��������Ʈ ���
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QRT_STATUP, lpSubInfo->RewardCount);

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d)", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					   lpObj->LevelUpPoint, lpSubInfo->RewardCount);
			}
#ifdef SECOND_QUEST
			else if( lpSubInfo->RewardType == QRT_PLUSSTAT )
			{	// ��������Ʈ ���
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
			{	// �޺� ��ų ���				
				GCSendQuestPrize(lpObj->m_Index, QRT_USINGCOMBOSKILL, 0);	
				lpObj->ComboSkillquestClear = 1;

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] ComboSkill", 
					   lpQuestInfo->Name, lpObj->AccountID, lpObj->Name);
			}
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ������ �ߴ�.
			else if( lpSubInfo->RewardType == QRT_THIRD_CHANGEUP )
			{
				// ���� ���� ó���� �Ѵ�
				lpObj->ThirdChangeUp = 1;
				lpObj->DbClass |= 0x02;

				// �ٲ� ������ �����ش�
				gObjMakePreviewCharSet( lpObj->m_Index );	
				
				BYTE Class = ( ( lpObj->Class << 5 ) & 0xE0 );
				Class |= (( lpObj->ChangeUP << 4 ) & 0x10 );	
				Class |= (( lpObj->ThirdChangeUp << 3 ) & 0x08 );
					 
				GCSendQuestPrize( lpObj->m_Index, QRT_THIRD_CHANGEUP, Class );

#ifdef MODIFY_LOG_THIRD_CHANGEUP_20070801	// ���� ��ȣ ����
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
			{	// ������
			}

			if( lpSubInfo->QuestType == 1 )
			{
				gObjDelteItemCountInInventory(lpObj->m_Index, lpSubInfo->NeedType, 
											  lpSubInfo->NeedSubType, lpSubInfo->NeedNumber);
			}
			
#ifdef MODIFY_QUEST_SYSTEM_20070525	// ���� ųī��Ʈ ����Ʈ �Ϸ� - ������ �ʱ�ȭ
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

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ����Ʈ ��ũ��Ʈ RewardSub Type ó��
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

#ifdef MODIFY_QUEST_SYSTEM_20070525	// CheckQuestMonsterKill �Լ� ó��
BOOL CQuestInfo::CheckQuestMonsterKill( LPOBJECTSTRUCT lpMonsterObj, LPOBJECTSTRUCT lpObj )
{	
	if( lpObj->Connected < 3 && lpObj->Live )
		return FALSE;

	// �������� ��Ƽ������ Ȯ��
	int iPartyNum = lpObj->PartyNumber;
	if( iPartyNum < 0 )
	{
		// ���� �÷���
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
		// ��Ƽ �÷���
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

	// �������� ����Ʈ���� Ȯ���ϴ�.
	int iCurrQuestState = GetQuestState( lpObj, QuestIndex );
	if( iCurrQuestState != QS_ACCEPT )
		return TRUE;		// Ư���� ������ �������� �ʴ´�.
		
	// ����Ʈ�� üũ�Ѵ�.
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
			if( lpSubInfo->QuestType == 2 )	// �Ϸ� ������ ���� ųī��Ʈ
			{			
				if( lpObj->m_bQuestMonKillDBLoad == FALSE )
				{
					// ����Ʈ ���� ų ������ ���� ���� ���ߴٸ� ������ ��û�Ѵ�.
					g_QuestUtil.GDReqQuestMonsterKillInfo( lpObj );
					return FALSE;
				}
				else if( lpObj->m_bQuestMonKillDBLoad == TRUE && lpObj->m_iQuestMonKillDBIndex == -1 )
				{
					// ������ ���û�Ѵ�. �� �� ������. ->���Ƿ� ������ ���� ����� ������ �� ����.
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
					pResult.btResult = 0x01;	// ����! �����͸� ������.				

					// ������ �״�θ� �����ش�.
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
		// ���� �����Ͱ� ���� ������ ������.
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
		// �α�
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

	int iNeedZen	= 3000000;	// 300����
	int iNeedLevel	= 350;		// 350����

	if( lpObj->Level < 350 || lpObj->Money < iNeedZen )
	{
		// ������.
		pResult.btResult = 0;
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
		return;
	}
	
	// �߰����� ���� ����Ʈ �������̰ų� �Ϸ��� ������ ���� ����	
	int iQuestState1 = GetQuestState( lpObj, 5 );		
	if( iQuestState1 == QS_ACCEPT || iQuestState1 == QS_CLEAR )
	{
		// ������ ������ ���� ��Ƽ���� '�߰����� ����'���� �̵�
		LPOBJECTSTRUCT lpPartyObj;
		int iPartyNum = lpObj->PartyNumber;
		int iPartyUserIndex;
		int	iDis;
		BOOL bSuccess = FALSE;		

		if( iPartyNum >= 0 )
		{
			// ��Ƽ�ΰ�� '��'�� ���� �ʰ� ����. (�� üũ����)
			for( int n = 0; n < MAX_PARTYUSER; n++ )
			{
				iPartyUserIndex = gParty.m_PartyS[iPartyNum].Number[n];
				if( iPartyUserIndex < 0 )
					continue;
				
				if( iIndex == iPartyUserIndex )	// '��'�� ����.
					continue;

				lpPartyObj = ( LPOBJECTSTRUCT )&gObj[iPartyUserIndex];		
				
				// ���巹�� Ȯ��
				if( lpPartyObj->Level < iNeedLevel )
					continue;

				// ���� �ʿ� �����ϰ�
				if( lpObj->MapNumber == lpPartyObj->MapNumber )
				{
					iDis = gObjCalDistance( lpObj, lpPartyObj );
					if( iDis < 10 )		// �Ÿ����
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

		// ������ ��� �Ǵ� ��Ƽ�� �� '��'
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
	
	// ���� Ȯ��
	int iNeedLevel	= 350;	// 350����
	if( lpObj->Level < iNeedLevel )
	{
		pResult.btResult = 0;
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );	
		return;
	}

	// '��Ҽ�����' ����Ʈ �������� �������� ���� �㰡
	if( lpObj->m_iQuestMonKillDBIndex != 6 )	
	{
		pResult.btResult = 0;
		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );	
		return;
	}

	// ������ ������ ���� ��Ƽ���� '�߰����� �Ƚ�ó'�� �̵�
	LPOBJECTSTRUCT lpPartyObj;
	int iPartyNum = lpObj->PartyNumber;
	int iPartyUserIndex;
	int	iDis;	
	BOOL bSuccess = FALSE;

	if( iPartyNum >= 0 )
	{
		// ��Ƽ�ΰ�� '��'�� ���� �ʰ� ����. (�� üũ����)
		for( int n = 0; n < MAX_PARTYUSER; n++ )
		{
			iPartyUserIndex = gParty.m_PartyS[iPartyNum].Number[n];
			if( iPartyUserIndex < 0 )
				continue;

			if( iIndex == iPartyUserIndex )	// '��'�� ���� �������� �̵�
				continue;

			lpPartyObj = ( LPOBJECTSTRUCT )&gObj[iPartyUserIndex];	
			
			// ���巹�� Ȯ��
			if( lpPartyObj->Level < iNeedLevel )
				continue;
			
			// ���� �ʿ� �����ϰ�
			if( lpObj->MapNumber == lpPartyObj->MapNumber ) 
			{
				iDis = gObjCalDistance( lpObj, lpPartyObj );
				if( iDis < 10 )		// �Ÿ����
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

	// ������ ��� �Ǵ� ��Ƽ�� �� '��'
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
	// �׽�Ʈ��忡���� ����Ѵ�.
#if ( TESTSERVER == 0 )
	return;
#endif

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	int iState		= QS_CLEAR;	// Ŭ����� �����Ѵ�.

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

	GCServerMsgStringSend( "����Ʈ ����", lpObj->m_Index, 1 );
}
#endif	// THIRD_CHANGEUP_SYSTEM_20070507

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// ����Ʈ �������� ������ �� �ִ��� Ȯ��
BOOL CQuestInfo::GetQuestItem( int iIndex, short nType, short nLevel )
{
	if( gObj[iIndex].m_iQuestMonKillDBIndex != -1 )
	{
		// �������� ��� ����Ʈ�� �ƴϴ�.
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
			
				// ������ ���� ����Ʈ�� �ƴ϶�� �н�.
				if( lpSubInfo->QuestType != 1 )
					return FALSE;

				// �������� ����Ʈ�� �������ΰ�
				if( nType == MAKE_ITEMNUM( lpSubInfo->NeedType, lpSubInfo->NeedSubType ) 
						&& nLevel == lpSubInfo->ItemLevel )
				{
					// ���� Ȯ��
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
			
			// ���� ����Ʈ�� ���� �ʴ´�.
			return FALSE;
		}
	}
	return FALSE;
}
	
#endif	// MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704

#endif