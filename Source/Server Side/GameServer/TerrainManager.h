#if ENABLE_MC_CODE == 1

#ifndef __TERRAINMANAGER_H__
#define __TERRAINMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct _TERRAIN_INFO
{
	int iTerrainID;
	int iMinLevel;
	char szTerrainName[24];
	char szMapName[24];
};

class CTerrainManager
{
public:
	void Load(const char *filename = "..\\data\\TerrainManager.cfg");

	const int Size()
	{
		return m_iSize;
	}

	int GetMinLevel(int MapNumber)
	{
		std::map<int, _TERRAIN_INFO>::iterator it = this->m_mTerrainMap.find(MapNumber);

		if(it == this->m_mTerrainMap.end())
			return 0;
		
		return it->second.iMinLevel;
	}

	char *GetTerrainName(int MapNumber)
	{
		std::map<int, _TERRAIN_INFO>::iterator it = this->m_mTerrainMap.find(MapNumber);

		if(it == this->m_mTerrainMap.end())
			return NULL;

		return it->second.szTerrainName;
	}

	char *GetMapName(int MapNumber)
	{
		std::map<int, _TERRAIN_INFO>::iterator it = this->m_mTerrainMap.find(MapNumber);

		if(it == this->m_mTerrainMap.end())
			return "Unknown";

		return it->second.szMapName;
	}

	bool CheckTerrain(int MapNumber)
	{
		return this->m_bMapCheck[MapNumber];
	}

	std::map<int, _TERRAIN_INFO> m_mTerrainMap;

private:
	int					 m_iSize;
	bool				*m_bMapCheck;
};

extern CTerrainManager g_TerrainManager;

#endif // __TERRAINMANAGER_H__
#endif // ENABLE_MC_CODE == 1