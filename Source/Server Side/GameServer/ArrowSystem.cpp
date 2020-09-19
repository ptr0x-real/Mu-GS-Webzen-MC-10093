#include "stdafx.h"
#include "ArrowSystem.h"
#include "..\include\readscript.h"

CArrowSystem g_ArrowSystem;

CArrowSystem::CArrowSystem() : m_bFreeArrow(false)
{
	InitializeCriticalSection(&this->m_criticalSection);
}

CArrowSystem::~CArrowSystem()
{
	DeleteCriticalSection(&this->m_criticalSection);
}

void CArrowSystem::Load(const char *filename)
{
	EnterCriticalSection(&this->m_criticalSection);

	this->m_itemList.clear();

	if ( !filename || !strcmp(filename , "") )
	{
		MsgBox("[Arrow System] - File load error : File Name Error");

		LeaveCriticalSection(&this->m_criticalSection);

		return;
	}

	try
	{
		SMDFile = fopen(filename, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[Arrow System] Info file Load Fail [%s]", filename);

			LeaveCriticalSection(&this->m_criticalSection);

			return;
		}

		int Token;
		int type = -1;

		bool eventEnabled = true;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END || eventEnabled == false )
			{
				break;
			}

			type = TokenNumber;

			while ( true )
			{
				if ( type == 0 ) //Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					m_bFreeArrow = TokenNumber > 0;
				}

				else if ( type == 2 ) //Reward List
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					INT itemId = TokenNumber;

					if(itemId < 0 || itemId > MAX_ITEM_TYPE-1)
						throw "Item type out of range";

					Token = GetToken();

					if(TokenNumber < 0 || TokenNumber > MAX_ITEM_INDEX-1)
						throw "Item index out of range";

					itemId = ItemGetNumberMake(itemId,TokenNumber);

					Token = GetToken();

					this->m_itemList.insert(std::make_pair(itemId, TokenNumber > 0));
				}
			}
		}

		fclose(SMDFile);
		LogAdd("[Arrow System] %s file load!", filename);

		if(m_bFreeArrow)
		{
			LogAdd("[Arrow System] Free Arrow Mode");
		}
		else
		{
			LogAdd("[Arrow System] Item List Mode");
		}
	}	// __try
	catch ( const char *exception )
	{
		MsgBox("[Arrow System] - Loading Exception Error (%s) - (%s) File. ", filename, exception);
	}

	LeaveCriticalSection(&this->m_criticalSection);
}

bool CArrowSystem::CheckArrow(int itemId)
{
	if(this->m_bFreeArrow == true) return true;

	ITEM_LIST::iterator it = this->m_itemList.find(itemId);

	if(it == m_itemList.end())
		return false;

	return it->second;
}