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
	BYTE	BuffIndex;								// ���� �ε���
	BYTE	BuffEffectType;							// ���� ȿ�� Ÿ�� (���� ȿ���� ������ ȿ���� ������ ����)
	BYTE	ItemType;								// ������ Ÿ��
	BYTE	ItemIndex;								// ������ �ε���
	char	BuffName[MAX_BUFF_NAME_LENGTH];			// ���� �̸�
	BYTE	BuffType;								// ����, �����
	BYTE	NoticeType;								// �˸� Ÿ�� (����, ����Ʈ, ��ü)
	BYTE	ClearType;								// �ʱ�ȭ Ÿ�� (�ϰ������� �����Ǵ� ���)
	char	BuffDescript[MAX_DESCRIPT_LENGTH];		// ���� ����
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
