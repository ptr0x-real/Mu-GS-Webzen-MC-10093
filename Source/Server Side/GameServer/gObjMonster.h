#ifndef __GOBJMONSTER_H__
#define __GOBJMONSTER_H__

// CRYWOLF_MONSTER_WORK_20050914 - �Ʒ� Attr�� �̰����� �ű���.


#ifdef ADD_NEW_MAP_KALIMA_20040518		// ������ ���� �Ӽ� (m_Attribute)

#define MON_ATTR_KALIMAGATE_1		51	// ����1 -> Į����1
#define MON_ATTR_KALIMAGATE_2		52	// ����2 -> Į����2
#define MON_ATTR_KALIMAGATE_3		53	// ����3 -> Į����3
#define MON_ATTR_KALIMAGATE_4		54	// ����4 -> Į����4
#define MON_ATTR_KALIMAGATE_5		55	// ����5 -> Į����5
#define MON_ATTR_KALIMAGATE_6		56	// ����6 -> Į����6
#ifdef HIDDEN_KALIMA_20050706
#define MON_ATTR_KALIMAGATE_7		57	// ����7 -> Į����7 (���� Į����)
#define MON_ATTR_KALIMAGATE_NXT		58	// ���� (Ư��) -> �ڽ��� Į���� �� ���� ������ ���� (1->6����)
#endif

#ifdef HIDDEN_KALIMA_20050706
#define CHECK_KALIMAGATE(C_KGATTR)	(C_KGATTR<MON_ATTR_KALIMAGATE_1?FALSE:C_KGATTR>MON_ATTR_KALIMAGATE_NXT?FALSE:TRUE)
#define CHECK_KALIMAMAP(C_VALUE)	(C_VALUE<MAP_INDEX_KALIMA1?FALSE:C_VALUE>MAP_INDEX_KALIMA6?C_VALUE!=MAP_INDEX_KALIMA7?FALSE:TRUE:TRUE)
#else
#define CHECK_KALIMAGATE(C_KGATTR)	(C_KGATTR<MON_ATTR_KALIMAGATE_1?FALSE:C_KGATTR>MON_ATTR_KALIMAGATE_6?FALSE:TRUE)
#define CHECK_KALIMAMAP(C_VALUE)	(C_VALUE<MAP_INDEX_KALIMA1?FALSE:C_VALUE>MAP_INDEX_KALIMA6?FALSE:TRUE)
#endif
#endif

#define MON_ATTR_SUMMONED			60	// ��ȯ�� ������ -> �׾��� �� gObjDel()

#ifdef CASTLEDEEP_EVENT_20050413
#define MON_ATTR_INSTANCE			61	// �ӽ� ������ (����)
#define MON_ATTR_CASTLEDEEP			62	// �η����� �̺�Ʈ ������
#endif

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	// ���� ���ָӴ� ���� �ε���
#define MON_NEWYEAR_LUCKYBAG		365
#endif

extern int  gObjMonsterTopHitDamageUser(LPOBJECTSTRUCT lpMonObj);
extern void gObjMonsterProcess(LPOBJECTSTRUCT	lpObj);
extern BOOL gObjMonsterBackSpring(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	extern void gObjMonsterStateProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int aMsgSubCode = 0);
#else
	extern void gObjMonsterStateProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex);
#endif

extern BOOL gObjMonsterRegen(LPOBJECTSTRUCT lpObj);
extern void gObjMonsterSetHitDamage(LPOBJECTSTRUCT lpObj, int hit_player, int hit_damage );
extern void gObjMonsterHitDamageInit(LPOBJECTSTRUCT lpObj);
extern int  gObjMonsterHitDamageUserDel(LPOBJECTSTRUCT lpObj);
extern int	gObjCallMonsterSetEnemy(LPOBJECTSTRUCT lpObj, int Target);
extern int  gObjMonsterTopHitDamageUser(LPOBJECTSTRUCT lpMonObj);

extern int	gObjMonsterSearchEnemy(LPOBJECTSTRUCT lpObj, BYTE objtype);
extern int	gObjMonsterGetTargetPos(LPOBJECTSTRUCT lpObj);

extern void gObjMonsterMoveAction(LPOBJECTSTRUCT lpObj);
extern BOOL PathFindMoveMsgSend(LPOBJECTSTRUCT lpObj);

// ���������� ���� ĳ���͸� ã�´�
extern int gObjMonsterLastHitDamageUser(LPOBJECTSTRUCT lpMonObj, int& hitindex);

#ifdef NEW_SKILL_FORSKYLAND
//õ���� ���� ������ �� ����
extern void	gObjSkylandBossSheildAttack(LPOBJECTSTRUCT lpObj);

extern BYTE NewOptionRand(int level);
extern void gObjMonsterDieGiveItem(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
#endif

#ifdef ADD_MONSTER_DIE_REWARD_ITEMS_20040915
extern void gObjMonsterDieRewardItems( 
									   LPOBJECTSTRUCT lpObj, 
									   LPOBJECTSTRUCT lpTargetObj,
									   INT	iCount = 1,
									   INT	iDropRateCommonItem = -1, 
									   INT  iDropRateExcellentItem = -1,
									   INT  iDropRateSetItem = -1,
									   BOOL bMustHaveSkill = FALSE,
									   BOOL bMustHaveLuck = FALSE,
									   BOOL bMustHaveAdditionalOption = FALSE
									 );
#endif

#ifdef ADD_ABILLITY_KUNDUN_01_20040920
// iRefillRate / 10000 = %  (�⺻ 1%)
extern void gObjRefillMonsterHP( LPOBJECTSTRUCT lpMonsterObj, INT iRefillHPSec = 500 );
extern void gObjUseMonsterSpecialAbillity( LPOBJECTSTRUCT lpMonsterObj );
#endif

#ifdef SECOND_QUEST

typedef struct 
{
	int mapnum;
	int x;
	int y;
	int dir;
}QuestNPCTeleportPos;


class CQeustNpcTeleport
{
	int					RefCount;
	CRITICAL_SECTION	CsRef;
	int					TimeCount;

	int					LastMapNumber;

public:
	QuestNPCTeleportPos	m_QuestNPCTeleportPos[4];
public:
	CQeustNpcTeleport()
	{
		LastMapNumber = -1;
		RefCount = 0;
		InitializeCriticalSection(&CsRef);
		TimeCount = 0;
		m_QuestNPCTeleportPos[0].mapnum = MAP_INDEX_DEVIAS;	m_QuestNPCTeleportPos[0].x = 198;
		m_QuestNPCTeleportPos[0].y = 47;					m_QuestNPCTeleportPos[0].dir = 2;

		m_QuestNPCTeleportPos[1].mapnum = MAP_INDEX_RORENCIA;	m_QuestNPCTeleportPos[1].x = 137;
		m_QuestNPCTeleportPos[1].y = 87;						m_QuestNPCTeleportPos[1].dir = 1;

		m_QuestNPCTeleportPos[2].mapnum = MAP_INDEX_NORIA;	m_QuestNPCTeleportPos[2].x = 169;
		m_QuestNPCTeleportPos[2].y = 89;					m_QuestNPCTeleportPos[2].dir = 2;

		m_QuestNPCTeleportPos[3].mapnum = MAP_INDEX_ATHLANSE;	m_QuestNPCTeleportPos[3].x = 17;
		m_QuestNPCTeleportPos[3].y = 25;						m_QuestNPCTeleportPos[3].dir = 2;
	};
	~CQeustNpcTeleport(){DeleteCriticalSection(&CsRef);}

	void				TalkRefAdd()
	{
		EnterCriticalSection(&CsRef);
		RefCount++;
		LogAdd("QeustNpc RefCount Inc = %d", RefCount);
		LeaveCriticalSection(&CsRef);
	}

	void				TalkRefDel()
	{
		EnterCriticalSection(&CsRef);
		RefCount--;
		if( RefCount < 0 )
		{
			RefCount = 0;
		}
		LogAdd("QeustNpc RefCount Dec= %d", RefCount);
		LeaveCriticalSection(&CsRef);
	}	

	int					GetRefCount(){return RefCount;}	

	void				Run(int aIndex);
};
extern CQeustNpcTeleport	gQeustNpcTeleport;		// NPC���� ���� ����� ���� ����Ʈ

#endif

#endif