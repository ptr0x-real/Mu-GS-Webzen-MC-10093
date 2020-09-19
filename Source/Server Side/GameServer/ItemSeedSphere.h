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

	// 시드 인덱스
	BYTE	m_SeedIndex;
	// 시드 속성 ( 1 : 불, 2 : 물, 3 : 얼음, 4 : 바람, 5 : 번개, 6 : 땅, 7 : 유니크 )
	BYTE	m_SeedType;
	// 시드 레벨 ( 시드 레벨에 따라 속성이 결정된다. )
	BYTE	m_ItemLevel;
	// 시드 추가 스텟 값 타입 ( 1 : 고정 값, 2 : 퍼센트, 3 : 레벨 / 20 )
	BYTE	m_SeedOptionValueType;
	// 시드 추가 스텟 값
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	BYTE	m_SeedOptionValue[MAX_SPHERE_LEVEL];
#else
	BYTE	m_SeedOptionValue;
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
	// 시드 아이템 코드
	int		m_ItemCode;
	// 시드 추출 확률
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

	// 스피어 레벨
	BYTE	m_SphereLevel;
	// 스피어 레벨별 수치 증가율
	int		m_ValueIncreaseRate;
	// 스피어 아이템 코드
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

	// 시드스피어 인덱스
	BYTE	m_SeedSphereIndex;
	// 시드 인덱스
	BYTE	m_SeedIndex;
	// 시드 속성 ( 1 : 불, 2 : 물, 3 : 얼음, 4 : 바람, 5 : 번개, 6 : 땅, 7 : 유니크 )
	BYTE	m_SeedType;
	// 시드 아이템 코드
	int		m_ItemCode;
	// 시드 레벨 ( 시드 레벨에 따라 속성이 결정된다. )
	BYTE	m_ItemLevel;
	// 시드 추가 스텟 값 타입 ( 1 : 고정 값, 2 : 퍼센트, 3 : 레벨 / 20 )
	BYTE	m_SeedOptionValueType;
	// 시드 추가 스텟 값
	WORD	m_SeedOptionValue;
	// 스피어 레벨
	BYTE	m_SphereLevel;

private:
};
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#endif // !defined(AFX_ITEMSEEDSPHERE_H__2D97B05A_BE6F_4BD2_8A6B_EF18227C168B__INCLUDED_)