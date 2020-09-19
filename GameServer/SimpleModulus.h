// SimpleModulus.h: interface for the CSimpleModulus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEMODULUS_H__5EC20CDC_CA87_4F9D_927B_24C8DD11ECB7__INCLUDED_)
#define AFX_SIMPLEMODULUS_H__5EC20CDC_CA87_4F9D_927B_24C8DD11ECB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*# define SIZE_ENCRYPTION_BLOCK	( 32)
# define SIZE_ENCRYPTION_KEY	( 16)
# define SIZE_ENCRYPTED_BLOCK	( 38)*/

# define SIZE_ENCRYPTION_BLOCK	( 8)
# define SIZE_ENCRYPTION_KEY	( 4)	// SIZE_ENCRYPTION_BLOCK �� ����
# define SIZE_ENCRYPTED_BLOCK	( 11)


class CSimpleModulus  
{
public:
	CSimpleModulus();
	virtual ~CSimpleModulus();

	void Init( void);

protected:
	DWORD m_dwModulus[SIZE_ENCRYPTION_KEY];
	DWORD m_dwEncryptionKey[SIZE_ENCRYPTION_KEY];
	DWORD m_dwDecryptionKey[SIZE_ENCRYPTION_KEY];
	DWORD m_dwXORKey[SIZE_ENCRYPTION_KEY];

	static DWORD s_dwSaveLoadXOR[SIZE_ENCRYPTION_KEY];


	//////////////////////////////////////////////////////////////////////////
	// ��ȣȭ/��ȣȭ ->

	// ��ȣȭ
public:
	int Encrypt( void *lpTarget, void *lpSource, int iSize);	// ū ������ ���, lpTarget �� NULL�̸� �ʿ䷮ ����
	int Decrypt( void *lpTarget, void *lpSource, int iSize);	// ū ������ ���, lpTarget �� NULL�̸� �ʿ䷮ ����

protected:
	void EncryptBlock( void *lpTarget, void *lpSource, int nSize);	// 32 byte �� 38 byte ��
	int DecryptBlock( void *lpTarget, void *lpSource);	// 38 byte �� 32 byte ��, Size�� ������
	int AddBits( void *lpBuffer, int nNumBufferBits, void *lpBits, int nInitialBit, int nNumBits);
	void Shift( void *lpBuffer, int nByte, int nShift);
	int GetByteOfBit( int nBit);

	// <- ��ȣȭ/��ȣȭ
	//////////////////////////////////////////////////////////////////////////

	// ����/�б�
public:
	BOOL SaveAllKey( char *lpszFileName);
	BOOL LoadAllKey( char *lpszFileName);
	BOOL SaveEncryptionKey( char *lpszFileName);
	BOOL LoadEncryptionKey( char *lpszFileName);
	BOOL SaveDecryptionKey( char *lpszFileName);
	BOOL LoadDecryptionKey( char *lpszFileName);
protected:
	BOOL SaveKey( char *lpszFileName, unsigned short sID, BOOL bMod, BOOL bEnc, BOOL bDec, BOOL bXOR);
	BOOL LoadKey( char *lpszFileName, unsigned short sID, BOOL bMod, BOOL bEnc, BOOL bDec, BOOL bXOR);
};


#pragma pack ( 1)
typedef struct
{
	unsigned short m_sID;
	unsigned int m_iSize;
} ChunkHeader;
#pragma pack ()

# define CHUNKID_ALLKEY		( 0x1111)
# define CHUNKID_ONEKEY		( 0x1112)


#endif // !defined(AFX_SIMPLEMODULUS_H__5EC20CDC_CA87_4F9D_927B_24C8DD11ECB7__INCLUDED_)
