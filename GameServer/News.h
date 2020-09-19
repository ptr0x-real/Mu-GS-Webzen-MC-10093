#ifndef __NEWS_H__
#define __NEWS_H__

#pragma once

class CNewsSystem
{
public:
	CNewsSystem();
	virtual ~CNewsSystem();

	static DWORD WINAPI Process(LPVOID lpParameter);

private:
	int Sections;
	struct sNews
	{
		char MSG01[50];
		char MSG02[50];
		char MSG03[50];
		float Time;
		int Type;
	};
	sNews NewsSys[20];
};

extern CNewsSystem g_NewsSystem;

#endif