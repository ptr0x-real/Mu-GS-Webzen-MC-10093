#include "StdAfx.h"
#include "AntiHack.h"

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409

_vtIP_BLOCK				vt_IPBlock;
_vtUSER_CONNECT_TIME	vtUserConnectTime;

void SetConnectUser(LPUSER_CONNECT_TIME pUserConnectTime)
{
	if( vtUserConnectTime.size() < 200 )
	{
		if( ChkUserConnectInterval(pUserConnectTime) == true )
		{
			vtUserConnectTime.push_back(*pUserConnectTime);
		}
		else
		{
			IP_BLOCK pIpBlock;
			memcpy(pIpBlock.szAccount, pUserConnectTime->szAccount, MAX_IDSTRING);
			memcpy(pIpBlock.Ip_addr, pUserConnectTime->Ip_addr, 15);
			pIpBlock.dwBlockTime = GetTickCount();

			setBlockIP(&pIpBlock);

			LogAddTD_TempLog(", [Login User Block], Account [%s], IP [%s]", pUserConnectTime->szAccount, pUserConnectTime->Ip_addr);

			CloseClient (pUserConnectTime->iIndex);
		}	
	}
	else
	{
		LogAddTD_TempLog(", [connect Time] Size Max");
		
		vtUserConnectTime.erase(vtUserConnectTime.begin(), vtUserConnectTime.end());
	}
}

bool ChkUserConnectInterval(LPUSER_CONNECT_TIME pUserConnectTime)
{
	_IterUSER_CONNECT_TIME iter;

	for (iter = vtUserConnectTime.begin(); iter != vtUserConnectTime.end(); ++iter)
	{
		USER_CONNECT_TIME& UserConnectTime = *iter;

		if ( strcmp(UserConnectTime.Ip_addr, pUserConnectTime->Ip_addr) == 0 )
		{
			DWORD dwTickCount = GetTickCount();

			if( (pUserConnectTime->dwConnectTime - UserConnectTime.dwConnectTime) < (1000 * gConnectIntervalTime) )
			{
				vtUserConnectTime.erase(iter);
			
				//LogAddTD_TempLog("[connect Hack Attack - ChkUserConnectInterval] IP [%s], UserConnectCnt [%d] ", pUserConnectTime->Ip_addr, vtUserConnectTime.size());

				return false;
			}
			else
			{
				vtUserConnectTime.erase(iter);

				//LogAddTD_TempLog("[connect Hack Attack - Ok] IP [%s], UserConnectCnt [%d] ", pUserConnectTime->Ip_addr, vtUserConnectTime.size());

				return true;
			}
		}
	}
	
	return true;
}

void setBlockIP(LPIP_BLOCK pIpBlock)
{
	if( vt_IPBlock.size() < 200 )
	{
		vt_IPBlock.push_back(*pIpBlock);

		//LogAddTD_TempLog("[connect Hack Attack - SetBlockIP] Account [%s] IP [%s]", pIpBlock->szAccount, pIpBlock->Ip_addr);
	}
	else
	{
		LogAddTD_TempLog(", [BlockIP] Size Max");

		vt_IPBlock.erase(vt_IPBlock.begin(), vt_IPBlock.end());
	}

	//LogAddTD_TempLog("[connect Hack Attack - SetBlockIP] BlockCnt [%d]", vt_IPBlock.size());
}

bool ChkBlockIp(LPIP_BLOCK pIpBlock)
{
	_IterIP_BLOCK iter;
	
	for (iter = vt_IPBlock.begin(); iter != vt_IPBlock.end(); ++iter)
	{
		IP_BLOCK& ipBlock = *iter;
		if ( strcmp(ipBlock.Ip_addr, pIpBlock->Ip_addr) == 0 )
		{
			DWORD dwTickCount = GetTickCount();
			
			if( (dwTickCount - ipBlock.dwBlockTime) > ((1000 * 60) * gConnectHackIPBlockTime) )
			{
				vt_IPBlock.erase(iter);
				
				return false;
			}
			else
			{
				int fTime = (dwTickCount - ipBlock.dwBlockTime) / 1000;
				int	iSec = 0;
				int	iMin = 0;

				iMin = fTime / 60;

				if( iMin > 0)
				{
					iSec = fTime % 60;
				}
				else
				{
					iSec = fTime;
				}

				if( gConnectRenewBlockTime == 1 )
				{
					//  블럭 시간을 갱신해 준다.
					ipBlock.dwBlockTime = dwTickCount;
				}

				//연결을 끊어라.
				LogAddTD_TempLog(", [BlockIP Disconnect], , IP [%s], ReConTime %02d:%02d", ipBlock.Ip_addr, iMin , iSec);

				return true;
			}
		}
	}
	
	return false;
}

#endif //MODIFY_CONNNECT_HACK_ATTACK_20090409
