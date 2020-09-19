// WhisperCash.h: interface for the CWhisperCash class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WHISPERCASH_H__9B7508BF_7EDB_4B3E_9341_2376854F1DEA__INCLUDED_)
#define AFX_WHISPERCASH_H__9B7508BF_7EDB_4B3E_9341_2376854F1DEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAXWHISPERCASH		2		// 귓말 캐쉬 최대 개수

typedef struct
{
	BOOL	Live;
	char	szId[MAX_IDSTRING];
	int		m_Index;
	DWORD	m_Time;
} WCHSH_STRUCT;


class CWhisperCash  
{
public:
	WCHSH_STRUCT	WChash[MAXWHISPERCASH];
	int				WChashCount;

public :
	CWhisperCash();
	virtual ~CWhisperCash();
	
	int  AddCash(char *id);
	void DelCash(char *id);
	int  SearchCash(char *id);
	void TimeCheckCashDelete();
	
	void Init()
	{
		WChashCount = 0;
		for( int n=0; n<MAXWHISPERCASH; n++)
		{
			WChash[n].Live = 0;
		}
	};
};

#endif // !defined(AFX_WHISPERCASH_H__9B7508BF_7EDB_4B3E_9341_2376854F1DEA__INCLUDED_)
