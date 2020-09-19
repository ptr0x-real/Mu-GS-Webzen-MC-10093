// NSerialCheck.h: interface for the NSerialCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSERIALCHECK_H__73F21745_DB66_416F_985C_44630095E492__INCLUDED_)
#define AFX_NSERIALCHECK_H__73F21745_DB66_416F_985C_44630095E492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NSerialCheck  
{
	BYTE	m_serial;
	BYTE	m_SendSeiral;

public:
	NSerialCheck();
	virtual ~NSerialCheck();

	void  init();
	BOOL  InCheck(BYTE serial);
	int   GetSerial();

	BYTE	GetSendSerial();

};

#endif // !defined(AFX_NSERIALCHECK_H__73F21745_DB66_416F_985C_44630095E492__INCLUDED_)
