/*
 *  __  __
 * |  \/  |
 * | \  / | ___ _ __ ___  _   _
 * | |\/| |/ _ \ '_ ` _ \| | | |
 * | |  | |  __/ | | | | | |_| |
 * |_|  |_|\___|_| |_| |_|\__,_|
 *
 * Mu Emulator
 *
 * File:   CriticalSection.h
 * Author: J.Kwog
 *
 * Created on December 24, 2009
 */
#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#include <windows.h>

class CCriticalSection
{
public:
	CCriticalSection(){};
	~CCriticalSection(){};

	__forceinline void Create() {	InitializeCriticalSection(&this->m_cs);	}
	__forceinline void Delete() {	DeleteCriticalSection(&this->m_cs); }
	__forceinline void Enter()	 {	EnterCriticalSection(&this->m_cs); }
	__forceinline void Leave()	 {	LeaveCriticalSection(&this->m_cs); }

private:
	CRITICAL_SECTION	m_cs;
};

class CAutoCriticalSection
{
public:
	CAutoCriticalSection(){InitializeCriticalSection(&this->m_cs);};
	~CAutoCriticalSection(){DeleteCriticalSection(&this->m_cs);};

	__forceinline void Enter()	 {	EnterCriticalSection(&this->m_cs); }
	__forceinline void Leave()	 {	LeaveCriticalSection(&this->m_cs); }

private:
	CRITICAL_SECTION	m_cs;
};

#endif // CRITICAL_SECTION_H