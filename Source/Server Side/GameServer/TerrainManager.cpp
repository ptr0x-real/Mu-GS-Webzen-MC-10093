#include "stdafx.h"
#include "TerrainManager.h"

#include "..\include\Readscript.h"

CTerrainManager g_TerrainManager;

void CTerrainManager::Load(const char *filename)
{
	SMDToken Token;

	SMDFile = fopen(filename, "r");

	if (SMDFile == NULL) return;

	this->m_iSize = 0;

	_TERRAIN_INFO terrainInfo;

	//std::map<int, _TERRAIN_INFO>

	while (true)
	{
		Token = GetToken();

		if (Token == END)
			break;
		else if (Token == NUMBER)
		{
			terrainInfo.iTerrainID = (int)TokenNumber;

			std::map<int, _TERRAIN_INFO>::iterator Iter;

			Iter = this->m_mTerrainMap.find(terrainInfo.iTerrainID);

			// DEVGAMESX ERROR
			//std::map<int, _TERRAIN_INFO>::iterator it = this->m_mTerrainMap.find(terrainInfo.iTerrainID); 

			//if(this->m_mTerrainMap.find(terrainInfo.iTerrainID) == this->m_mTerrainMap.end())
			if (Iter == this->m_mTerrainMap.end())
			{
				Token = GetToken();
				//strcpy(terrainInfo.szTerrainName, (const char*)24);//, TokenString);
				strcpy(terrainInfo.szTerrainName, TokenString);

				Token = GetToken();
				//strcpy(terrainInfo.szMapName, (const char*)24);//, TokenString);
				strcpy(terrainInfo.szMapName, TokenString);

				Token = GetToken();
				terrainInfo.iMinLevel = TokenNumber;

				this->m_mTerrainMap.insert(std::pair<int, _TERRAIN_INFO>(terrainInfo.iTerrainID, terrainInfo));

				if (terrainInfo.iTerrainID + 1 > this->m_iSize)
					this->m_iSize = terrainInfo.iTerrainID + 1;
			}
		}
	};

	this->m_bMapCheck = new bool[this->m_iSize];

	for (int i = 0; i < this->m_iSize; ++i)
		this->m_bMapCheck[i] = (this->m_mTerrainMap.find(i) != this->m_mTerrainMap.end());

	fclose(SMDFile);
}