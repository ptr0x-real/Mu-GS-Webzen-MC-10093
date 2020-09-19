// WzSvMng_Interface.h: interface for the CWzSvMng_Interface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZSVMNG_INTERFACE_H__786B67D2_C94F_4807_9805_D2E6D76C4AC7__INCLUDED_)
#define AFX_WZSVMNG_INTERFACE_H__786B67D2_C94F_4807_9805_D2E6D76C4AC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WZAG_DLLINTERFACE_IMPORT
	#define WZAG_DLLINTERFACE			extern "C" _declspec(dllimport)
#else
	#define WZAG_DLLINTERFACE			extern "C" _declspec(dllexport)
#endif

#define AG_HANDLE	DWORD	

typedef AG_HANDLE	(* IAGFuncSetInfo)			(int, BYTE, short, char*, char*, int, void callbackfunc(int, int));
typedef void		(* IAGGetClientVersion)		(AG_HANDLE, char*, char*);
typedef void		(* IAGSetSubInfo)			(AG_HANDLE, int , int , int , int , float , int );
typedef void		(* IAGGetKey)				(AG_HANDLE, char*, int, int );
typedef void		(* IAGRequestData)			(AG_HANDLE, int );
typedef int			(* IAGGetDataBufferSize)	(AG_HANDLE);
typedef char*		(* IAGGetDataBuffer)		(AG_HANDLE);
typedef BOOL		(* IAGConnectSetFS)			(AG_HANDLE);
typedef void		(* IAGReleaseHandle)		(AG_HANDLE);

// 게임서버 기본 정보 설정 (게임인증서버 접속 및 인증도 여기서 된다)
WZAG_DLLINTERFACE AG_HANDLE AGSetInfo(int iCountryCode, 
									BYTE cPartition,
									short iServerCode,
									char* szServerVersion,
									char* szServerName,
									int ServerType, 
									void callbackfunc(int, int));

// 클라이언트 버전 정보를 얻는다
WZAG_DLLINTERFACE void AGGetClientVersion(AG_HANDLE, char* szClientVersion,char* szClientSerial);

// 서버 정보를 설정한다
WZAG_DLLINTERFACE void AGSetSubInfo(AG_HANDLE, int MaxUser, int CurUser, int MonsterHP, int ItemDropRate, float AddExp, int EventFlag);

// 키값을 읽는다
WZAG_DLLINTERFACE void AGGetKey(AG_HANDLE, char* Key, int startindex, int len);

// 서버에 데이터 파일을 요청한다(데이터를 받을때 까지 블럭)
WZAG_DLLINTERFACE void AGRequestData(AG_HANDLE, int type);

// 서버에서 받은 데이터 파일의 크기를 얻어 온다
WZAG_DLLINTERFACE int AGGetDataBufferSize(AG_HANDLE);

// 서버에서 받은 데이터 버퍼를 얻는다
WZAG_DLLINTERFACE char* AGGetDataBuffer(AG_HANDLE);

// FS 서버에 접속한다
WZAG_DLLINTERFACE BOOL AGConnectSetFS(AG_HANDLE);

// 핸들을 해제 한다
WZAG_DLLINTERFACE void AGReleaseHandle(AG_HANDLE);

#endif // !defined(AFX_WZSVMNG_INTERFACE_H__786B67D2_C94F_4807_9805_D2E6D76C4AC7__INCLUDED_)
