// BuffScriptLoader.h: interface for the CBuffScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFSCRIPTLOADER_H__AD3819A4_1C9A_4886_AB83_DA77FAAD7433__INCLUDED_)
#define AFX_BUFFSCRIPTLOADER_H__AD3819A4_1C9A_4886_AB83_DA77FAAD7433__INCLUDED_

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BUFFTYPEDEF.h"

typedef struct
{
	BYTE	BuffIndex;								// 버프 인덱스
	BYTE	BuffEffectType;							// 버프 효과 타입 (같은 효과를 가지는 효과의 종류를 구분)
	BYTE	ItemType;								// 아이템 타입
	BYTE	ItemIndex;								// 아이템 인덱스
	char	BuffName[MAX_BUFF_NAME_LENGTH];			// 버프 이름
	BYTE	BuffType;								// 버프, 디버프
	BYTE	NoticeType;								// 알림 타입 (개인, 뷰포트, 전체)
	BYTE	ClearType;								// 초기화 타입 (일괄적으로 삭제되는 대상)
	char	BuffDescript[MAX_DESCRIPT_LENGTH];		// 버프 설명
}BUFF_EFFECT_DATE,*LPBUFF_EFFECT_DATA;

class CBuffScriptLoader  
{
public:
	CBuffScriptLoader();
	virtual ~CBuffScriptLoader();
	bool Load(char* pchFileName);

	void Initialize();
	bool AddBuffEffectData(BYTE btBuffIndex, BYTE btBuffEffectType, BYTE btItemType, BYTE btItemIndex, char* pchName, BYTE btBuffType, BYTE btNoticeType, BYTE btClearType, char* pchDescript);
	bool CheckVaildBuffEffect(BYTE btBuffIndex);
	LPBUFF_EFFECT_DATA GetBuffData(BYTE btBuffIndex);
private:
	int iBuffCount;
	BUFF_EFFECT_DATE BuffData[MAX_BUFF_EFFECT_INDEX];
};

extern CBuffScriptLoader g_BuffScript;

#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#endif // !defined(AFX_BUFFSCRIPTLOADER_H__AD3819A4_1C9A_4886_AB83_DA77FAAD7433__INCLUDED_)
