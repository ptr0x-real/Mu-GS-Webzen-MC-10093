// SetItemOption.h: interface for the CSetItemOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETITEMOPTION_H__E2F2BB72_7EB5_4280_A54C_783B6EFDA283__INCLUDED_)
#define AFX_SETITEMOPTION_H__E2F2BB72_7EB5_4280_A54C_783B6EFDA283__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if GAME_VERSION >= G_V_99B

#include "..\\Common\\LogProc.h"
#include "..\\Include\\Define.h"
#include "..\\Include\\ItemDef.h"
#include "..\\Include\\ReadScript.h"
#include "..\\common\\Winutil.h"
#include "..\\common\\zzzitem.h"


// ��Ʈ ������ �ɼ� ����Ʈ
//0		���� ������ Ȯ�� 2%
//1		�� ���� ���� 2%
//2		��հ� ���� ������ 15% ����
//3		���� ������ ���� 5%
//4		100% ������ �ݻ� Ȯ�� 5% ����
//5		���� ���� ���� 5% ����
//6		�� ���� +10
//7		�� ���� +20
//8		��ø�� ���� +10
//9		��ø�� ���� +20
//10	������ ���� +10
//11	������ ���� +20
//12	ü�� ���� +10
//13	ü�� ���� +20
//14	�ִ� ����� ���� +40
//15	�ִ� ����� ���� +80
//16	�ִ� ���� ���� +40
//17	�ִ� ���� ���� +80
//18	�ִ� AG 5% ����
//19	�Һ� AG 10% ����
//20	������ ��� Ȯ�� 5% ����
//21	Ư�� ĳ���� ���ݷ� ���� ��ø��/60
//22	Ư�� ĳ���� ���ݷ� ���� ��/50
//23	Ư�� ĳ���� ���� ���� ������/70
//24	Ư�� ĳ���� ���� ���� ����/20
//25	Ư�� ĳ���� ���� ���� 3%
//26	Ư�� ĳ���� ���� ���� ��ø��/50
//27	Ư�� ĳ���� ���� ���� ü��/40
//28	Ư�� ĳ���� �ҼӼ� �߰�
//29	Ư�� ĳ���� �����Ӽ� �߰�
//30	Ư�� ĳ���� �����Ӽ� �߰�
//31	Ư�� ĳ���� ���Ӽ� �߰�
//32	Ư�� ĳ���� ���Ӽ� �߰�
//33	Ư�� ĳ���� �ٶ��Ӽ� �߰�
//34	Ư�� ĳ���� ���Ӽ� �߰�

#define		MAX_SETITEM_OPTION	80

// ��Ʈ ������ Ÿ�� ���� 
// (�ش� �ɼǿ� ���� �ε��� �� ī���� ���� ���� ������ ������)
typedef struct 
{
	BOOL	IsLoad;

	int	OptionIndex[2];
	int	ChaosMixLevel[2];	
}SETITEM_TYPEINFO;

// ��Ʈ ������ �ɼǿ� ���� �� ����
typedef struct 
{
	BOOL	IsLoad;
	char	Name[255];						// ��Ʈ ������ �̸�
	
	int		SetOptionCount;					// �⺻ ��Ʈ �ɼ� ��
	int		SetOptionTable[6][2];			// �������� �ɼ��� �ΰ���
	int		SetOptionTableValue[6][2];		// �ɼ��� ��
	int		SetExPOptionTable[2];			// Ȯ�� �ɼ� 7~8 �ɼ�
	int		SetExPOptionTableValue[2];		// �ɼ��� ��
	int		SetFullOtionTable[5];			// Ǯ�ɼ� �ִ� 4��
	int		SetFullOtionTableValue[5];		// �ɼ��� ��

	BYTE	RequireClass[MAX_CLASSTYPE+1];	// �䱸 Ŭ����
}SETITEM_OPTIONINFO;

class CSetItemOption  
{
	SETITEM_TYPEINFO	m_SetItemTypeInfo[MAX_ITEM];
	SETITEM_OPTIONINFO	m_SetItemOptionInfo[MAX_SETITEM_OPTION];
public:
	CSetItemOption();
	virtual ~CSetItemOption();

	BOOL	LoadTypeInfo(char* filename);
	BOOL	LoadOptionInfo(char* filename);
	
#ifdef AUTH_GAMESERVER
	BOOL	LoadTypeInfo(char *Buffer, int iSize);
	BOOL	LoadOptionInfo(char *Buffer, int iSize);
#endif

	BOOL	IsSetItem(int itemnum);

	// �ش� �������� ��Ʈ �ɼ� �ε����� ��� �´�
	int		GetOptionTableIndex(int itemnum, int setoption);

	char*	GetSetOptionName(int optionindex);
	char*	GetSetOptionName(int itemnum, int settype);

	// �ɼ��� ���� �´�
	int		GetSetOption(int optionindex, int optablenum, int& op1, int& op2, int& opvalue1, int& opvalue2, int Class, int ChangeUp);
	// �ɼ��� �ִ� ���� ��� �´�
	int		GetMaxSetOptionCount(int optionindex);

	// Ǯ�ɼ��� ��� �´�
	int		GetGetFullSetOption(int optionindex, int& op1, int& op2, int& op3, int& op4, int& op5, 
								int& opvalue1, int& opvalue2, int& opvalue3, int& opvalue4, int& opvalue5, int Class, int ChangeUp );

	int		IsRequireClass(int optionindex, int Class, int ChangeUp);	

	BOOL	CheckMixContition(int itemnum, int mixitemlevel);
	// ��Ʈ �ɼ��� �����( A��Ʈ == 1, B��Ʈ == 2 )
	int		MakeSetOption(int itemnum, int mixitemlevel);

	int		GenRandomItemNum();
	int		GenSetOption(int itemnum);

	SETITEM_TYPEINFO *	GetSetType(int index);

};

extern CSetItemOption	gSetItemOption;

#endif

#endif // !defined(AFX_SETITEMOPTION_H__E2F2BB72_7EB5_4280_A54C_783B6EFDA283__INCLUDED_)
