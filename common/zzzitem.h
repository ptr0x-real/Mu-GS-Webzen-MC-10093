#ifndef __ZZZITEM_H__
#define __ZZZITEM_H__

#include "..\\Include\\ItemDef.h"


#ifdef ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808
extern int g_MaxItemIndexOfEachItemType[MAX_ITEM_TYPE];
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#include "ItemSocketOptionDefine.h"
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];

class CItem
{
public :
	DWORD m_Number;
	char  m_serial;
	short m_Type;
	short m_Level;	
	BYTE  m_Part;
	BYTE  m_Class;
	bool  m_TwoHand;
	BYTE  m_AttackSpeed;
	BYTE  m_WalkSpeed;
	WORD  m_DamageMin;
	WORD  m_DamageMax;
	BYTE  m_SuccessfulBlocking;
	WORD  m_Defense;
	WORD  m_MagicDefense;
	BYTE  m_Speed;

	WORD  m_DamageMinOrigin;
	WORD  m_DefenseOrigin;
	WORD  m_Magic;
	
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	WORD  m_Curse;	// ���ַ�
#endif

	//BYTE  m_Durability;
	float m_Durability;
	WORD  m_DurabilitySmall;

	float m_BaseDurability;		// ������1�� ���� �������(Value���� ����� �д�)

	BYTE  m_SpecialNum;
	BYTE  m_Special     [MAX_ITEM_SPECIAL];
	BYTE  m_SpecialValue[MAX_ITEM_SPECIAL];	// ����

	WORD  m_RequireStrength;
	WORD  m_RequireDexterity;
	WORD  m_RequireEnergy;
	WORD  m_RequireLevel;

#ifdef EXPAND_ITEMSCRIPT_01_20050808
	WORD  m_RequireVitality;
#endif

#ifdef DARKLORD_WORK
	WORD  m_RequireLeaderShip;
	WORD  m_Leadership;		// �߰� ���
#endif

	BYTE  m_RequireClass[MAX_CLASSTYPE];
	BYTE  m_Resistance[MAX_RESISTANCE];

	int   m_Value;
	DWORD m_SellMoney;	// �ȶ� ��
	DWORD m_BuyMoney;	// �춧 ��

#ifdef PERSONAL_SHOP_20040113
	int	  m_iPShopValue;	// ���λ������� �ȶ��� �� (����� ����)
#endif
	
#ifdef ITEMDELETE_MODIFY_20040604	// ������ �Ӽ� �� �������� �����ϴ� �������ΰ� �ƴѰ��� ���� �÷���
	bool  m_bItemExist;
#endif
	
	// ī���� ������ ���� ������ ��ġ�� �ٲ�� ���� �� ���
	int   m_OldSellMoney;	// �ȶ� ��
	int   m_OldBuyMoney;	// �춧 ��

	BYTE  m_Option1;	// ������ �ɼ� 1 ���õǾ�����?
	BYTE  m_Option2;	// ������ �ɼ� 2 ���õǾ�����?
	BYTE  m_Option3;	// ������ �ɼ� 3 ���õǾ�����?

	BYTE  m_NewOption;	// ���� �߰��� �ɼ�, ��Ʈ üũ

	float  m_DurabilityState[4];
	float  m_CurrentDurabilityState;

#ifdef DARKLORD_WORK
	BYTE	m_SkillChange;
#endif

#ifdef NEW_SKILL_FORSKYLAND
	BYTE m_QuestItem;				// ����Ʈ ������?
#endif

	BYTE	m_SetOption;
	BYTE	m_SetAddStat;			// ��Ʈ �����ۿ� �ٴ� �߰� ����
	BYTE	m_IsValidItem;			// ��ȿ�� �������ΰ�?(�ɷ�ġ�� �Ǵ� �������ΰ�?)

	BYTE	m_SkillResistance[MAX_RESISTANCE];



#ifdef DARKLORD_WORK	// ��ũ���Ǹ�, ��ũȣ�� ���� ��(�����װ���ġ�� ������) ������ ���� 
	BOOL	m_IsLoadPetItemInfo;	// ������� ������ ��� �Ծ���?
	int		m_PetItem_Level;	
	int		m_PetItem_Exp;
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� ���� ��ȭ �ɼ�
	BYTE	m_JewelOfHarmonyOption;
	// ��ȭ�� ���� �ɼ� : �ʿ� ���� ����
	WORD	m_HJOpStrength;
	WORD	m_HJOpDexterity;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	// ������ �߰� �ɼ� ( Ȯ�� ��� )
	BYTE	m_ItemOptionEx;	// ���� 1 bit :  380 ������ �߰� �ɼ�
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	BYTE	m_BonusSocketOption;
	BYTE	m_SocketOption[MAX_SOCKET_SLOT];
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	// ���� ������ ������ ��ȭ ���� ����
	BYTE	m_ImproveDurabilityRate;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
public :
	CItem();
	
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	void Convert( int type, BYTE Option1=0, BYTE Option2=0, BYTE Option3=0, BYTE Attribute2=0, BYTE SetOption = 0, BYTE ItemEffectEx = 0, BYTE SocketOption[] = NULL, BYTE SocketBonusOption = 255, BYTE DbVersion=3 );
#else
	#ifdef ITEM_INDEX_EXTEND_20050706
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			void Convert( int type, BYTE Option1=0, BYTE Option2=0, BYTE Option3=0, BYTE Attribute2=0, BYTE SetOption = 0, BYTE ItemEffectEx = 0, BYTE DbVersion=3);
		#else
			void Convert( int type, BYTE Option1=0, BYTE Option2=0, BYTE Option3=0, BYTE Attribute2=0, BYTE SetOption = 0, BYTE DbVersion=3);
		#endif  // ADD_380ITEM_NEWOPTION_20060711
	#else
		void Convert( int type, BYTE Option1=0, BYTE Option2=0, BYTE Option3=0, BYTE Attribute2=0, BYTE SetOption = 0, BYTE DbVersion=1);
	#endif
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	
	void Value();	

	void OldValue();

	BOOL GetSize(int &w, int &h);
	void Clear();
	BOOL IsItem();	

	BOOL	IsSetItem();
	int		GetAddStatType();	
	
#ifdef DARKLORD_WORK
	void	SetPetItemInfo(int petlevel, int petexp);
	// return value : level up : TRUE, else FALSE
	int		AddPetItemExp(int petexp);
	// return value : level down : TRUE, else FALSE
	int		DecPetItemExp(int percent);
	// �� ������ ���� ���
	void	PetValue();		
	// �� ������ ���� ����
	int		PetItemLevelDown(int exp);
#endif


	int  ItemDamageMin();
	int  ItemDefense();

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	int	 ItemCurse();
#endif

#ifdef ADD_THIRD_WING_20070525	// IsClass() ����
	BOOL IsClass( char aClass, int ChangeUP, int ThirdChangeUp );
	#ifdef NEW_SKILL_FORSKYLAND
	BOOL IsClass(char aClass, int ChangeUP);
	#else
	BOOL IsClass(char aClass);
	#endif
#else
	#ifdef NEW_SKILL_FORSKYLAND
	BOOL IsClass(char aClass, int ChangeUP);
	#else
	BOOL IsClass(char aClass);
	#endif
#endif	// ADD_THIRD_WING_20070525

	char *GetName();
	int  GetLevel(void);
	void PlusSpecial(int *Value,int Special);	
	void PlusSpecialPercent(int* Value,int Special, WORD Percent);

	void PlusSpecialPercentEx(int* Value,int SourceValue, int Special);
	void SetItemPlusSpecialStat(WORD* Value,int Special);
	int	 GetWeaponType();
	void PlusSpecialSetRing(BYTE* Value);

	DWORD GetNumber();


	int IsExtItem();
	int IsExtLifeAdd();
	int IsExtManaAdd();
	int IsExtDamageMinus();
	int IsExtDamageReflect();
	int IsExtDefenseSuccessfull();
	int IsExtMonsterMoney();

	int IsExtExcellentDamage();
	int IsExtAttackRate();
	int IsExtAttackRate2();
	int IsExtAttackSpeed();
	int IsExtMonsterDieLife();
	int IsExtMonsterDieMana();
	
	int IsWingOpGetOnePercentDamage();
	int IsWingOpGetLifeToMonster();
	int IsWingOpGetManaToMoster();

	int IsDinorantReduceAttackDamaege();

#ifdef ADD_ITEM_FENRIR_01_20051110
	int IsFenrirIncLastAttackDamage();
	int IsFenrirDecLastAttackDamage();
#endif

#ifdef ADD_ILLUSION_FENRIR_20070703
	int IsIllusionFenrirOption();
#endif

#ifdef ADD_THIRD_WING_20070525	// 3�� ���� �߰��ɼ� �� �Լ�����
	int GetIgnoreDefencePer();		// ���湫�� Ȯ���� �ǵ����ش�.
#endif


	// ������ ����

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	int SimpleDurabilityDown( int iDur );
#endif

	int DurabilityDown(int dur, int aIndex);	
	int DurabilityDown2(int dur, int aIndex);	

	int NormalWeaponDurabilityDown(int defence, int aIndex);
	int BowWeaponDurabilityDown(int defence, int aIndex);
	int StaffWeaponDurabilityDown(int defence, int aIndex);

	int ArmorDurabilityDown(int damagemin, int aIndex);

	int	CheckDurabilityState();		// ������ ���� �˻�

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	// ���� ������ �����ִ� ���������� Ȯ��
	bool	IsEnableSetSocketItem();
	// ���� ������ ����
	void	MakeSocketSlot(BYTE SlotCount);
	// �õ彺�Ǿ ���Ͽ� ����
	bool	SetSeedSphere(BYTE SlotIndex, BYTE SeedIndex, BYTE SphereLevel);
	// �õ彺�Ǿ ���Ͽ��� ����
	bool	RemoveSeedSphere(BYTE SlotIndex);
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

	BOOL CheckExcOption(int iMinExcOption);
};

extern BOOL OpenItemScript(char *FileName);

#ifdef AUTH_GAMESERVER
extern BOOL OpenItemScript(char* Buffer, int iSize);
extern BOOL OpenItemNameScript(char *Buffer, int iSize);
#endif


#ifdef SCRIPT_DECODE_WORK
extern BOOL OpenItemScriptDecode(char *FileName);
extern BOOL OpenItemNameScriptDecode(char *FileName);
#endif

extern BOOL OpenItemNameScript(char *FileName);
extern int  ItemGetNumberMake(int type, int index );
extern void ItemGetSize(int index, int & width, int & height);
extern void ItemByteConvert3(LPBYTE buf, CItem item[], int maxitem);
extern void ItemByteConvert7(LPBYTE buf, CItem item[], int maxitem);
extern void ItemByteConvert10(LPBYTE buf, CItem item[], int maxitem);
extern void ItemByteConvert(LPBYTE buf, CItem item);

#ifdef ITEM_DBSIZE_EXTEND_20050706
extern void ItemByteConvert16(LPBYTE buf, CItem item[], int maxitem);
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
extern void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption=0, BYTE JewelOfHarmonyOption=0, BYTE ItemEffectEx =0, BYTE SocketOption[] = NULL, BYTE SocketBonusOption = 255);
#else

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		extern void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption=0, BYTE JewelOfHarmonyOption=0, BYTE ItemEffectEx =0);
	#else
		extern void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption=0, BYTE JewelOfHarmonyOption=0);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		extern void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption=0, BYTE ItemEffectEx =0);
	#else
		extern void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption=0);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif //ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

extern int  GetLevelItem(int type, int index, int level);
extern BOOL zzzItemLevel(int type, int index, int level);
extern int GetSerialItem(int type);
extern void BufferItemtoConvert3(LPBYTE buf, int & type, BYTE & level, BYTE & op1, BYTE & op2, BYTE & op3, BYTE & dur);

// �������ΰ�?
extern BOOL IsItem(int item_num);

// �������� �������� ������ �ִ°�?
extern BOOL HasItemDurability(int index);

extern int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem);
extern int ItemGetAttackDurability(int index);
extern int ItemGetDefenseDurability(int index);
extern void CalRepairRate(int type, int index, ITEM_ATTRIBUTE *p);

extern inline float	GetRepairItemRate(int index);
extern inline float	GetAllRepairItemRate(int index);

extern ITEM_ATTRIBUTE *GetItemAttr(int item_num);

#ifdef DARKLORD_WORK
#define MAX_DARK_SPIRIT_LEVEL	50
#define MAX_DARK_HORSE_LEVEL	50

class CPetItemExp
{	
public:
	CPetItemExp()
	{		
		m_DarkSpiritExpTable[0] = 0;
		m_DarkSpiritExpTable[1] = 0;
		for( int i = 2; i < MAX_DARK_SPIRIT_LEVEL+2; i++ )
		{	
			m_DarkSpiritExpTable[i] = ((10+i)*(i)*(i)*(i)*100);
		}

		m_DarkHorseExpTable[0] = 0;
		m_DarkHorseExpTable[1] = 0;
		for( int x = 2; x < MAX_DARK_HORSE_LEVEL+2; x++ )
		{				
			m_DarkHorseExpTable[x] = ((10+x)*(x)*(x)*(x)*100);
		}
	}
	int		m_DarkSpiritExpTable[MAX_DARK_SPIRIT_LEVEL+2];
	int		m_DarkHorseExpTable[MAX_DARK_SPIRIT_LEVEL+2];
};
#endif

#endif

