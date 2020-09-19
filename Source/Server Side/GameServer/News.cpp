#include "StdAfx.h"
#include "News.h"
#include "TNotice.h"

#include "Include\Readscript.h"

CNewsSystem g_NewsSystem;

CNewsSystem::CNewsSystem()
{
}

CNewsSystem::~CNewsSystem()
{
}

DWORD WINAPI CNewsSystem::Process(LPVOID lpParameter)
{
#if 1
	while(true)
	{ 
		g_NewsSystem.Sections = GetPrivateProfileIntA("News","NewsSections",5,("..\\Data\\NewsSystem.cfg")); 

		char PState[10]; 
		for(int i = 1; i <= g_NewsSystem.Sections; i++)
		{
			wsprintf(PState, "Section%d", i);		
			g_NewsSystem.NewsSys[i].Time = GetPrivateProfileIntA(PState,"ShowTime",0,("..\\Data\\NewsSystem.cfg")) * 60000;
			g_NewsSystem.NewsSys[i].Type = GetPrivateProfileIntA(PState,"ShowType",0,("..\\Data\\NewsSystem.cfg"));
			GetPrivateProfileStringA(PState,"News1","NULL",g_NewsSystem.NewsSys[i].MSG01,50,("..\\Data\\NewsSystem.cfg"));
			GetPrivateProfileStringA(PState,"News2", "NULL", g_NewsSystem.NewsSys[i].MSG02,50,("..\\Data\\NewsSystem.cfg"));
			GetPrivateProfileStringA(PState,"News3", "NULL", g_NewsSystem.NewsSys[i].MSG03,50,("..\\Data\\NewsSystem.cfg")); 
			Sleep(g_NewsSystem.NewsSys[i].Time); 
			PMSG_NOTICE	pNotice;

			TNotice::MakeNoticeMsg( &pNotice, g_NewsSystem.NewsSys[i].Type, g_NewsSystem.NewsSys[i].MSG01 );
			TNotice::SendNoticeToAllUser(&pNotice);
			TNotice::MakeNoticeMsg( &pNotice, g_NewsSystem.NewsSys[i].Type, g_NewsSystem.NewsSys[i].MSG02 );
			TNotice::SendNoticeToAllUser(&pNotice);
			TNotice::MakeNoticeMsg( &pNotice, g_NewsSystem.NewsSys[i].Type, g_NewsSystem.NewsSys[i].MSG03 );
			TNotice::SendNoticeToAllUser(&pNotice);
		} 
	}
#endif
	return 0;
}