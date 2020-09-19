#ifndef __NPCTALK_H__
#define __NPCTALK_H__

#define NPC_BATTLEANNOUNCER	239
#define NPC_GUILDMASTER		241
#define NPC_FASI			254


#define NPC_ROLENSIAGUARD	249		// �η��þ� ���
#define NPC_CHAOSGOBLELIN	238		// ī���� ���
#define NPC_RUSIPHER		237		// �����

#define NPC_POSIONGIRL		253		// �����Ĵ� �ҳ�
#define NPC_DEVIASWIZARD	245		// ����ƽ� ������
#define NPC_DEVIASGUARD		247		// ����ƽ� ���
#define NPC_DEVIASWAREHOUSEMEN	240		// ����ƽ�&&�븮�� â������

#define NPC_NORIARARA	242		// �븮�� ������

#define NPC_DEVIASMADAM	244		// ����ƽ� ��������

#define NPC_EVETNCHIPNPC	236		// �̺�ƮĨ NPC
#define NPC_SERVERDIVISION	234		// �������� NPC

#ifdef JAPAN_NPC_NEWTALK
#define NPC_RELENSIASMITH	251		// �η��þ� ��������
#define NPC_NORIAJANGIN		243		// �븮�� ����
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
#define NPC_COUPONNPC		258
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef SECOND_QUEST
#define NPC_QEUSTNPC2		229		// ����Ʈ NPC2(���߿� �̸� �ٲ�ߵ�)
#endif


#ifdef DARKLORD_WORK
#define NPC_DARKSPRIT_TRAINER		226		// ��ũ���Ǹ� ���û�
#endif


#ifdef FOR_BLOODCASTLE
#define NPC_ANGELKING		232		// ��õ��
#define NPC_ANGELMESSANGER	233		// ��õ�� ����
#define NPC_ROLENSIAMADAM	255		// �η��þ� ��������
#define NPC_ROADMERCHANT	250		// ������ ����
#define NPC_DEVIASWEAPON	246		// ����ƽ� �������
#define NPC_ROLENSIASMITH	251		// �η��þ� ��������
#define NPC_NORIAJANGIN		243		// �븮�� ����
#endif

#ifdef JEWEL_MIX_SYSTEM_20050309
#define NPC_JEWEL_MIX_DEALER	256			// ���� ȯ����
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
#define NPC_CASTLE_HUNTZONE_GUARD	220		// ����� ������
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#define NPC_SIEGE_MACHINE_ATTACK	221		// �������� - ����
#define NPC_SIEGE_MACHINE_DEFENSE	222		// �������� - ���
#endif

#ifdef ADD_NEW_NPC_CASTLE_20041115
#define NPC_CASTLE_CROWN			216		// �հ�
#define NPC_CASTLE_SWITCH1			217		// �հ�����ġ1
#define NPC_CASTLE_SWITCH2			218		// �հ�����ġ2
#define NPC_CASTLE_GATE_LEVER		219		// ��������
#define NPC_ELDER_CIRCLE			223		// ���ο�
#define NPC_CASTLE_GUARD			224		// ���
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

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// NPC �ε��� ����
#define NPC_WHITEANGEL_EVENT			371
#define NPC_WHITEANGEL_EVENT_MSGBOX		14
#endif

#define NPC_ROLENSIAGUARD_MSGBOX	0
#define NPC_RUSIPHER_MSGBOX			2		// �����
#define NPC_FASI_MSGBOX				6		// ī���� ���
#define NPC_CHAOSGOBLELIN_MSGBOX	9		// ī���� ���

#define NPC_POSIONGIRL_MSGBOX		5		// �����Ĵ� �ҳ�
#define NPC_DEVIASWIZARD_MSGBOX		8		// ����ƽ� ������

#define NPC_DEVIASGUARD_MSGBOX		4		// ����ƽ� ���
#define NPC_DEVIASWAREHOUSEMEN_MSGBOX		7		// ����ƽ� â������
#define NPC_NORIARARA_MSGBOX		11		// �븮�� ������
#define NPC_DEVIASMADAM_MSGBOX		12		// ����ƽ� ��������

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
#define NPC_SHADOW_PHANTOM_MSGBOX   13      // ������ ����
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
#define NPC_COUPONNPC_MSGBOX		7		// ���� ������ �߱� NPC
#endif

#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
#define MSGBOX_ENTERLIMIT_PKUSER_DEVILSQURE		55	// ���θ� ���������� ���� ����
#define MSGBOX_ENTERLIMIT_PKUSER_BLOODCASTLE	56	// ���θ� ����ĳ�� ���� ����
#define MSGBOX_ENTERLIMIT_PKUSER_CHAOSCASTLE	57	// ���θ� ī����ĳ�� ���� ����
#endif

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
#define NPC_CHAOSCARDMASTER			375		// ���� ������ ���� - ī���� ī�� ������
#define NPC_CHAOSCARDMASTER_MSGBOX	15
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// ���ں��ް� �ĸ��, ������ �߰�(goni)
	#define NPC_QUARTERMASTER_PAMELLA	376		// ���� ���ް� �ĸ��
	#define NPC_QUARTERMASTER_ANGELA	377		// ���� ���ް� �ĸ��
#endif

#ifdef ADD_FIRECRACKER_NPC_20070306			// �����Ǹ� NPC
#define NPC_FRIECRACKER_STORE			379		// ���� �Ǹ� NPC
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� �������� NPC
#define NPC_PRIEST_DEVIN				406		// ��������
#define NPC_WERWOLF_QUERELLE			407		// ������� ����
#define NPC_THIRD_CHANGEUP_BOSS_GATE	408		// 3�� ü������ ������ ����
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// ������ ���� NPC
#define NPC_ALAN					414		// ����� ���� �ٷ�
#define NPC_ALANS_MSGBOX			15		// ����� �ٷ� �޼����ڽ�
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� NPC
#define NPC_SILVIA						415		// ������� �Ǻ��
#define NPC_RHEA						416		// �߱޹��� �� �پ ����
#define NPC_MARCE						417		// ���� ���� ������
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
#define NPC_STAT_RESET					464		//���� 4 ü�輭�� �����ʱ�ȭ NPC
#endif // EXPERIENCE_SERVER_STAT_RESET_NPC

//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - NPC(grooving)
#ifdef ADD_NPC_XMASEVENT_20080930
#define NPC_SANTA						465		//��ŸŬ�ν�
#define NPC_SNOW_MAN					467		//�����
#define NPC_LITTLE_SANTA_1				468		//��Ʋ��Ÿ1
#define NPC_LITTLE_SANTA_2				469		//��Ʋ��Ÿ2
#define NPC_LITTLE_SANTA_3				470		//��Ʋ��Ÿ3
#define NPC_LITTLE_SANTA_4				471		//��Ʋ��Ÿ4
#define NPC_LITTLE_SANTA_5				472		//��Ʋ��Ÿ5
#define NPC_LITTLE_SANTA_6				473		//��Ʋ��Ÿ6
#define NPC_LITTLE_SANTA_7				474		//��Ʋ��Ÿ7
#define NPC_LITTLE_SANTA_8				475		//��Ʋ��Ÿ8
#define NPC_SANTA_MSGBOX				16      //��Ÿ �޽��� �ڽ�    by natinda
#define NPC_SNOWMAN_MSGBOX				17      //����� �޽��� �ڽ� by natinda
#endif //ADD_NPC_XMASEVENT_20080930
//<-------------

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
#define NPC_DELGADO						478		// ����� ���� ��� NPC	
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_NEWPVP_PKFIELD
#define NPC_TITUS						479		// PK�ʵ� ������
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_GAMBLING_20090120
#define NPC_GAMBLER_1					492		// �׺� ���� NPC 1
#define NPC_GAMBLER_2					493		// �׺� ���� NPC 2
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
241 1 "��帶����"       2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
242 1 "����������"       2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
243 1 "��������"         2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
244 1 "�ܿ��������"     2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
245 1 "�ܿ︶����"       2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
246 1 "�ܿ﹫�����"    20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
247 1 "�ܿ���"      50 10000  0 100 120  70   0   50 100		  0  0 0 3 400  1000  10  1 200 10 0	0 0 0 0
248 1 "�����̻���"      20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
249 1 "���"          50 10000  0 100 120  70   0   50 100		  2  0 2 3 400  1000  10  1 200 10 0	0 0 0 0
250 1 "�����̻���"      20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
251 1 "�ѽ�"            20  1000  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
252 1 "--------"         2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0 	0 0 0 0
253 1 "����"             2    50  0  15  30  70  20   50  30		  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
254 1 "������ �Ľ�"      2    50  0  15  30  70  20   50  30	   	  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
255 1 "���Ƹ�"           2    50  0  15  30  70  20   50  30     	  0  0 0 5 400  1500  10  0 200 10 0	0 0 0 0
end

*/