#ifndef ANTI_HACK_H
#define ANTI_HACK_H

#include "..\INCLUDE\DEFINE.H"
#include <vector>

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409

using namespace std;

typedef struct
{
	char	szAccount[MAX_IDSTRING+1];
	char	Ip_addr[16];
	DWORD	dwBlockTime;
	
}IP_BLOCK, *LPIP_BLOCK;

typedef struct
{
	DWORD	dwConnectTime;
	int		iIndex;
	char	szAccount[MAX_IDSTRING+1];
	char	Ip_addr[16];
	
}USER_CONNECT_TIME, *LPUSER_CONNECT_TIME;

typedef vector<USER_CONNECT_TIME>			_vtUSER_CONNECT_TIME;
typedef vector<USER_CONNECT_TIME>::iterator	_IterUSER_CONNECT_TIME;

extern bool ChkUserConnectInterval(LPUSER_CONNECT_TIME pUserConnectTime);
extern void SetConnectUser(LPUSER_CONNECT_TIME pUserConnectTime);

extern int  gConnectIntervalTime;

typedef vector<IP_BLOCK>			_vtIP_BLOCK;
typedef vector<IP_BLOCK>::iterator	_IterIP_BLOCK;

extern int  gConnectHackIPBlockTime;

extern bool ChkBlockIp(LPIP_BLOCK pIpBlock);
extern void setBlockIP(LPIP_BLOCK pIpBlock);

extern int	gConnectRenewBlockTime;

#endif //MODIFY_CONNNECT_HACK_ATTACK_20090409

#endif