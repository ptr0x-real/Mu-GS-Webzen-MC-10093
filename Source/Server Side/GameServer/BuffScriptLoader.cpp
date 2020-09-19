// BuffScriptLoader.cpp: implementation of the CBuffScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuffScriptLoader.h"

#include "..\include\ReadScript.h"

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917

CBuffScriptLoader g_BuffScript;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuffScriptLoader::CBuffScriptLoader()
{

}

CBuffScriptLoader::~CBuffScriptLoader()
{

}

void CBuffScriptLoader::Initialize()
{
	iBuffCount = 0;

	for( int i = 0 ; i < MAX_BUFF_EFFECT_INDEX ; i++ )
	{
		BuffData[i].BuffIndex	= -1;
		BuffData[i].BuffType	= 0;
		BuffData[i].ClearType	= 0;
		BuffData[i].NoticeType	= 0;
		BuffData[i].BuffDescript[0] = '\0';
	}
}

bool CBuffScriptLoader::Load(char* pchFileName)
{
	Initialize();

	if( ( SMDFile = fopen( pchFileName, "r" ) ) == FALSE )
	{
		return false;
	}

	SMDToken Token;
	int iType = 0;
	BYTE btBuffIndex		= 0;
	BYTE btBuffEffectType	= 0;
	BYTE ItemType			= 0;
	BYTE ItemIndex			= 0;
	BYTE btBuffType			= 0;
	BYTE btNoticeType		= 0;
	BYTE btClearType		= 0;
	char chBuffName[MAX_BUFF_NAME_LENGTH];
	char chBuffDescript[MAX_DESCRIPT_LENGTH];

	while( true )
	{
		Token = (*GetToken)();
		if( Token == END )
		{
			break;
		}

		if( Token == NUMBER )
		{
			iType = (int)TokenNumber;
			while( true )
			{
				Token = (*GetToken)();
				if( strcmp( "end", TokenString ) == NULL )
				{
					break;
				}

				btBuffIndex = (int)TokenNumber;
				Token = (*GetToken)();
				btBuffEffectType = (int)TokenNumber;
				Token = (*GetToken)();
				ItemType = (int)TokenNumber;
				Token = (*GetToken)();
				ItemIndex = (int)TokenNumber;
				Token = (*GetToken)();
				memcpy( chBuffName, TokenString, MAX_BUFF_NAME_LENGTH );
				Token = (*GetToken)();
				btBuffType = (int)TokenNumber;
				Token = (*GetToken)();
				btNoticeType = (int)TokenNumber;
				Token = (*GetToken)();
				btClearType = (int)TokenNumber;
				Token = (*GetToken)();
				memcpy( chBuffDescript, TokenString, MAX_DESCRIPT_LENGTH );

				AddBuffEffectData( btBuffIndex, btBuffEffectType, ItemType, ItemIndex, chBuffName, btBuffType, btNoticeType, btClearType, chBuffDescript );
			}
		}
	}
	
	fclose( SMDFile );

	return true;
}

bool CBuffScriptLoader::AddBuffEffectData(BYTE btBuffIndex, BYTE btBuffEffectType, BYTE btItemType, BYTE btItemIndex, char* pchName, BYTE btBuffType, BYTE btNoticeType, BYTE btClearType, char* pchDescript)
{
	BuffData[iBuffCount].BuffIndex		= btBuffIndex;
	BuffData[iBuffCount].BuffEffectType	= btBuffEffectType;
	BuffData[iBuffCount].ItemType		= btItemType;
	BuffData[iBuffCount].ItemIndex		= btItemIndex;
	BuffData[iBuffCount].BuffType		= btBuffType;
	BuffData[iBuffCount].NoticeType		= btNoticeType;
	BuffData[iBuffCount].ClearType	= btClearType;
	memcpy( BuffData[iBuffCount].BuffName, pchName, MAX_BUFF_NAME_LENGTH );
	memcpy( BuffData[iBuffCount].BuffDescript, pchDescript, MAX_DESCRIPT_LENGTH );
	iBuffCount++;

	return true;
}

bool CBuffScriptLoader::CheckVaildBuffEffect(BYTE btBuffIndex)
{
	if( btBuffIndex < 0 || btBuffIndex > MAX_BUFF_EFFECT_INDEX )
	{
		return false;
	}

	return true;
}

LPBUFF_EFFECT_DATA CBuffScriptLoader::GetBuffData(BYTE btBuffIndex)
{
	LPBUFF_EFFECT_DATA lpBuffData = NULL;

	if( CheckVaildBuffEffect( btBuffIndex ) == false )
	{
		return NULL;
	}

	for( int i = 0 ; i < MAX_BUFF_EFFECT_INDEX ; i++ )
	{
		if( BuffData[i].BuffIndex == btBuffIndex )
		{
			return &BuffData[i];
		}
	}

	return NULL;
}

#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917