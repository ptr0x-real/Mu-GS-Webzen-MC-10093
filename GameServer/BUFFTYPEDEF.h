#define MAX_BUFF_NAME_LENGTH	20
#define MAX_DESCRIPT_LENGTH		100
#define MAX_BUFF_EFFECT_INDEX	254
#define MAX_BUFF_SLOT_INDEX		16

#define MAX_BUFF_EFFECT_COUNT	8

#define BUFFTIME_INFINITE		(-10)
#define BUFFTIME_MINUTE			(60)
#define BUFFTIME_HOUR			(60 * 60)
#define BUFFTIME_DAY			(24 * 60 * 60)

#define TYPE_BUFF				0
#define TYPE_DEBUFF				1

#define MASK_BUFFTYPE_FOREIGN4ND		0x0057
#define MASK_BUFFTYPE_SANTAEVENT		0x005B	

enum eBuffEffectIndex
{
	BUFFTYPE_NONE						= 0,
	BUFFTYPE_ATTACK_POWER_INC			,				// 요정 공격력 향상
	BUFFTYPE_DEFENSE_POWER_INC			,				// 요정 방어력 향상
	BUFFTYPE_ATTDEF_POWER_INC			,				// 쉐도우 펜텀 NPC 공방 향상
	BUFFTYPE_MELEE_DEFENSE_INC			,				// 마법사 마나 쉴드
	BUFFTYPE_CRITICAL_DMG_INC			,				// 다크로드 크리티컬 데미지 증가
	BUFFTYPE_INFINITY_ARROW				,				// 요정 인피니티 에로우
	BUFFTYPE_AG_RECOVERY_INC			,				// AG 회복속도 증가
	BUFFTYPE_HP_INC						,				// 기사 스웰 라이프
	BUFFTYPE_MANA_INC					,				// 스웰 마나
	BUFFTYPE_BLESS_POTION				,				// 축복의 물약
	BUFFTYPE_SOUL_POTION				,				// 영혼의 물약
	BUFFTYPE_MAGIC_CANCEL				,				// 마법 취소 - 공성 스킬
	BUFFTYPE_CASTLE_DOOR_STATE			,
	BUFFTYPE_CASTLE_DEFENSE_MARK		,
	BUFFTYPE_CASTLE_ATTACK_MARK			,
	BUFFTYPE_CASTLE_ATTACK_MARK_2		,
	BUFFTYPE_CASTLE_ATTACK_MARK_3		,
	BUFFTYPE_INVISABLE					,
	BUFFTYPE_DARKLORD_CASTLE_SKILL		,
	BUFFTYPE_CASTLE_CROWN_STATE			,
	BUFFTYPE_CRYWOLF_CONTRACT_ENABLE	,
	BUFFTYPE_CRYWOLF_CONTRACT_DISABLE	,
	BUFFTYPE_CRYWOLF_CONTRACT			,
	BUFFTYPE_CRYWOLF_CONTRACT_TRY		,
	BUFFTYPE_CRYWOLF_OCCUPATION			,
	BUFFTYPE_CRYWOLF_CONTRACT_HERO		,
	BUFFTYPE_CRYWOLF_NPC_INVISABLE		,
	BUFFTYPE_GM_MARK					,
	BUFFTYPE_PCBANG_POINT_MARK1			,			// 삭제
	BUFFTYPE_PCBANG_POINT_MARK2			,			// 삭제
	BUFFTYPE_PCBANG_POINT_MARK3			,			// 삭제
	BUFFTYPE_TEMPLE_QUICKNESS			,
	BUFFTYPE_TEMPLE_SUBLIMATION			,
	BUFFTYPE_TEMPLE_PROTECTION			,
	BUFFTYPE_HALLOWEEN_BLESS			,
	BUFFTYPE_HALLOWEEN_ANGER			,
	BUFFTYPE_HALLOWEEN_CRY				,
	BUFFTYPE_HALLOWEEN_FOOD				,
	BUFFTYPE_HALLOWEEN_DRINK			,
	BUFFTYPE_PCS_MARK1					,
	BUFFTYPE_PCS_MARK2					,
	BUFFTYPE_PCS_MARK3					,
	BUFFTYPE_PCS_MARK4					,
	BUFFTYPE_PCS_SCROLL_HASTE			,
	BUFFTYPE_PCS_SCROLL_DEFENSE			,
	BUFFTYPE_PCS_SCROLL_ANGER			,
	BUFFTYPE_PCS_SCROLL_MAGIC			,
	BUFFTYPE_PCS_SCROLL_HEALTH			,
	BUFFTYPE_PCS_SCROLL_MANA			,
	BUFFTYPE_PCS_MEDICINE_STRENGTH		,
	BUFFTYPE_PCS_MEDICINE_DEXTERITY		,
	BUFFTYPE_PCS_MEDICINE_VITALITY		,
	BUFFTYPE_PCS_MEDICINE_ENERGY		,
	BUFFTYPE_PCS_MEDICINE_LEADERSHIP	,
	BUFFTYPE_POISON						,
	BUFFTYPE_FREEZE						,
	BUFFTYPE_STONE						,
	BUFFTYPE_DEFENSE_POWER_DEC			,
	BUFFTYPE_ATTACK_POWER_DEC			,
	BUFFTYPE_MAGIC_POWER_DEC			,
	BUFFTYPE_STUN						,
	BUFFTYPE_MONSTER_MAGIC_DEFENSE		,
	BUFFTYPE_MONSTER_MAGIC_IMMUNE		,
	BUFFTYPE_MONSTER_MELEE_IMMUNE		,
	BUFFTYPE_RESTRICTION				,
	BUFFTYPE_CRYWOLF_STATUE_LV1			,
	BUFFTYPE_CRYWOLF_STATUE_LV2			,
	BUFFTYPE_CRYWOLF_STATUE_LV3			,
	BUFFTYPE_CRYWOLF_STATUE_LV4			,
	BUFFTYPE_CRYWOLF_STATUE_LV5			,
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	BUFFTYPE_DAMAGE_REFLECT				,
	BUFFTYPE_SLEEP						,
	BUFFTYPE_BLIND						,
	BUFFTYPE_BLOODDOT					,	// 출혈
	BUFFTYPE_FIREDOT					,	// 화이어 돗트 데미지
#endif	
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
	BUFFTYPE_CURSED_ATTACK_DOWN			,	// 소환술사 공격력 감소_저주
	BUFFTYPE_CURSED_DEFENSE_DOWN		,	// 소환술사 방어력 감소_저주
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111	
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	BUFFTYPE_CHERRYBLOSSOM_DRINK		,	// 벚꽃 술
	BUFFTYPE_CHERRYBLOSSOM_DUMPLING		,	// 벚꽃놀이 경단
	BUFFTYPE_CHERRYBLOSSOM_PETAL		,	// 벚꽃 꽃잎
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312	

#ifdef SEASON4_PATCH_GROOVING
	// 시즌 4.0 추가 스킬  grooving 2008/04/24 : 버서커
	BUFFTYPE_BERSERKER					,	// 81 버서커
#endif
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 신규스킬 버프
	BUFFTYPE_MAGIC_POWER_INC			,	// 마력 증대
	BUFFTYPE_FLAMESTRKE					,	// 플레임 스트라이크
	BUFFTYPE_GIGANTICSTORM				,	// 기간틱 스톰
#endif	// ADD_SEASON4_NEW_SKILL_20080522
#ifdef SEASON4_PATCH_GROOVING
	// 시즌 4.0 추가 스킬  grooving 2008/04/24 : 버서커
	BUFFTYPE_LIGHTNINGSHOCK				,	// 85 라이트닝 쇼크
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 신규스킬 버프(추위)
	BUFFTYPE_COLD						,	// 86 추위
#endif	// ADD_SEASON4_NEW_SKILL_20080522	
#endif

#ifdef ADD_CASHITEM_4RD_MARK_20080723
	BUFFTYPE_PCS_MARK5 = MASK_BUFFTYPE_FOREIGN4ND,	// 치유의 인장
	BUFFTYPE_PCS_MARK6					,			// 신성의 인장
#endif // ADD_CASHITEM_4RD_MARK_20080723
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
	BUFFTYPE_PCS_SCROLL_CRITICAL		,			// 전투의 스크롤
	BUFFTYPE_PCS_SCROLL_EXCELLENT		,			// 강화의 스크롤
#endif // ADD_CASHITEM_4RD_SCROLL_20080723	
	//------------->
	//20081013 해외 크리스마스 이벤트 - 버프(grooving)
#ifdef UPDATE_XMASEVENT_MAP_20080930
	BUFFTYPE_CHRISTMAS_BLESS = MASK_BUFFTYPE_SANTAEVENT,	//크리스마스의축복
	BUFFTYPE_SANTA_HEAL					,					//산타의치유
	BUFFTYPE_SANTA_PROTECT				,					//산타의보호
	BUFFTYPE_SANTA_POWER				,					//산타의강화
	BUFFTYPE_SANTA_GUARD				,					//산타의방어
	BUFFTYPE_SANTA_SPEED				,					//산타의신속
	BUFFTYPE_SANTA_LUCKY				,					//산타의행운
#endif //UPDATE_XMASEVENT_MAP_20080930
//<-------------

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
BUFFTYPE_CHARM_GUARDIAN = 99,							// 수호의 부적
BUFFTYPE_CHARM_PROTECTITEM,							// 아이템 보호 부적
BUFFTYPE_PCS_MARK1_FOR_MASTER,						// 상승의 인장 마스터 전용
BUFFTYPE_PCS_MARK2_FOR_MASTER,                      // 풍요의 인장 마스터 전용
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129 

#ifdef ADD_NEWPVP_PKFIELD
	BUFFTYPE_OBSERVER					= 98, 				//PVP-PK필드
	BUFFTYPE_GLORYOFGLADIATOR			= 103,				//검투사의 명예
#endif // ADD_NEWPVP_PKFIELD
	
#ifdef ADD_PCBANG_BUFF_MARK4
	BUFFTYPE_PCBANG_POINT_MARK4			= 104,				// 강함의 인장
#endif // ADD_PCBANG_BUFF_MARK4	
	
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
	BUFFTYPE_PARTY_EXP_INCREASE_SCROLL = 112,				// 파티 경험치 증가 주문서
	BUFFTYPE_MAX_AG_INCREASE_AURA,							// 최대 AG 증가 오라
	BUFFTYPE_MAX_SD_INCREASE_AURA,							// 최대 SD 증가 오라
#endif // ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
	
	BUFFTYPE_END
};

enum eBuffClearType
{
	CLEAR_TYPE_DIEREGEN				= 1,
	CLEAR_TYPE_LOGOUT				= 2,
	CLEAR_TYPE_MONSTERREGEN			= 3,
	CLEAR_TYPE_CHAOSCASTLE_ENTER	= 4,
	CLEAR_TYPE_PCBANG_SEAL_FINISH	= 5,
	CLEAR_TYPE_NON_PCS_ITEM_EFFECT	= 6,
	CLEAR_TYPE_CASTLESIEGE			= 7,
};

enum eBuffStateType
{
	BUFFSTATE_BUFF_DELETE			= 0,
	BUFFSTATE_BUFF_ADD				= 1,
	BUFFSTATE_BUFF_NEW				= 2,
	BUFFSTATE_BUFF_CLEAR			= 3,
};

typedef struct
{
	BYTE	BuffIndex;			// 버프 효과 인덱스 (eBuffEffectIndex 참조)
	BYTE	EffectCategory;		// 버프 효과 카테고리
	BYTE	EffectType1;		// 버프 효과 타입1
	BYTE	EffectType2;		// 버프 효과 타입2
	int		EffectValue1;		// 해당 효과 1에 따라 적용되는 수치
	int		EffectValue2;		// 해당 효과 2에 따라 적용되는 수치
	DWORD	EffectSetTime;		// 효과 사용 시간
	int		EffectDuration;		// 지속시간
}EFFECTLIST, *LPEFFECTLIST;