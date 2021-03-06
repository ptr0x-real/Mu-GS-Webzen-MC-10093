// BuffEffect.h: interface for the CBuffEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFEFFECT_H__ADB0C9F9_3AD3_4F0D_A187_00CCEFA18435__INCLUDED_)
#define AFX_BUFFEFFECT_H__ADB0C9F9_3AD3_4F0D_A187_00CCEFA18435__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917

enum EFFECTTYPE_DEFINE
{
	EFFECTTYPE_NONE						= 0,
	EFFECTTYPE_ATTACKSPEED				= 1,
	EFFECTTYPE_IMPROVE_DAMAGE			= 2,
	EFFECTTYPE_IMPROVE_DEFENSE			= 3,
	EFFECTTYPE_HP						= 4,
	EFFECTTYPE_MANA						= 5,
	EFFECTTYPE_EXPERIENCE				= 6,
	EFFECTTYPE_ITEMDROPRATE				= 7,
	EFFECTTYPE_EXPERIENCE_ZERO			= 8,
	EFFECTTYPE_STRENGTH					= 9,
	EFFECTTYPE_DEXTERITY				= 10,
	EFFECTTYPE_VITALITY					= 11,
	EFFECTTYPE_ENERGY					= 12,
	EFFECTTYPE_LEADERSHIP				= 13,
	EFFECTTYPE_MELEEDAMAGE				= 14,
	EFFECTTYPE_MAGICDAMAGE				= 15,
	EFFECTTYPE_MAPMOVEFREE				= 16,
	EFFECTTYPE_CRITICAL_DMG				= 17,
	EFFECTTYPE_GIVE_DMG_TICK			= 18,
	EFFECTTYPE_POISON_DMG_TICK			= 19,
	EFFECTTYPE_REDUCE_MOVE_SPEED		= 20,
	EFFECTTYPE_IMPROVE_MELEE_DEFENSE	= 21,
	EFFECTTYPE_IMPROVE_MAGIC_DEFENSE	= 22,
	EFFECTTYPE_DECREASE_DEFENSE			= 23,
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	EFFECTTYPE_DAMAGEREFLECT			= 24,
	EFFECTTYPE_SLEEP					= 25,
	EFFECTTYPE_BLIND					= 26,
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	EFFECTTYPE_CURSED_DAMAGE			= 27,
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 소울바리어 : 마나 소모 수치 추가
	EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA	= 28,
#endif	
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
	EFFECTTYPE_CURSED_ATTACKDOWN	= 29,	// 위크니스_공격력 감소
	EFFECTTYPE_CURSED_DEFENSEDOWN	= 30,	// 이너베이션_방어력 감소
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111	
#ifdef SEASON4_PATCH_GROOVING
	EFFECTTYPE_BERSERKER_UP			= 31,	// 버서커 저주력,마력,물리공격력,최대mp 상승
	EFFECTTYPE_BERSERKER_DOWN		= 32,	// 버서커 방어력,최대생명 하락
#endif
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 신규스킬 이펙트 타입
	EFFECTTYPE_MAGICPOWER_INC		= 33,	// 마력 증가
#endif	// ADD_SEASON4_NEW_SKILL_20080522	

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_04_20080723
	EFFECTTYPE_IMPROVE_DAMAGE_PER		= 51,	// 공격력 +%d% (일반 공격력/마력)
	EFFECTTYPE_ABSORB_DAMAGE			= 52,	// 몬스터로받는 데미지 흡수
	EFFECTTYPE_RECOVERY_HP				= 53,	// 생명자동회복 d%
	EFFECTTYPE_RECOVERY_MANA			= 54,	// 마나자동회복 d%
	EFFECTTYPE_CRITICALDAMAGE			= 55,	// 크리티컬 데미지 d%
	EFFECTTYPE_EXCELLENTDAMAGE			= 56,	// 엑설런트 데미지 d%
#endif // ADD_PARTIALY_CHARGE_SYSTEM_04_20080723
#ifdef UPDATE_XMASEVENT_MAP_20080930
	EFFECTTYPE_POWER_UP					= 57,	//산타의강화
	EFFECTTYPE_GUARD_UP					= 58,

	//삭제예정
	//EFFECTTYPE_MAXLIFE_UP				= 59,
	//EFFECTTYPE_MAXMANA_UP				= 60,
	//EFFECTTYPE_AG_RECOVERY_UP			= 61,

	EFFECTTYPE_AG_RECOVERY_UP			= 59,
#endif
	
#ifdef ADD_NEWPVP_PKFIELD
	EFFECTTYPE_VULCANUS_EXPERIENCE		= 60, 
	EFFECTTYPE_VULCANUS_ITEMDROPRATE	= 61, 
#endif // ADD_NEWPVP_PKFIELD
	
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	EFFECTTYPE_GUARD_EXPNITEM			= 62,      // 경험치와 아이템이 사망해도 그대로 유지 된다
	EFFECTTYPE_PROTECT_ITEM				= 63,      // 아이템 내구도가 일정 기간만큼 감소하지 않는다.
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	
#ifdef ADD_PREMIUMITEM_PANDA
	EFFECTTYPE_ZEN_UP					= 64, 
#endif // ADD_PREMIUMITEM_PANDA
	
#ifdef ADD_AURA_CASH_ITEM_20090915
	EFFECTTYPE_MAX_AG_INCREASE			= 66,		// AG 최대값 상승
	EFFECTTYPE_MAX_SD_INCREASE			= 67,		// SD 최대값 상승
	EFFECTTYPE_PARTY_EXP_INCREASE		= 68,		// SD 최대값 상승
#endif // ADD_AURA_CASH_ITEM_20090915
};

class CBuffEffect  
{
public:
	CBuffEffect();
	virtual ~CBuffEffect();

public:
	void SetBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue);			// 스테이터스 증감과 관련된 버프 효과 적용
	void ClearBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue);		// 스테이터스 증감과 관련된 버프 효과 삭제
	void SetActiveBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue);	// 틱 단위로 적용되는 버프 효과 적용 (DOT 등.)
	
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
	void SetPrevEffect(LPOBJECTSTRUCT lpObj);
	void SetNextEffect(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
	
#ifdef MODIFY_INCREASE_LIFE_AND_SETITEM_BUGFIX_20080829
	void ClearAllBuffEffect( LPOBJECTSTRUCT lpObj );
#endif // MODIFY_INCREASE_LIFE_AND_SETITEM_BUGFIX_20080829

	// 각 버프 효과에 따른 적용 (계산 공식 등 적용)
	void GiveDamageEffect(LPOBJECTSTRUCT lpObj, int Damage);
	// 독은 다른 계산 공식이 들어가기 때문에 따로 구현
	void PoisonEffect(LPOBJECTSTRUCT lpObj, BYTE PoisonRate);
};

extern CBuffEffect g_BuffEffect;

#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#endif // !defined(AFX_BUFFEFFECT_H__ADB0C9F9_3AD3_4F0D_A187_00CCEFA18435__INCLUDED_)
