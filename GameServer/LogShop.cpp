#include "StdAfx.h"
#include "protocol.h"
#include "LogProc.h"

void LogSystem::CustomLog(char * Text)
{
	SYSTEMTIME now;
	GetLocalTime(&now);

	TCHAR LogTexto[512];
	FILE *stream;

	TCHAR logString[512];

	wsprintf(logString,"Log\\%02d_%02d_%02d_SQL_Log.txt", now.wYear, now.wMonth,now.wDay );
	stream=fopen(logString, "a+");
	fprintf(stream,"%02d:%02d:%02d - %s\n", now.wHour, now.wMinute, now.wSecond, Text);
	fclose(stream);
}

//AGREGAR LOGS PARA CASH
void LogSystem::CustomLogcash(char * Text)
{
	SYSTEMTIME now;
	GetLocalTime(&now);

	TCHAR LogTexto[512];
	FILE *stream;

	TCHAR logString[512];

	wsprintf(logString, "STORE\\%02d_%02d_%02d_cash_Log.txt", now.wYear, now.wMonth, now.wDay);
	stream = fopen(logString, "a+");
	fprintf(stream, "%02d:%02d:%02d - %s\n", now.wHour, now.wMinute, now.wSecond, Text);
	fclose(stream);
}

//LOG PARA COMANDO GM CASH
void LogSystem::CustomLogcashgm(char * Text)
{
	SYSTEMTIME now;
	GetLocalTime(&now);

	TCHAR LogTexto[512];
	FILE *stream;

	TCHAR logString[512];

	wsprintf(logString, "CASHGM\\%02d_%02d_%02d_cash_Log.txt", now.wYear, now.wMonth, now.wDay);
	stream = fopen(logString, "a+");
	fprintf(stream, "%02d:%02d:%02d - %s\n", now.wHour, now.wMinute, now.wSecond, Text);
	fclose(stream);
}