// ItemSeedSphere.h: interface for the CItemSeedSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSEEDSPHERE_H__2D97B05A_BE6F_4BD2_8A6B_EF18227C168B__INCLUDED_)
#define AFX_ITEMSEEDSPHERE_H__2D97B05A_BE6F_4BD2_8A6B_EF18227C168B__INCLUDED_

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSeedItem
{
public:
	CSeedItem();
	~CSeedItem();

	int		GetItemCode() { return m_ItemCode; };

	CSeedItem &operator= (const CItem& rhs);

	bool	IsMatched(CItem* lpItem = NULL);
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	void	SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, BYTE SeeedOptionValue[], int ExtractRate );
#else
	void	SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, BYTE SeeedOptionValue, int ExtractRate );
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

	void	Clear();

	// �õ� �ε���
	BYTE	m_SeedIndex;
	// �õ� �Ӽ� ( 1 : ��, 2 : ��, 3 : ����, 4 : �ٶ�, 5 : ����, 6 : ��, 7 : ����ũ )
	BYTE	m_SeedType;
	// �õ� ���� ( �õ� ������ ���� �Ӽ��� �����ȴ�. )
	BYTE	m_ItemLevel;
	// �õ� �߰� ���� �� Ÿ�� ( 1 : ���� ��, 2 : �ۼ�Ʈ, 3 : ���� / 20 )
	BYTE	m_SeedOptionValueType;
	// �õ� �߰� ���� ��
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	BYTE	m_SeedOptionValue[MAX_SPHERE_LEVEL];
#else
	BYTE	m_SeedOptionValue;
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
	// �õ� ������ �ڵ�
	int		m_ItemCode;
	// �õ� ���� Ȯ��
	int		m_SeedExtractRate;

private:
};

class CSphereItem
{
public:
	CSphereItem();
	~CSphereItem();

	int		GetItemCode() { return m_ItemCode; };
	void	SetOption(BYTE SphereLevel, int ValueIncreaseRate);

	void	Clear();

	// ���Ǿ� ����
	BYTE	m_SphereLevel;
	// ���Ǿ� ������ ��ġ ������
	int		m_ValueIncreaseRate;
	// ���Ǿ� ������ �ڵ�
	int		m_ItemCode;

private:
};

class CSeedSphereItem
{
public:
	CSeedSphereItem();
	~CSeedSphereItem();

	int		GetItemCode() { return m_ItemCode; };
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	bool	SetOption(CSeedItem* SeedItem, BYTE SphereLevel);
#else
	bool	SetOption(CSeedItem* SeedItem, CSphereItem* SphereItem);
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

	void	AddOptionValue(BYTE btOptionValue);

	void	Clear();

	// �õ彺�Ǿ� �ε���
	BYTE	m_SeedSphereIndex;
	// �õ� �ε���
	BYTE	m_SeedIndex;
	// �õ� �Ӽ� ( 1 : ��, 2 : ��, 3 : ����, 4 : �ٶ�, 5 : ����, 6 : ��, 7 : ����ũ )
	BYTE	m_SeedType;
	// �õ� ������ �ڵ�
	int		m_ItemCode;
	// �õ� ���� ( �õ� ������ ���� �Ӽ��� �����ȴ�. )
	BYTE	m_ItemLevel;
	// �õ� �߰� ���� �� Ÿ�� ( 1 : ���� ��, 2 : �ۼ�Ʈ, 3 : ���� / 20 )
	BYTE	m_SeedOptionValueType;
	// �õ� �߰� ���� ��
	WORD	m_SeedOptionValue;
	// ���Ǿ� ����
	BYTE	m_SphereLevel;

private:
};
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#endif // !defined(AFX_ITEMSEEDSPHERE_H__2D97B05A_BE6F_4BD2_8A6B_EF18227C168B__INCLUDED_)