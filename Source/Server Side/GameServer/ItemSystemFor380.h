// ItemSystemFor380.h: interface for the CItemSystemFor380 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSYSTEMFOR380_H__C69D017E_B1AA_4120_81D8_971216D676D6__INCLUDED_)
#define AFX_ITEMSYSTEMFOR380_H__C69D017E_B1AA_4120_81D8_971216D676D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_380ITEM_NEWOPTION_20060711

// �߰� ������ ȿ��
const BYTE ITEMOPTION_FOR380ITEM_NULL					  = 0;	// �ɼ� ����
const BYTE ITEMOPTION_FOR380ITEM_ATTACKSUCCESRATE_PVP	  = 1;	// ���� ���� ������ ���
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_DAMAGE_PVP		  = 2;	// ���� ���������� +%d
const BYTE ITEMOPTION_FOR380ITEM_BLOCKSUCCESRATE_PVP	  = 3;	// ���� ��� ������ ���
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_DEFENSE_PVP	  = 4;  // ���� ���� +%d
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_MAXHP			  = 5;	// �ִ� HP ���
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_MAXSD			  = 6;	// �ִ� SD ���
const BYTE ITEMOPTION_FOR380ITEM_RECOVER_SD				  = 7;	// SD �ڵ�ȸ�� : �������밡 �ƴϴ���.
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_REFILLSD		  = 8;	// SD �ڵ� ȸ���� ����

// 380 �߰� �ɼ� ���̺� ����
#ifdef MODIFY_380ITEM_SCRIPTTYPE_EXTEND_20060811
	typedef struct 
	{
		BOOL IsLoad;		// Ȱ�� ����
		BYTE m_Type;		// ������ Ÿ��
		BYTE m_Index;		// ������ �ε���
		BYTE m_Option1;		// ������ �ɼ�1
		WORD m_Value1;		// ������ �ɼ�1 ��
		BYTE m_Option2;		// ������ �ɼ�2
		WORD m_Value2;		// ������ �ɼ�2 ��
	} ITEMOPTION_FOR380ITEM;
#else
	typedef struct 
	{
		BOOL IsLoad;		// Ȱ�� ����
		BYTE m_Type;		// ������ Ÿ��
		BYTE m_Index;		// ������ �ε���
		BYTE m_Option1;		// ������ �ɼ�1
		BYTE m_Value1;		// ������ �ɼ�1 ��
		BYTE m_Option2;		// ������ �ɼ�2
		BYTE m_Value2;		// ������ �ɼ�2 ��
	} ITEMOPTION_FOR380ITEM;
#endif // MODIFY_380ITEM_SCRIPTTYPE_EXTEND_20060811
	
// 380 �߰� �ɼ� ���� ����
typedef struct  
{
	short	OpAddAttackSuccessRatePVP;	// ���� ���� ������ ���
	short   OpAddDamage;			    // ���� ���������� +%d
	short	OpAddDefenseSuccessRatePvP;	// ���� ��� ������ ���
	short	OpAddDefense;				// ���� ���� +%d
	short	OpAddMaxHP;					// �ִ� HP ���
	short	OpAddMaxSD;					// �ִ� SD ���
	short	OpRefillOn;					// SD�ڵ� ȸ�� On : �������밡 �ƴϴ��� ȸ��.
	short	OpAddRefillSD;				// SD�ڵ� ������ ���
} ITEMOPTION_FOR380ITEM_EFFECT, *LPITEMOPTION_FOR380ITEM_EFFECT;

class CItemSystemFor380  
{
public:
	CItemSystemFor380();
	virtual ~CItemSystemFor380();

private:
	// 380 �߰� �ɼ� ������
	ITEMOPTION_FOR380ITEM m_itemOption[MAX_ITEM];

	// ȯ�溯��
	BOOL	m_bSystemFor380ItemOption;						// �ý��� On/Off
	int		m_iNeedJewelOfHarmonyCount;						// ��ȭ�Ǻ��� ��
	int		m_iNeedJewelOfSuhoCount;						// ��ȣ�Ǻ��� ��
	int		m_iNeedZenFor380Option;							// �ʿ���
	int		m_iRateSuccessRateForMix1;						// 380 �ɼ� ������ ���� Ȯ��. 1�ܰ�
	int		m_iRateSuccessRateForMix2;						// 380 �ɼ� ������ ���� Ȯ��. 2�ܰ�
	int		m_iRateSuccessRateForMix3;						// 380 �ɼ� ������ ���� Ȯ��. 3�ܰ�
	void	_InitOption();

public:
	BOOL	Load380ItemOptionInfo(char* filename);			// script load

	BOOL	Is380Item( const CItem* pItem );				// 380 ������ Ȯ��
	BOOL	Is380OptionItem( const CItem* pItem );			// 380 �ɼ� ���� ������ Ȯ��

	void	InitEffectValue( LPITEMOPTION_FOR380ITEM_EFFECT pItemEffect );
	BOOL	ApplyFor380Option(LPOBJECTSTRUCT lpObj);		// 380 �ɼ� ����

private:
	BOOL	_CalcItemEffectValue(int iItemOptionType, int iItemEffectValue, LPITEMOPTION_FOR380ITEM_EFFECT pItemEffect);
	BOOL	_SetOption(CItem *pItem, BOOL bOption);			// �ش� �����ۿ� 380 �ɼ� �߰�

public:
	void	SetOptionItemByMacro(LPOBJECTSTRUCT lpObj, BYTE invenrotyTargetPos, BOOL bOption);	// ��ũ�θ� �̿��� �ɼ��߰�

public:
	BOOL	ChaosMix380ItemOption(LPOBJECTSTRUCT lpObj);	// 380 ������ �ɼ� ����
};


extern CItemSystemFor380	g_kItemSystemFor380;


inline void DebugLog(char *szlog, ...)
{
	char szBuffer[512]="";
	va_list		pArguments;

	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);

	OutputDebugString( szBuffer );
}

#endif // ADD_380ITEM_NEWOPTION_20060711

#endif // !defined(AFX_ITEMSYSTEMFOR380_H__C69D017E_B1AA_4120_81D8_971216D676D6__INCLUDED_)
