

#include "stdafx.h"

#include "CallStackTrace.H"

#include <time.h>
#include "dbghelp.h"
#include <string>
#include <fstream>



///////////////////////////////////////////////////////////////////
//	IMPLEMENTATION
///////////////////////////////////////////////////////////////////


//*---------------------------------------------------------------
// TYPE : FUNCTION
// NAME : CCallStackTrace()
//*---------------------------------------------------------------
// DESC : CCallStackTrace 클래스 생성자
// RETV : N/A
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
CCallStackTrace::CCallStackTrace (char szLogID [MAX_PATH], char szLogFileName [MAX_PATH], char szLogDirectoryName [MAX_PATH])
{
	if ( ! strcmp (szLogID, "") ) {
		MessageBox(HWND_DESKTOP, "INSERT LOG-ID !!!", "ERROR", MB_OK);
		return;
	}
	else
		strcpy (m_szLogID, szLogID);
		
	if ( ! strcmp (szLogFileName, "") )
		strcpy (m_szLogFileName, DEF_LOG_FILE_NAME);
	else
		strcpy (m_szLogFileName, szLogFileName);

	if ( ! strcmp (szLogDirectoryName, "") )
		strcpy (m_szLogDirectoryName, DEF_LOG_DIRECTORY_NAME);
	else
		strcpy (m_szLogDirectoryName, szLogDirectoryName);

	CreateDirectory (szLogDirectoryName, NULL());

	InitializeCriticalSection (& m_critLogToFile);
}


//*---------------------------------------------------------------
// TYPE : FUNCTION
// NAME : ~CCallStackTrace()
//*---------------------------------------------------------------
// DESC : CCallStackTrace 클래스 소멸자
// RETV : N/A
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
CCallStackTrace::~CCallStackTrace()
{
	DeleteCriticalSection (& m_critLogToFile);
}


//*---------------------------------------------------------------
// TYPE : FUNCTION
// NAME : CCallStackTrace::Output()
//*---------------------------------------------------------------
// DESC : m_strCallStackLog 에 저장된 로그를 파일에 쓴다.
// RETV : N/A
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
void CCallStackTrace::Output()
{
	if ( ! strcmp (m_szLogID, "") ) {
		MessageBox(HWND_DESKTOP, "NO LOG-ID !!!", "ERROR", MB_OK);
		return;
	}

	char szLogFileName [MAX_PATH];
	SYSTEMTIME strSystime;

	GetLocalTime (& strSystime);
	wsprintf (szLogFileName, "%s\\%s %04u-%02u-%02u.txt",
		m_szLogDirectoryName,
		m_szLogFileName,
		strSystime.wYear,
		strSystime.wMonth,
		strSystime.wDay
		);

	if ( ! ( m_fLogFile = fopen (szLogFileName, "a+") ) ) {
		return;
	}

	fprintf (m_fLogFile, "\nCallStack Trace Infomation (DATE:%04u-%02u-%02u %02u:%02u:%02u / ID:%s)\n", 
		strSystime.wYear,
		strSystime.wMonth,
		strSystime.wDay,
		strSystime.wHour,
		strSystime.wMinute,
		strSystime.wSecond,
		m_szLogID
		);
	fprintf (m_fLogFile, "\n*-----> CallStack List <-----*\n");
	

	for (int iC = 0 ; iC < m_strCallStackLog.size() ; iC++) {
		fputc(m_strCallStackLog[iC], m_fLogFile);
	}

	fclose (m_fLogFile);
}


//*---------------------------------------------------------------
// TYPE : FUNCTION
// NAME : CCallStackTrace::ConvertAddress()
//*---------------------------------------------------------------
// DESC : 주어진 가상 주소값을 소스 파일 이름과 라인 문자열로 변환한다.
// RETV : m_strCallStackLog 들어간 문자열의 길이
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
DWORD CCallStackTrace::ConvertAddress (
   HANDLE hProcess,			// 프로세스 핸들
   DWORD address,			// 가상 주소값
   char* output_buffer		// 호출스택 정보가 들어갈 버퍼
   )
{
    char* current_pointer = output_buffer;

    // Get the module name.
    IMAGEHLP_MODULE imagehlp_module;
    ::ZeroMemory(&imagehlp_module, sizeof(imagehlp_module));
    imagehlp_module.SizeOfStruct = sizeof(imagehlp_module);

    if (::SymGetModuleInfo(hProcess, address, &imagehlp_module) != 0) {
        // Strip off the path.
        char* image_name = strrchr(imagehlp_module.ImageName, '\\');
        if (image_name != NULL)
        {
            ++image_name;
        }
        else
        {
            image_name = imagehlp_module.ImageName;
        }

        current_pointer += sprintf(current_pointer, "%s : ", image_name);
    }
    else {
        current_pointer += sprintf(current_pointer, "<unknown module> : ");
    }

    // Get the function.
    DWORD displacement;

    char temp [MAX_PATH + sizeof(IMAGEHLP_SYMBOL)];
    IMAGEHLP_SYMBOL* imagehlp_symbol = reinterpret_cast<IMAGEHLP_SYMBOL*>(temp);
    ::ZeroMemory(imagehlp_symbol, MAX_PATH + sizeof(IMAGEHLP_SYMBOL));
    imagehlp_symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
    imagehlp_symbol->Address = address;
    imagehlp_symbol->MaxNameLength = MAX_PATH;

    if (::SymGetSymFromAddr(
        hProcess, address, &displacement, imagehlp_symbol) != 0
		)
    {
        current_pointer += sprintf(current_pointer, "%s() ", imagehlp_symbol->Name);

        // If I got a symbol, give the source and line a whirl.
        IMAGEHLP_LINE imagehlp_line;
        ZeroMemory(&imagehlp_line, sizeof(imagehlp_line));
        imagehlp_line.SizeOfStruct = sizeof(imagehlp_line);

        if (::SymGetLineFromAddr(
            hProcess, address, &displacement, &imagehlp_line) != 0)
        {
            current_pointer += 
                sprintf(current_pointer, "// %s(%u)", 
                imagehlp_line.FileName, imagehlp_line.LineNumber);
        }
    }
    else
    {
        current_pointer += sprintf(current_pointer, "<unknown symbol>");
    }

    // Tack on a CRLF.
    current_pointer += sprintf(current_pointer, "\r\n");

    return (DWORD)(current_pointer - output_buffer);
}


//*---------------------------------------------------------------
// TYPE : FUNCTION
// NAME : CCallStackTrace::SaveCallStack()
//*---------------------------------------------------------------
// DESC : TraceStack() 와의 인터페이스 (실제 호출자의 주소를 남기기 위해 호출)
// RETV : m_strCallStackLog 들어간 문자열의 길이 (-1이면 에러)
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
int CCallStackTrace::SaveCallStack (
	BOOL bWriteInFile		// 파일에 저장할 지 여부 (FALSE이면 나중에 GetCallStackLogBuffer() 로 버퍼포인터를 받아도 됨)
	)
{
	return TraceStack(bWriteInFile);
}

//*---------------------------------------------------------------
// TYPE : FUNCTION
// NAME : CCallStackTrace::TraceStack()
//*---------------------------------------------------------------
// DESC : stack trace를 수행하여, 문자열 버퍼에다 그 결과를 담는다.
// RETV : m_strCallStackLog 들어간 문자열의 길이 (-1이면 에러)
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
int CCallStackTrace::TraceStack (
	BOOL bWriteInFile		// 파일에 저장할 지 여부 (FALSE이면 나중에 GetCallStackLogBuffer() 로 버퍼포인터를 받아도 됨)
	)
{
	if ( ! strcmp (m_szLogID, "") ) {
		MessageBox(HWND_DESKTOP, "INSERT LOG-ID !!!", "ERROR", MB_OK);
		return -1;
	}

	m_strCallStackLog = "";

	EnterCriticalSection (& m_critLogToFile);

	int ret = 0;

#define LEAVE_RET(x) {ret = x; __leave;}

	__try {
		HANDLE hProcess = ::GetCurrentProcess();
		HANDLE hThread  = ::GetCurrentThread();

		DWORD option = ::SymGetOptions();
		option |= SYMOPT_LOAD_LINES;
		option &= ~SYMOPT_UNDNAME;
		::SymSetOptions(option | SYMOPT_LOAD_LINES);

		if (!::SymInitialize(hProcess, NULL, TRUE)) 	LEAVE_RET(-1);


		// The array of addresses.
		static ULONG addresses [8192] = {0,};
		unsigned int address_count = 0;

		// The thread information.
		CONTEXT context;
		::ZeroMemory(&context, sizeof(context));
		context.ContextFlags = CONTEXT_FULL;
		if (!::GetThreadContext(GetCurrentThread(), &context)) LEAVE_RET(-1);

		STACKFRAME stackframe;
		::ZeroMemory(&stackframe, sizeof(stackframe));

		stackframe.AddrPC.Mode      = AddrModeFlat;
		stackframe.AddrPC.Offset    = context.Eip;
		stackframe.AddrStack.Offset = context.Esp;
		stackframe.AddrStack.Mode   = AddrModeFlat;
		stackframe.AddrFrame.Offset = context.Ebp;
		stackframe.AddrFrame.Mode   = AddrModeFlat;

		// Loop for the first 512 stack elements.
		for (unsigned int i = 0; i < 512; ++i) {
			if (!::StackWalk(
				IMAGE_FILE_MACHINE_I386, // MachineType
				hProcess,                // hProcess
				hThread,                 // hThread
				&stackframe,             // StackFrame
				&context,                // ContextRecord
				NULL,                    // ReadMemoryRoutine
				SymFunctionTableAccess,  // FunctionTableAccessRoutine
				GetModuleBase,           // GetModuleBaseRoutine
				NULL                     // TranslateAddress
				))
			{
				break;
			}

			if (i > 1) {
				// Also check that the address is not zero. 
				// Sometimes StackWalk returns TRUE with a frame of zero.
				if (stackframe.AddrPC.Offset != 0)
				{
					addresses[address_count] = stackframe.AddrPC.Offset;
					++address_count;
				}
			}
		}

		// Now start converting the addresses.
		for (unsigned int addr_index = 0; 
			addr_index < address_count; ++addr_index)
		{
			char symbol[1024 + MAX_PATH] = {0, };

			DWORD symbol_size = ConvertAddress(hProcess, addresses[addr_index], symbol);

			m_strCallStackLog += "\t";
			m_strCallStackLog += symbol;
		}

		if (bWriteInFile && !m_strCallStackLog.empty()) {
			Output();
		}

		ret = m_strCallStackLog.size();
	}
	__finally {
		LeaveCriticalSection (& m_critLogToFile);
	}

	return ret;
}


//*---------------------------------------------------------------
// TYPE : FUNCTION
// NAME : CCallStackTrace::GetModuleBase()
//*---------------------------------------------------------------
// DESC : StackWalk API를 호출할 때 쓰이는 콜백 함수로서, 특정 프로세스의 베이스 값을 알아내는 역할을 한다.
// RETV : 프로세스의 베이스 주소
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
DWORD __stdcall CCallStackTrace::GetModuleBase (
	HANDLE hProcess,	// 프로세스 핸들
	DWORD address		// SymLoadModule 함수에 의해 로드된 모듈들 중 하나의 주소
	)
{
    IMAGEHLP_MODULE imagehlp_module;
    imagehlp_module.SizeOfStruct = sizeof(imagehlp_module);
    if (::SymGetModuleInfo(hProcess, address, &imagehlp_module) == TRUE)
        return imagehlp_module.BaseOfImage;

    MEMORY_BASIC_INFORMATION memory_basic_information;
    if (::VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(address), 
        &memory_basic_information, sizeof(memory_basic_information)) == 0
		) 
	{
        return 0;
    }

    char module_file_name [MAX_PATH];
    DWORD module_file_name_length = ::GetModuleFileName(
        reinterpret_cast<HMODULE>(memory_basic_information.AllocationBase),
        module_file_name, sizeof(module_file_name)
		);

    HANDLE file_handle;
    if (module_file_name_length != 0) {
        file_handle = ::CreateFile(module_file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    }
    else {
        file_handle = NULL;
    }

    char* image_name = NULL;
    if (module_file_name_length > 0) {
        image_name = module_file_name;
    }
    else {
        image_name = NULL;
    }

    ::SymLoadModule(hProcess, file_handle, image_name, NULL, reinterpret_cast<DWORD>(memory_basic_information.AllocationBase), 0);

    return reinterpret_cast<DWORD>(memory_basic_information.AllocationBase);
}

