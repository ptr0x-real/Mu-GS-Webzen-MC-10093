#ifndef __NPCTALK_H__
#define __NPCTALK_H__

#define NPC_BATTLEANNOUNCER	239
#define NPC_GUILDMASTER		241
#define NPC_FASI			254


#define NPC_ROLENSIAGUARD	249		// 로랜시아 경비병
#define NPC_CHAOSGOBLELIN	238		// 카오스 고블린
#define NPC_RUSIPHER		237		// 루시퍼

#define NPC_POSIONGIRL		253		// 물약파는 소녀
#define NPC_DEVIASWIZARD	245		// 데비아스 마법사
#define NPC_DEVIASGUARD		247		// 데비아스 경비병
#define NPC_DEVIASWAREHOUSEMEN	240		// 데비아스&&노리아 창고지기

#define NPC_NORIARARA	242		// 노리아 라라요정

#define NPC_DEVIASMADAM	244		// 데비아스 술집마담

#define NPC_EVETNCHIPNPC	236		// 이벤트칩 NPC
#define NPC_SERVERDIVISION	234		// 서버분할 NPC

#ifdef JAPAN_NPC_NEWTALK
#define NPC_RELENSIASMITH	251		// 로랜시아 대장장이
#define NPC_NORIAJANGIN		243		// 노리아 장인
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
#define NPC_COUPONNPC		258
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef SECOND_QUEST
#define NPC_QEUSTNPC2		229		// 퀘스트 NPC2(나중에 이름 바꿔야됨)
#endif


#ifdef DARKLORD_WORK
#define NPC_DARKSPRIT_TRAINER		226		// 다크스피릿 조련사
#endif


#ifdef FOR_BLOODCASTLE
#define NPC_ANGELKING		232		// 대천사
#define NPC_ANGELMESSANGER	233		// 대천사 전령
#define NPC_ROLENSIAMADAM	255		// 로랜시아 대장장이
#define NPC_ROADMERCHANT	250		// 떠돌이 상인
#define NPC_DEVIASWEAPON	246		// 데비아스 무기상인
#define NPC_ROLENSIASMITH	251		// 로랜시아 대장장이
#define NPC_NORIAJANGIN		243		// 노리아 장인
#endif

#ifdef JEWEL_MIX_SYSTEM_20050309
#define NPC_JEWEL_MIX_DEALER	256			// 보석 환전상
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
#define NPC_CASTLE_HUNTZONE_GUARD	220		// 사냥터 문지기
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#define NPC_SIEGE_MACHINE_ATTACK	221		// 공성무기 - 공격
#define NPC_SIEGE_MACHINE_DEFENSE	222		// 공성무기 - 방어
#endif

#ifdef ADD_NEW_NPC_CASTLE_20041115
#define NPC_CASTLE_CROWN			216		// 왕관
#define NPC_CASTLE_SWITCH1			217		// 왕관스위치1
#define NPC_CASTLE_SWITCH2			218		// 왕관스위치2
#define NPC_CASTLE_GATE_LEVER		219		// 성문레버
#define NPC_ELDER_CIRCLE			223		// 원로원
#define NPC_CASTLE_GUARD			224		// 경비병
#endif

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
#define NPC_SHADOW_PHANTOM          257
#endif

#ifdef ADD_SHOP_KALIMA_NPC_REIRA_20060403
#define NPC_KALIMA_REIRA			259
//#define NPC_KALIMA_REIRA_MSGBOX		14
#endif

#ifdef ADD_NEW_NPC_KANTURU_20060612
#define NPC_KANTURU_MACHINE			367
#endif

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// NPC 인덱스 설정
#define NPC_WHITEANGEL_EVENT			371
#define NPC_WHITEANGEL_EVENT_MSGBOX		14
#endif

#define NPC_ROLENSIAGUARD_MSGBOX	0
#define NPC_RUSIPHER_MSGBOX			2		// 루시퍼
#define NPC_FASI_MSGBOX				6		// 카오스 고블린
#define NPC_CHAOSGOBLELIN_MSGBOX	9		// 카오스 고블린

#define NPC_POSIONGIRL_MSGBOX		5		// 물약파는 소녀
#define NPC_DEVIASWIZARD_MSGBOX		8		// 데비아스 마법사

#define NPC_DEVIASGUARD_MSGBOX		4		// 데비아스 경비병
#define NPC_DEVIASWAREHOUSEMEN_MSGBOX		7		// 데비아스 창고지기
#define NPC_NORIARARA_MSGBOX		11		// 노리아 라라요정
#define NPC_DEVIASMADAM_MSGBOX		12		// 데비아스 술집마담

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
#define NPC_SHADOW_PHANTOM_MSGBOX   13      // 쉐도우 팬텀
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
#define NPC_COUPONNPC_MSGBOX		7		// 쿠폰 아이템 발급 NPC
#endif

#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
#define MSGBOX_ENTERLIMIT_PKUSER_DEVILSQURE		55	// 살인마 데빌스퀘어 입장 금지
#define MSGBOX_ENTERLIMIT_PKUSER_BLOODCASTLE	56	// 살인마 블러드캐슬 입장 금지
#define MSGBOX_ENTERLIMIT_PKUSER_CHAOSCASTLE	57	// 살인마 카오스캐슬 입장 금지
#endif

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
#define NPC_CHAOSCARDMASTER			375		// 복권 아이템 조합 - 카오스 카드 마스터
#define NPC_CHAOSCARDMASTER_MSGBOX	15
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// 물자보급관 파멜라, 안젤라 추가(goni)
	#define NPC_QUARTERMASTER_PAMELLA	376		// 물자 보급관 파멜라
	#define NPC_QUARTERMASTER_ANGELA	377		// 물자 보급관 파멜라
#endif

#ifdef ADD_FIRECRACKER_NPC_20070306			// 폭죽판매 NPC
#define NPC_FRIECRACKER_STORE			379		// 폭죽 판매 NPC
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3차 전직관련 NPC
#define NPC_PRIEST_DEVIN				406		// 사제데빈
#define NPC_WERWOLF_QUERELLE			407		// 웨어울프 쿼렐
#define NPC_THIRD_CHANGEUP_BOSS_GATE	408		// 3차 체인지업 보스맵 성문
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// 아이템 지급 NPC
#define NPC_ALAN					414		// 도우미 헬퍼 앨런
#define NPC_ALANS_MSGBOX			15		// 도우미 앨런 메세지박스
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// 엘베란드 NPC
#define NPC_SILVIA						415		// 물약상점 실비아
#define NPC_RHEA						416		// 중급무기 및 바어구 상점
#define NPC_MARCE						417		// 마법 상점 마르세
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
#define	NPC_CHERRY						450
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef MODIFY_CHEREEY_NPC_REGISTER_NPC_GROUP_20090803
#define	NPC_CHERRY_TREE					451
#endif // MODIFY_CHEREEY_NPC_REGISTER_NPC_GROUP_20090803

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
#define NPC_SEED_MASTER					452
#define NPC_SEED_RESEARCHER				453
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
#define NPC_STAT_RESET					464		//시즌 4 체험서버 스탯초기화 NPC
#endif // EXPERIENCE_SERVER_STAT_RESET_NPC

//------------->
//20081013 해외 크리스마스 이벤트 - NPC(grooving)
#ifdef ADD_NPC_XMASEVENT_20080930
#define NPC_SANTA						465		//산타클로스
#define NPC_SNOW_MAN					467		//눈사람
#define NPC_LITTLE_SANTA_1				468		//리틀산타1
#define NPC_LITTLE_SANTA_2				469		//리틀산타2
#define NPC_LITTLE_SANTA_3				470		//리틀산타3
#define NPC_LITTLE_SANTA_4				471		//리틀산타4
#define NPC_LITTLE_SANTA_5				472		//리틀산타5
#define NPC_LITTLE_SANTA_6				473		//리틀산타6
#define NPC_LITTLE_SANTA_7				474		//리틀산타7
#define NPC_LITTLE_SANTA_8				475		//리틀산타8
#define NPC_SANTA_MSGBOX				16      //산타 메시지 박스    by natinda
#define NPC_SNOWMAN_MSGBOX				17      //눈사람 메시지 박스 by natinda
#endif //ADD_NPC_XMASEVENT_20080930
//<-------------

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
#define NPC_DELGADO						478		// 행운의 동전 등록 NPC	
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_NEWPVP_PKFIELD
#define NPC_TITUS						479		// PK필드 문지기
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_GAMBLING_20090120
#define NPC_GAMBLER_1					492		// 겜블링 상점 NPC 1
#define NPC_GAMBLER_2					493		// 겜블링 상점 NPC 2
#endif // ADD_GAMBLING_20090120

extern BOOL NpcTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);



#define MAX_TALKCHAT	10

class CTalkTableClass
{
	char Talk[MAX_TALKCHAT][MAX_CHAT+1];
	int  Delay[MAX_TALKCHAT];

public :
	BOOL Load(char *filename);
};


#endif

/*
// NPC
241 1 "길드마스터"       2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
242 1 "요정마법사"       2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
243 1 "요정상인"         2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
244 1 "겨울술집상인"     2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
245 1 "겨울마법사"       2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
246 1 "겨울무기상인"    20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
247 1 "겨울경비병"      50 10000  0 100 120  70   0   50 100		  0  0 0 3 400  1000  10  1 200 10 0	0 0 0 0
248 1 "떠돌이상인"      20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
249 1 "경비병"          50 10000  0 100 120  70   0   50 100		  2  0 2 3 400  1000  10  1 200 10 0	0 0 0 0
250 1 "떠돌이상인"      20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
251 1 "한스"            20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
252 1 "--------"         2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0 	0 0 0 0
253 1 "꼬마"             2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
254 1 "마법사 파시"      2    50  0  15  30  70  20   50  30	   	  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
255 1 "리아먼"           2    50  0  15  30  70  20   50  30     	  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
end

*/