# Microsoft Developer Studio Project File - Name="GameServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GameServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak" CFG="GameServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 DebugRelease" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KOR" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORCS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORTest" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORTestCS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KOR_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORCS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORTest_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORTestCS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 JPN" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 JPNCS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 JPN_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 JPNCS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 CHS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 CHSCS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 CHS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 CHSCS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 TAI" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 TAICS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 TAIFree" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 TAIFreeCS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 TAI_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 TAICS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 USA" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 USACS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 USA_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 USACS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 VIE" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 VIECS" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 VIE_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 VIECS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KOR_N" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORCS_N" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KOR_CHECKSUM" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 KORTest_CHECKSUM" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mumsg.lib comctl32.lib CSAuth.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mumsg.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /profile /incremental:no

!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_DebugRelease"
# PROP BASE Intermediate_Dir "GameServer___Win32_DebugRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_DebugRelease"
# PROP Intermediate_Dir "GameServer___Win32_DebugRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /profile /incremental:yes /debug
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KOR"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KOR"
# PROP BASE Intermediate_Dir "GameServer___Win32_KOR"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KOR"
# PROP Intermediate_Dir "GameServer___Win32_KOR"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORCS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORCS"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORCS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORCS"
# PROP Intermediate_Dir "GameServer___Win32_KORCS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KORCS/GameServer_CS.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORTest"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORTest"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORTest"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORTest"
# PROP Intermediate_Dir "GameServer___Win32_KORTest"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_TESTSERVER" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORTestCS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORTestCS"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORTestCS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORTestCS"
# PROP Intermediate_Dir "GameServer___Win32_KORTestCS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_TESTSERVER" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KORTestCS/GameServer_CS.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KOR_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KOR_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_KOR_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KOR_Debug"
# PROP Intermediate_Dir "GameServer___Win32_KOR_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORCS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORCS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORCS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORCS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_KORCS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KORCS_Debug/GameServer_공성.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORTest_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORTest_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORTest_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORTest_Debug"
# PROP Intermediate_Dir "GameServer___Win32_KORTest_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_TESTSERVER" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORTestCS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORTestCS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORTestCS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORTestCS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_KORTestCS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_TESTSERVER" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KORTestCS_Debug/GameServer_공성.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 JPN"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_JPN"
# PROP BASE Intermediate_Dir "GameServer___Win32_JPN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_JPN"
# PROP Intermediate_Dir "GameServer___Win32_JPN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /w /W0 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_JAPAN" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 JPNCS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_JPNCS"
# PROP BASE Intermediate_Dir "GameServer___Win32_JPNCS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_JPNCS"
# PROP Intermediate_Dir "GameServer___Win32_JPNCS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_JAPAN" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 JPN_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_JPN_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_JPN_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_JPN_Debug"
# PROP Intermediate_Dir "GameServer___Win32_JPN_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_JAPAN" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 JPNCS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_JPNCS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_JPNCS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_JPNCS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_JPNCS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_JAPAN" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 CHS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_CHS"
# PROP BASE Intermediate_Dir "GameServer___Win32_CHS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_CHS"
# PROP Intermediate_Dir "GameServer___Win32_CHS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_CHINA" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 CHSCS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_CHSCS"
# PROP BASE Intermediate_Dir "GameServer___Win32_CHSCS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_CHSCS"
# PROP Intermediate_Dir "GameServer___Win32_CHSCS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_CHINA" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 CHS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_CHS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_CHS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_CHS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_CHS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_CHINA" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 CHSCS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_CHSCS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_CHSCS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_CHSCS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_CHSCS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_CHINA" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 TAI"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_TAI"
# PROP BASE Intermediate_Dir "GameServer___Win32_TAI"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_TAI"
# PROP Intermediate_Dir "GameServer___Win32_TAI"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_TAIWAN" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 TAICS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_TAICS"
# PROP BASE Intermediate_Dir "GameServer___Win32_TAICS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_TAICS"
# PROP Intermediate_Dir "GameServer___Win32_TAICS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_TAIWAN" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 TAIFree"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_TAIFree"
# PROP BASE Intermediate_Dir "GameServer___Win32_TAIFree"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_TAIFree"
# PROP Intermediate_Dir "GameServer___Win32_TAIFree"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_TAIWAN" /D "_FREE" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 TAIFreeCS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_TAIFreeCS"
# PROP BASE Intermediate_Dir "GameServer___Win32_TAIFreeCS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_TAIFreeCS"
# PROP Intermediate_Dir "GameServer___Win32_TAIFreeCS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_TAIWAN" /D "_FREE" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 TAI_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_TAI_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_TAI_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_TAI_Debug"
# PROP Intermediate_Dir "GameServer___Win32_TAI_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_TAIWAN" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 TAICS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_TAICS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_TAICS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_TAICS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_TAICS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_TESTSERVER" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_TAIWAN" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_USA"
# PROP BASE Intermediate_Dir "GameServer___Win32_USA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_USA"
# PROP Intermediate_Dir "GameServer___Win32_USA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_USA" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 USACS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_USACS"
# PROP BASE Intermediate_Dir "GameServer___Win32_USACS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_USACS"
# PROP Intermediate_Dir "GameServer___Win32_USACS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_USA" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_USACS/GameServer_CS.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 USA_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_USA_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_USA_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_USA_Debug"
# PROP Intermediate_Dir "GameServer___Win32_USA_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_USA" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 USACS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_USACS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_USACS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_USACS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_USACS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_USA" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_USACS_Debug/GameServer_CS.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 VIE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_VIE"
# PROP BASE Intermediate_Dir "GameServer___Win32_VIE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_VIE"
# PROP Intermediate_Dir "GameServer___Win32_VIE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_VIETNAM" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 VIECS"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_VIECS"
# PROP BASE Intermediate_Dir "GameServer___Win32_VIECS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_VIECS"
# PROP Intermediate_Dir "GameServer___Win32_VIECS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_VIETNAM" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 VIE_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_VIE_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_VIE_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_VIE_Debug"
# PROP Intermediate_Dir "GameServer___Win32_VIE_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_VIETNAM" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 VIECS_Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_VIECS_Debug"
# PROP BASE Intermediate_Dir "GameServer___Win32_VIECS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_VIECS_Debug"
# PROP Intermediate_Dir "GameServer___Win32_VIECS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_VIETNAM" /D "_CS" /D "_RTEST" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KOR_N"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KOR_N"
# PROP BASE Intermediate_Dir "GameServer___Win32_KOR_N"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KOR_N"
# PROP Intermediate_Dir "GameServer___Win32_KOR_N"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_LIVESEVER" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORCS_N"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORCS_N"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORCS_N"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORCS_N"
# PROP Intermediate_Dir "GameServer___Win32_KORCS_N"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_CS" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "_CS" /D "FOR_NEW_LIVESEVER" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KORCS/GameServer_CS.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KORCS_N/GameServer_CS.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KOR_CHECKSUM"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KOR_CHECKSUM"
# PROP BASE Intermediate_Dir "GameServer___Win32_KOR_CHECKSUM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KOR_CHECKSUM"
# PROP Intermediate_Dir "GameServer___Win32_KOR_CHECKSUM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_CHECKSUM_SERVER" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KOR_CHECKSUM/GameServer_체크섬.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameServer - Win32 KORTest_CHECKSUM"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameServer___Win32_KORTest_CHECKSUM"
# PROP BASE Intermediate_Dir "GameServer___Win32_KORTest_CHECKSUM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GameServer___Win32_KORTest_CHECKSUM"
# PROP Intermediate_Dir "GameServer___Win32_KORTest_CHECKSUM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_TESTSERVER" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "FOR_KOREA" /D "FOR_NEW_TESTSERVER" /D "FOR_CHECKSUM_SERVER" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib ws2_32.lib mumsg.lib comctl32.lib CSAuth.lib dbghelp.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"GameServer___Win32_KORTest_CHECKSUM/GameServer_체크섬.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "GameServer - Win32 Release"
# Name "GameServer - Win32 Debug"
# Name "GameServer - Win32 DebugRelease"
# Name "GameServer - Win32 KOR"
# Name "GameServer - Win32 KORCS"
# Name "GameServer - Win32 KORTest"
# Name "GameServer - Win32 KORTestCS"
# Name "GameServer - Win32 KOR_Debug"
# Name "GameServer - Win32 KORCS_Debug"
# Name "GameServer - Win32 KORTest_Debug"
# Name "GameServer - Win32 KORTestCS_Debug"
# Name "GameServer - Win32 JPN"
# Name "GameServer - Win32 JPNCS"
# Name "GameServer - Win32 JPN_Debug"
# Name "GameServer - Win32 JPNCS_Debug"
# Name "GameServer - Win32 CHS"
# Name "GameServer - Win32 CHSCS"
# Name "GameServer - Win32 CHS_Debug"
# Name "GameServer - Win32 CHSCS_Debug"
# Name "GameServer - Win32 TAI"
# Name "GameServer - Win32 TAICS"
# Name "GameServer - Win32 TAIFree"
# Name "GameServer - Win32 TAIFreeCS"
# Name "GameServer - Win32 TAI_Debug"
# Name "GameServer - Win32 TAICS_Debug"
# Name "GameServer - Win32 USA"
# Name "GameServer - Win32 USACS"
# Name "GameServer - Win32 USA_Debug"
# Name "GameServer - Win32 USACS_Debug"
# Name "GameServer - Win32 VIE"
# Name "GameServer - Win32 VIECS"
# Name "GameServer - Win32 VIE_Debug"
# Name "GameServer - Win32 VIECS_Debug"
# Name "GameServer - Win32 KOR_N"
# Name "GameServer - Win32 KORCS_N"
# Name "GameServer - Win32 KOR_CHECKSUM"
# Name "GameServer - Win32 KORTest_CHECKSUM"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=GameServer.ico
# End Source File
# Begin Source File

SOURCE=.\GameServer.rc
# End Source File
# Begin Source File

SOURCE=resource.h
# End Source File
# Begin Source File

SOURCE=.\test_gam.ico
# End Source File
# End Group
# Begin Group "Monster"

# PROP Default_Filter ""
# Begin Group "Monster Herd System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MonsterHerd.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterHerd.h
# End Source File
# End Group
# Begin Group "KalimaGate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KalimaGate.cpp
# End Source File
# Begin Source File

SOURCE=.\KalimaGate.h
# End Source File
# End Group
# Begin Source File

SOURCE=gObjMonster.cpp
# End Source File
# Begin Source File

SOURCE=gObjMonster.h
# End Source File
# Begin Source File

SOURCE=MonsterAttr.cpp
# End Source File
# Begin Source File

SOURCE=MonsterAttr.h
# End Source File
# Begin Source File

SOURCE=.\MonsterItemMng.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterItemMng.h
# End Source File
# Begin Source File

SOURCE=MonsterSetBase.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterSetBase.h
# End Source File
# End Group
# Begin Group "Protocol"

# PROP Default_Filter ""
# Begin Group "Char-DB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=DbSave.cpp
# End Source File
# Begin Source File

SOURCE=DbSave.h
# End Source File
# Begin Source File

SOURCE=DBSockMng.cpp
# End Source File
# Begin Source File

SOURCE=DBSockMng.h
# End Source File
# Begin Source File

SOURCE=DSProtocol.cpp
# End Source File
# Begin Source File

SOURCE=DSProtocol.h
# End Source File
# End Group
# Begin Group "LoginServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Include\Sprodef.h
# End Source File
# Begin Source File

SOURCE=Sprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprotocol.h
# End Source File
# End Group
# Begin Group "MainProtocol"

# PROP Default_Filter ""
# Begin Source File

SOURCE=protocol.cpp
# End Source File
# Begin Source File

SOURCE=protocol.h
# End Source File
# End Group
# Begin Group "ExDB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EDSProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\EDSProtocol.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\INCLUDE\cb_protocol.h
# End Source File
# Begin Source File

SOURCE=..\Include\GMProtocol.h
# End Source File
# Begin Source File

SOURCE=..\Include\MngProtocol.h
# End Source File
# Begin Source File

SOURCE=..\Include\ProDef.h
# End Source File
# End Group
# Begin Group "Anti-Hack"

# PROP Default_Filter ""
# Begin Group "다중공격핵 체크"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MultiAttackHackCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiAttackHackCheck.h
# End Source File
# End Group
# Begin Group "프로토콜 시리얼 체크"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NSerialCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\NSerialCheck.h
# End Source File
# End Group
# Begin Group "Function Checksum"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PacketCheckSum.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketCheckSum.h
# End Source File
# End Group
# Begin Group "Cryption"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimpleModulus.h
# End Source File
# Begin Source File

SOURCE=.\SimpleModulus.lib
# End Source File
# End Group
# Begin Group "지속마법판정체크"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TDurMagicKeyChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\TDurMagicKeyChecker.h
# End Source File
# End Group
# Begin Group "중국핵로그시스템"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChinaHackUserKick.cpp
# End Source File
# Begin Source File

SOURCE=.\ChinaHackUserKick.h
# End Source File
# End Group
# Begin Group "핵툴 맵 이동 차단"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HackToolBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\HackToolBlock.h
# End Source File
# Begin Source File

SOURCE=.\KeyGenerater.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyGenerater.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AntiHack.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiHack.h
# End Source File
# Begin Source File

SOURCE=.\AntiHackDef.h
# End Source File
# Begin Source File

SOURCE=.\PacketStatistics.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketStatistics.h
# End Source File
# Begin Source File

SOURCE=.\SendHackLog.cpp
# End Source File
# Begin Source File

SOURCE=.\SendHackLog.h
# End Source File
# End Group
# Begin Group "Devil Sqaure"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DevilSquare.cpp
# End Source File
# Begin Source File

SOURCE=.\DevilSquare.h
# End Source File
# Begin Source File

SOURCE=.\DevilSquareGround.cpp
# End Source File
# Begin Source File

SOURCE=.\DevilSquareGround.h
# End Source File
# End Group
# Begin Group "Battle Soccer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=BattleGround.cpp
# End Source File
# Begin Source File

SOURCE=BattleGround.h
# End Source File
# Begin Source File

SOURCE=BattleSoccer.cpp
# End Source File
# Begin Source File

SOURCE=BattleSoccer.h
# End Source File
# Begin Source File

SOURCE=.\BattleSoccerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleSoccerManager.h
# End Source File
# End Group
# Begin Group "Event"

# PROP Default_Filter ""
# Begin Group "환수드래곤"

# PROP Default_Filter ""
# Begin Source File

SOURCE=DragonEvent.cpp
# End Source File
# Begin Source File

SOURCE=DragonEvent.h
# End Source File
# End Group
# Begin Group "지하군단의 역습"

# PROP Default_Filter ""
# Begin Source File

SOURCE=AttackEvent.cpp
# End Source File
# Begin Source File

SOURCE=AttackEvent.h
# End Source File
# End Group
# Begin Group "엘도라도의상자"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EledoradoEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\EledoradoEvent.h
# End Source File
# End Group
# Begin Group "반지의제왕 이벤트"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RingAttackEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\RingAttackEvent.h
# End Source File
# End Group
# Begin Group "월드토너먼드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WTEventItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\WTEventItemList.h
# End Source File
# End Group
# Begin Group "로랜협곡 습격"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CastleDeepEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleDeepEvent.h
# End Source File
# End Group
# Begin Group "PC방포인트 시스템"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MDate.cpp
# End Source File
# Begin Source File

SOURCE=.\MDate.h
# End Source File
# Begin Source File

SOURCE=.\PCBangPointSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\PCBangPointSystem.h
# End Source File
# End Group
# Begin Group "GamblingSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GamblingSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\GamblingSystem.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BaseAttackEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseAttackEvent.h
# End Source File
# Begin Source File

SOURCE=Event.cpp
# End Source File
# Begin Source File

SOURCE=Event.h
# End Source File
# Begin Source File

SOURCE=.\EventManagement.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManagement.h
# End Source File
# Begin Source File

SOURCE=ItemBag.cpp
# End Source File
# Begin Source File

SOURCE=ItemBag.h
# End Source File
# Begin Source File

SOURCE=.\ItemBagEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBagEx.h
# End Source File
# Begin Source File

SOURCE=.\ProbabilityItemBag.cpp
# End Source File
# Begin Source File

SOURCE=.\ProbabilityItemBag.h
# End Source File
# Begin Source File

SOURCE=.\XMasAttackEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\XMasAttackEvent.h
# End Source File
# End Group
# Begin Group "Magic & Skill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=MagicDamage.cpp
# End Source File
# Begin Source File

SOURCE=MagicDamage.h
# End Source File
# Begin Source File

SOURCE=MagicInf.cpp
# End Source File
# Begin Source File

SOURCE=MagicInf.h
# End Source File
# Begin Source File

SOURCE=.\SkillAdditionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillAdditionInfo.h
# End Source File
# Begin Source File

SOURCE=.\SkillDelay.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillDelay.h
# End Source File
# Begin Source File

SOURCE=.\SkillHitBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillHitBox.h
# End Source File
# End Group
# Begin Group "Quest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\QuestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestInfo.h
# End Source File
# Begin Source File

SOURCE=.\QuestUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestUtil.h
# End Source File
# End Group
# Begin Group "SOCKET"

# PROP Default_Filter ""
# Begin Group "사용안함"

# PROP Default_Filter ""
# Begin Source File

SOURCE=wsGameServer.cpp
# End Source File
# Begin Source File

SOURCE=wsGameServer.h
# End Source File
# End Group
# Begin Group "MultiCast"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WzMultiCastSock.cpp
# End Source File
# Begin Source File

SOURCE=.\WzMultiCastSock.h
# End Source File
# End Group
# Begin Source File

SOURCE=giocp.cpp
# End Source File
# Begin Source File

SOURCE=giocp.h
# End Source File
# Begin Source File

SOURCE=..\common\MyWinsockBase.cpp
# End Source File
# Begin Source File

SOURCE=..\common\MyWinsockBase.h
# End Source File
# Begin Source File

SOURCE=wsJoinServerCli.cpp
# End Source File
# Begin Source File

SOURCE=wsJoinServerCli.h
# End Source File
# Begin Source File

SOURCE=WzIocp.h
# End Source File
# Begin Source File

SOURCE=WzQueue.cpp
# End Source File
# Begin Source File

SOURCE=WzQueue.h
# End Source File
# Begin Source File

SOURCE=WzUdp.cpp
# End Source File
# End Group
# Begin Group "Map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=MapClass.cpp
# End Source File
# Begin Source File

SOURCE=MapClass.h
# End Source File
# Begin Source File

SOURCE=MapItem.cpp
# End Source File
# Begin Source File

SOURCE=MapItem.h
# End Source File
# End Group
# Begin Group "UserConnect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=AcceptIp.cpp
# End Source File
# Begin Source File

SOURCE=AcceptIp.h
# End Source File
# Begin Source File

SOURCE=.\CLoginCount.cpp
# End Source File
# Begin Source File

SOURCE=.\CLoginCount.h
# End Source File
# Begin Source File

SOURCE=closeplayer.cpp
# End Source File
# Begin Source File

SOURCE=closeplayer.h
# End Source File
# Begin Source File

SOURCE=ConMember.cpp
# End Source File
# Begin Source File

SOURCE=ConMember.h
# End Source File
# Begin Source File

SOURCE=IpCache.cpp
# End Source File
# Begin Source File

SOURCE=IpCache.h
# End Source File
# End Group
# Begin Group "ETC"

# PROP Default_Filter ""
# Begin Group "디렉토리관리"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DirPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DirPath.h
# End Source File
# End Group
# Begin Group "로그출력"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\logproc.cpp
# End Source File
# Begin Source File

SOURCE=.\LogProc.h
# End Source File
# Begin Source File

SOURCE=.\LogToFile.cpp
# End Source File
# Begin Source File

SOURCE=.\LogToFile.h
# End Source File
# End Group
# Begin Group "이동체크(사용안함)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MoveCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveCheck.h
# End Source File
# End Group
# Begin Group "Message_WTF"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\Msg.h
# End Source File
# End Group
# Begin Group "WinUtil"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\winutil.cpp
# End Source File
# Begin Source File

SOURCE=..\common\winutil.h
# End Source File
# End Group
# Begin Group "귓말"

# PROP Default_Filter ""
# Begin Source File

SOURCE=WhisperCash.cpp
# End Source File
# Begin Source File

SOURCE=WhisperCash.h
# End Source File
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=zzzmathlib.cpp
# End Source File
# End Group
# Begin Group "PathFind"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\ZzzPath.h
# End Source File
# End Group
# Begin Group "스크립트암호화"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\WZScriptEncode.cpp
# End Source File
# Begin Source File

SOURCE=..\common\WZScriptEncode.h
# End Source File
# End Group
# Begin Group "WhatsUp 더미서버"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CWhatsUpDummyServer.cpp
# End Source File
# Begin Source File

SOURCE=.\CWhatsUpDummyServer.h
# End Source File
# End Group
# Begin Group "공지사항"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\TNotice.h
# End Source File
# End Group
# Begin Group "게임통계"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TStatistics.cpp
# End Source File
# Begin Source File

SOURCE=.\TStatistics.h
# End Source File
# End Group
# Begin Group "CallStackTrace"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CallStackTrace.CPP
# End Source File
# Begin Source File

SOURCE=.\CallStackTrace.H
# End Source File
# End Group
# Begin Group "게임서버정보 Diplay"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TServerAlertManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TServerAlertManager.h
# End Source File
# Begin Source File

SOURCE=.\TServerInfoDisplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\TServerInfoDisplayer.h
# End Source File
# End Group
# Begin Group "MiniDump"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MiniDump.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniDump.h
# End Source File
# End Group
# End Group
# Begin Group "User"

# PROP Default_Filter ""
# Begin Group "Party"

# PROP Default_Filter ""
# Begin Source File

SOURCE=PartyClass.cpp
# End Source File
# Begin Source File

SOURCE=PartyClass.h
# End Source File
# End Group
# Begin Group "Guild"

# PROP Default_Filter ""
# Begin Source File

SOURCE=GuildClass.cpp
# End Source File
# Begin Source File

SOURCE=GuildClass.h
# End Source File
# Begin Source File

SOURCE=ViewportGuild.cpp
# End Source File
# Begin Source File

SOURCE=ViewportGuild.h
# End Source File
# End Group
# Begin Group "공격처리"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ObjAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjAttack.h
# End Source File
# Begin Source File

SOURCE=.\ObjBaseAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjBaseAttack.h
# End Source File
# End Group
# Begin Group "스킬&마법 사용"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ObjUseSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjUseSkill.h
# End Source File
# End Group
# Begin Group "캐릭터 생성시기본정보"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\classdef.cpp
# End Source File
# Begin Source File

SOURCE=..\common\classdef.h
# End Source File
# End Group
# Begin Group "올릴수있는 스탯 정보"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StatMng.cpp
# End Source File
# Begin Source File

SOURCE=.\StatMng.h
# End Source File
# End Group
# Begin Group "콤보공격"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ComboAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboAttack.h
# End Source File
# End Group
# Begin Group "PK시스템"

# PROP Default_Filter ""
# End Group
# Begin Group "캐릭터 수치계산"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ObjCalCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjCalCharacter.h
# End Source File
# End Group
# Begin Group "다크스피릿"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DarkSpirit.cpp
# End Source File
# Begin Source File

SOURCE=.\DarkSpirit.h
# End Source File
# End Group
# Begin Group "Union"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TUnion.cpp
# End Source File
# Begin Source File

SOURCE=.\TUnion.h
# End Source File
# Begin Source File

SOURCE=.\TUnionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TUnionInfo.h
# End Source File
# End Group
# Begin Group "마스터레벨 시스템"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MasterLevelSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterLevelSystem.h
# End Source File
# Begin Source File

SOURCE=.\MasterSkillSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterSkillSystem.h
# End Source File
# End Group
# Begin Source File

SOURCE=user.cpp
# End Source File
# Begin Source File

SOURCE=user.h
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Group "SocketOption"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemSeedSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemSeedSphere.h
# End Source File
# Begin Source File

SOURCE=..\common\ItemSocketOptionDefine.h
# End Source File
# Begin Source File

SOURCE=.\ItemSocketOptionSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemSocketOptionSystem.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ItemAddOption.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemAddOption.h
# End Source File
# Begin Source File

SOURCE=..\Include\ItemDef.h
# End Source File
# Begin Source File

SOURCE=.\JewelMixSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\JewelMixSystem.h
# End Source File
# Begin Source File

SOURCE=..\common\SetItemOption.cpp
# End Source File
# Begin Source File

SOURCE=..\common\SetItemOption.h
# End Source File
# Begin Source File

SOURCE=..\common\zzzitem.cpp
# End Source File
# Begin Source File

SOURCE=..\common\zzzitem.h
# End Source File
# End Group
# Begin Group "Shop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Shop.cpp
# End Source File
# Begin Source File

SOURCE=Shop.h
# End Source File
# End Group
# Begin Group "Defines"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Include\define.h
# End Source File
# Begin Source File

SOURCE=..\Include\public.h
# End Source File
# End Group
# Begin Group "명령어처리"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CCastleEventItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\CCastleEventItemList.h
# End Source File
# Begin Source File

SOURCE=GMMng.cpp
# End Source File
# Begin Source File

SOURCE=GMMng.h
# End Source File
# Begin Source File

SOURCE=.\MoveCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveCommand.h
# End Source File
# Begin Source File

SOURCE=.\SetItemMacro.cpp
# End Source File
# Begin Source File

SOURCE=.\SetItemMacro.h
# End Source File
# End Group
# Begin Group "GameMain"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Gamemain.cpp
# End Source File
# Begin Source File

SOURCE=Gamemain.h
# End Source File
# End Group
# Begin Group "MainAPP"

# PROP Default_Filter ""
# Begin Group "stdafx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=GameServer.cpp
# End Source File
# Begin Source File

SOURCE=GameServer.h
# End Source File
# End Group
# Begin Group "Gate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Gate.cpp
# End Source File
# Begin Source File

SOURCE=Gate.h
# End Source File
# End Group
# Begin Group "Blood Castle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BloodCastle.cpp
# End Source File
# Begin Source File

SOURCE=.\BloodCastle.h
# End Source File
# End Group
# Begin Group "GameGuard"

# PROP Default_Filter ""
# Begin Group "1.0"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSAuth.h
# End Source File
# Begin Source File

SOURCE=.\CSAuth.lib
# End Source File
# End Group
# Begin Group "2.0"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ggerror.h
# End Source File
# Begin Source File

SOURCE=.\ggsrv.h
# End Source File
# Begin Source File

SOURCE=.\ggsrvlib.lib
# End Source File
# End Group
# Begin Group "2.5"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ggsrv25.h
# End Source File
# Begin Source File

SOURCE=.\ggsrvlib25.lib
# End Source File
# End Group
# End Group
# Begin Group "DOC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\History200502.txt
# End Source File
# Begin Source File

SOURCE=.\OldCodeBackup_BY_CAT.txt
# End Source File
# Begin Source File

SOURCE=patch.txt
# End Source File
# Begin Source File

SOURCE=.\patch_country.txt
# End Source File
# Begin Source File

SOURCE=ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\패치표.xls"
# End Source File
# End Group
# Begin Group "Chaos Castle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChaosCastle.cpp
# End Source File
# Begin Source File

SOURCE=.\ChaosCastle.h
# End Source File
# Begin Source File

SOURCE=.\ChaosCastleSetItemDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\ChaosCastleSetItemDrop.h
# End Source File
# End Group
# Begin Group "Castle Siege"

# PROP Default_Filter ""
# Begin Group "CS NPC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CannonTower.cpp
# End Source File
# Begin Source File

SOURCE=.\CannonTower.h
# End Source File
# Begin Source File

SOURCE=.\CastleCrown.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleCrown.h
# End Source File
# Begin Source File

SOURCE=.\CastleCrownSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleCrownSwitch.h
# End Source File
# Begin Source File

SOURCE=.\Guardian.cpp
# End Source File
# Begin Source File

SOURCE=.\Guardian.h
# End Source File
# Begin Source File

SOURCE=.\GuardianStatue.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardianStatue.h
# End Source File
# Begin Source File

SOURCE=.\LifeStone.cpp
# End Source File
# Begin Source File

SOURCE=.\LifeStone.h
# End Source File
# Begin Source File

SOURCE=.\Mercenary.cpp
# End Source File
# Begin Source File

SOURCE=.\Mercenary.h
# End Source File
# Begin Source File

SOURCE=.\Weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapon.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CastleSiege.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleSiege.h
# End Source File
# Begin Source File

SOURCE=.\CastleSiegeSync.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleSiegeSync.h
# End Source File
# Begin Source File

SOURCE=.\MapServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MapServerManager.h
# End Source File
# End Group
# Begin Group "NPC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=NpcTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcTalk.h
# End Source File
# End Group
# Begin Group "New Monster"

# PROP Default_Filter ""
# Begin Group "Skill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TMonsterSkillElement.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillElement.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillElementInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillElementInfo.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillManager.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillUnit.h
# End Source File
# Begin Source File

SOURCE=.\TSkillDefine.h
# End Source File
# Begin Source File

SOURCE=.\TSkillElement.h
# End Source File
# Begin Source File

SOURCE=.\TSkillUnit.h
# End Source File
# End Group
# Begin Group "AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TMonsterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAI.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAgro.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAgro.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAutomata.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAutomata.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIElement.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIElement.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroup.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroupMember.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroupMember.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIMovePath.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIMovePath.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRule.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRule.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRuleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRuleInfo.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIState.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUnit.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUtil.h
# End Source File
# End Group
# End Group
# Begin Group "Crywolf"

# PROP Default_Filter ""
# Begin Group "Crywolf NPC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CrywolfAltar.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfAltar.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfStatue.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfStatue.h
# End Source File
# End Group
# Begin Group "Crywolf Monster"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CrywolfDarkElf.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfDarkElf.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfMonster.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfMonsterInfo.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfTanker.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfTanker.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Crywolf.cpp
# End Source File
# Begin Source File

SOURCE=.\Crywolf.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfDefine.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfObjInfo.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfStateTimeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfStateTimeInfo.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfSync.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfSync.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfUtil.h
# End Source File
# End Group
# Begin Group "JewelOfHarmnoy System & 380"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemSystemFor380.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemSystemFor380.h
# End Source File
# Begin Source File

SOURCE=.\JewelOfHarmonySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\JewelOfHarmonySystem.h
# End Source File
# Begin Source File

SOURCE=.\TRandomPoolMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TRandomPoolMgr.h
# End Source File
# End Group
# Begin Group "Kanturu"

# PROP Default_Filter ""
# Begin Group "BOSS MONSTER"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KanturuMaya.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuMaya.h
# End Source File
# Begin Source File

SOURCE=.\KanturuMonsterMng.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuMonsterMng.h
# End Source File
# Begin Source File

SOURCE=.\KanturuNightmare.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuNightmare.h
# End Source File
# End Group
# Begin Group "BATTLE STATE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KanturuBattleOfMaya.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleOfMaya.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleOfNightmare.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleOfNightmare.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleStanby.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleStanby.h
# End Source File
# Begin Source File

SOURCE=.\KanturuTowerOfRefinement.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuTowerOfRefinement.h
# End Source File
# End Group
# Begin Group "BATTLE USER"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KanturuBattleUser.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleUserMng.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleUserMng.h
# End Source File
# End Group
# Begin Group "KANTURU NPC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KanturuEntranceNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuEntranceNPC.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Kanturu.cpp
# End Source File
# Begin Source File

SOURCE=.\Kanturu.h
# End Source File
# Begin Source File

SOURCE=.\KanturuDefine.h
# End Source File
# Begin Source File

SOURCE=.\KanturuObjInfo.h
# End Source File
# Begin Source File

SOURCE=.\KanturuStateInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuStateInfo.h
# End Source File
# Begin Source File

SOURCE=.\KanturuUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuUtil.h
# End Source File
# End Group
# Begin Group "Illusion Temple"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IllusionTempleEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\IllusionTempleEvent.h
# End Source File
# Begin Source File

SOURCE=.\IllusionTempleProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\IllusionTempleProcess.h
# End Source File
# End Group
# Begin Group "MixSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MixSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\MixSystem.h
# End Source File
# End Group
# Begin Group "PartialyChargeShop"

# PROP Default_Filter ""
# Begin Group "CashShop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CashShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CashShop.h
# End Source File
# End Group
# Begin Group "ShopServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PCSdefines.h
# End Source File
# Begin Source File

SOURCE=.\PCSprotocol.h
# End Source File
# Begin Source File

SOURCE=.\wsShopServerCli.cpp
# End Source File
# Begin Source File

SOURCE=.\wsShopServerCli.h
# End Source File
# End Group
# Begin Group "CashItemPeriodSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CashItemPeriodSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\CashItemPeriodSystem.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CashLotterySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\CashLotterySystem.h
# End Source File
# End Group
# Begin Group "BuffEffect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BuffEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\BuffEffect.h
# End Source File
# Begin Source File

SOURCE=.\BuffEffectSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\BuffEffectSlot.h
# End Source File
# Begin Source File

SOURCE=.\BuffScriptLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\BuffScriptLoader.h
# End Source File
# Begin Source File

SOURCE=.\BUFFTYPEDEF.h
# End Source File
# End Group
# Begin Group "Raklion"

# PROP Default_Filter ""
# Begin Group "Raklion Boss Monster"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RaklionSelupan.cpp
# End Source File
# Begin Source File

SOURCE=.\RaklionSelupan.h
# End Source File
# End Group
# Begin Group "Raklion Battle State"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RaklionBattleOfSelupan.cpp
# End Source File
# Begin Source File

SOURCE=.\RaklionBattleOfSelupan.h
# End Source File
# End Group
# Begin Group "Raklion Battle User"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RaklionBattleUserMng.cpp
# End Source File
# Begin Source File

SOURCE=.\RaklionBattleUserMng.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Raklion.cpp
# End Source File
# Begin Source File

SOURCE=.\Raklion.h
# End Source File
# Begin Source File

SOURCE=.\RaklionDefine.h
# End Source File
# Begin Source File

SOURCE=.\RaklionObjInfo.h
# End Source File
# Begin Source File

SOURCE=.\RaklionUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RaklionUtil.h
# End Source File
# End Group
# Begin Group "Lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Lua\LuaFun.cpp
# End Source File
# Begin Source File

SOURCE=..\Lua\LuaSample.cpp
# End Source File
# Begin Source File

SOURCE=..\Lua\LuaSample.h
# End Source File
# Begin Source File

SOURCE=..\Lua\MuLua.cpp
# End Source File
# Begin Source File

SOURCE=..\Lua\MuLua.h
# End Source File
# End Group
# Begin Group "Custom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ArrowSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrowSystem.h
# End Source File
# Begin Source File

SOURCE=.\BuffHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\BuffHelper.h
# End Source File
# Begin Source File

SOURCE=.\DropTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DropTable.h
# End Source File
# Begin Source File

SOURCE=.\EngagePVP.cpp
# End Source File
# Begin Source File

SOURCE=.\EngagePVP.h
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\HappyHour.cpp
# End Source File
# Begin Source File

SOURCE=.\HappyHour.h
# End Source File
# Begin Source File

SOURCE=.\News.cpp
# End Source File
# Begin Source File

SOURCE=.\News.h
# End Source File
# Begin Source File

SOURCE=.\PKClear.cpp
# End Source File
# Begin Source File

SOURCE=.\PKClear.h
# End Source File
# Begin Source File

SOURCE=.\Post.cpp
# End Source File
# Begin Source File

SOURCE=.\Post.h
# End Source File
# Begin Source File

SOURCE=.\ResetSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ResetSystem.h
# End Source File
# Begin Source File

SOURCE=.\TerrainManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EventItemBagExMng.cpp
# End Source File
# Begin Source File

SOURCE=.\EventItemBagExMng.h
# End Source File
# Begin Source File

SOURCE=.\ExceptionHandler.h
# End Source File
# Begin Source File

SOURCE=.\GameEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\GameEvent.h
# End Source File
# Begin Source File

SOURCE=.\GameServerAuth.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServerAuth.h
# End Source File
# Begin Source File

SOURCE=.\LargeRand.cpp
# End Source File
# Begin Source File

SOURCE=.\LargeRand.h
# End Source File
# Begin Source File

SOURCE=.\MultiCheckSum.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiCheckSum.h
# End Source File
# Begin Source File

SOURCE=.\NewPVP.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPVP.h
# End Source File
# Begin Source File

SOURCE=..\Common\TSync.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\TSync.h
# End Source File
# Begin Source File

SOURCE=..\common\WzMemScript.cpp
# End Source File
# Begin Source File

SOURCE=..\common\WzMemScript.h
# End Source File
# Begin Source File

SOURCE=".\버그리스트.txt"
# End Source File
# End Target
# End Project
