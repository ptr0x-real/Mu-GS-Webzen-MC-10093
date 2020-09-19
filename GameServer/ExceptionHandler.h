#include <direct.h>
#include <imagehlp.h>
#include <stdio.h>

LPCTSTR GetExceptionString(DWORD dwExcept)
{
    switch ( dwExcept )
    {
	case EXCEPTION_ACCESS_VIOLATION:
		return "EXCEPTION_ACCESS_VIOLATION";

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return "EXCEPTION_DATATYPE_MISALIGNMENT";

	case EXCEPTION_BREAKPOINT:
		return "EXCEPTION_BREAKPOINT";

	case EXCEPTION_SINGLE_STEP:
		return "EXCEPTION_SINGLE_STEP";

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return "EXCEPTION_FLT_DENORMAL_OPERAND";

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return "EXCEPTION_FLT_DIVIDE_BY_ZERO";

	case EXCEPTION_FLT_INEXACT_RESULT:
		return "EXCEPTION_FLT_INEXACT_RESULT";

	case EXCEPTION_FLT_INVALID_OPERATION:
		return "EXCEPTION_FLT_INVALID_OPERATION";

	case EXCEPTION_FLT_OVERFLOW:
		return "EXCEPTION_FLT_OVERFLOW";

	case EXCEPTION_FLT_STACK_CHECK:
		return "EXCEPTION_FLT_STACK_CHECK";

	case EXCEPTION_FLT_UNDERFLOW:
		return "EXCEPTION_FLT_UNDERFLOW";

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return "EXCEPTION_INT_DIVIDE_BY_ZERO";

	case EXCEPTION_INT_OVERFLOW:
		return "EXCEPTION_INT_OVERFLOW";

	case EXCEPTION_PRIV_INSTRUCTION:
		return "EXCEPTION_PRIV_INSTRUCTION";

	case EXCEPTION_IN_PAGE_ERROR:
		return "EXCEPTION_IN_PAGE_ERROR";

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return "EXCEPTION_ILLEGAL_INSTRUCTION";

	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return "EXCEPTION_NONCONTINUABLE_EXCEPTION";

	case EXCEPTION_STACK_OVERFLOW:
		return "EXCEPTION_STACK_OVERFLOW";

	case EXCEPTION_INVALID_DISPOSITION:
		return "EXCEPTION_INVALID_DISPOSITION";

	case EXCEPTION_GUARD_PAGE:
		return "EXCEPTION_GUARD_PAGE";

	case EXCEPTION_INVALID_HANDLE:
		return "EXCEPTION_INVALID_HANDLE";

	default:
		return NULL;
    }
}



#define SYM_BUFF_SIZE 512
static BYTE g_stSymbol[SYM_BUFF_SIZE];
static char g_ExceptionBuffer[256];
static SYSTEMTIME g_Exception_systime;

extern	void	GameClose();

LONG __stdcall MyExceptionFilter(EXCEPTION_POINTERS* pExPtrs)
{
	strcpy(g_ExceptionBuffer, ".\\BugReport");
	_mkdir(g_ExceptionBuffer);
	GetLocalTime(&g_Exception_systime);
	sprintf(g_ExceptionBuffer, ".\\BugReport\\%04d-%02d-%02d-%02d-%02d-%02d.txt",
		g_Exception_systime.wYear, g_Exception_systime.wMonth,
		g_Exception_systime.wDay, g_Exception_systime.wHour,
		g_Exception_systime.wMinute, g_Exception_systime.wSecond);

	HANDLE hFile = CreateFile(g_ExceptionBuffer, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	BOOL g_bSymEngInit = FALSE;
	DWORD dwOpts = SymGetOptions();
	// Always defer loading to make life faster.
	SymSetOptions(dwOpts | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
	// Initialize the symbol engine.
	g_bSymEngInit = SymInitialize(GetCurrentProcess(), NULL, TRUE);


	EXCEPTION_RECORD* pRecord = pExPtrs->ExceptionRecord;
	CONTEXT* pContext = pExPtrs->ContextRecord;

	DWORD written;

	sprintf(g_ExceptionBuffer, "---------------------------\r\n");
	WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

	sprintf(g_ExceptionBuffer, "# Exception : %s\r\n", GetExceptionString(pRecord->ExceptionCode));
	WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

	sprintf(g_ExceptionBuffer, "# Address : 0x%08X\r\n", pRecord->ExceptionAddress);
	WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

	sprintf(g_ExceptionBuffer, "# Registers\r\n");
	WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

    // This puts 48 bytes on the stack.  This could be a problem when
    //  the stack is blown.
    sprintf(g_ExceptionBuffer,"EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X  ESI=%08X\r\n"\
                "EDI=%08X  EBP=%08X  ESP=%08X  EIP=%08X  FLG=%08X\r\n"\
                "CS=%04X   DS=%04X   SS=%04X   ES=%04X   FS=%04X   GS=%04X\r\n",
				pExPtrs->ContextRecord->Eax,
				pExPtrs->ContextRecord->Ebx,
				pExPtrs->ContextRecord->Ecx,
				pExPtrs->ContextRecord->Edx,
				pExPtrs->ContextRecord->Esi,
				pExPtrs->ContextRecord->Edi,
				pExPtrs->ContextRecord->Ebp,
				pExPtrs->ContextRecord->Esp,
				pExPtrs->ContextRecord->Eip,
				pExPtrs->ContextRecord->EFlags,
				pExPtrs->ContextRecord->SegCs,
				pExPtrs->ContextRecord->SegDs,
				pExPtrs->ContextRecord->SegSs,
				pExPtrs->ContextRecord->SegEs,
				pExPtrs->ContextRecord->SegFs,
				pExPtrs->ContextRecord->SegGs);
	WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);


#define SAVE_EBP(f)        f.Reserved[0]
#define TRAP_TSS(f)        f.Reserved[1]
#define TRAP_EDITED(f)     f.Reserved[1]
#define SAVE_TRAP(f)       f.Reserved[2]

	PIMAGEHLP_SYMBOL sym = (PIMAGEHLP_SYMBOL)&g_stSymbol;
	//lint +e545
	ZeroMemory(sym , SYM_BUFF_SIZE);
	sym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
	sym->MaxNameLength = SYM_BUFF_SIZE - sizeof(IMAGEHLP_SYMBOL);

	DWORD dwDisplacement = 0;
	DWORD frames = 0;
	LPSTR szSymName;
	IMAGEHLP_MODULE mi;
	DWORD i;
	DWORD machType;
	STACKFRAME stk;
	ZeroMemory(&stk, sizeof(stk));
    stk.AddrPC.Offset       = pContext->Eip;
    stk.AddrPC.Mode         = AddrModeFlat;
    stk.AddrStack.Offset    = pContext->Esp;
    stk.AddrStack.Mode      = AddrModeFlat;
    stk.AddrFrame.Offset    = pContext->Ebp;
    stk.AddrFrame.Mode      = AddrModeFlat;
	machType = IMAGE_FILE_MACHINE_I386;

	sprintf(g_ExceptionBuffer, "# Call Stack Dump\r\n");
	WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

	for (i=0; i<100; i++)
	{
		if (!StackWalk(machType, GetCurrentProcess(), GetCurrentThread(), &stk, pContext,
			NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
//		if (!StackWalk(machType, (HANDLE)dp, NULL, &stk, &Context, SwReadProcessMemory, SwFunctionTableAccess,
//			SwGetModuleBase, SwTranslateAddress))
		{
			break;
        }
		if (SymGetSymFromAddr(GetCurrentProcess(), stk.AddrPC.Offset, &dwDisplacement, sym))
		{
			szSymName = sym->Name;
        }
		else
		{
			szSymName = "<nosymbols>";
		} 

		sprintf(g_ExceptionBuffer, "%04X:%08X  ", pExPtrs->ContextRecord->SegCs,
			stk.AddrPC.Offset);
		WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

		sprintf(g_ExceptionBuffer, "%08X %08X  [%08X %08X %08X %08X] %08X ",
			stk.AddrFrame.Offset, stk.AddrReturn.Offset, stk.Params[0], 
			stk.Params[1], stk.Params[2], stk.Params[3], stk.AddrPC.Offset - 0x00400000 - 0x1000);
		WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

		if (SymGetModuleInfo(GetCurrentProcess(), stk.AddrPC.Offset, &mi))
		{
			sprintf(g_ExceptionBuffer, "%s!", mi.ModuleName);
			WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
		}

		sprintf(g_ExceptionBuffer, "%s ", szSymName);
		WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

		if (sym && (sym->Flags & SYMF_OMAP_GENERATED || sym->Flags & SYMF_OMAP_MODIFIED))
		{
			sprintf(g_ExceptionBuffer, "[omap] ");
			WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
		}  


		if (stk.FuncTableEntry && machType == IMAGE_FILE_MACHINE_I386)
		{
			PFPO_DATA pFpoData = (PFPO_DATA)stk.FuncTableEntry;
			switch (pFpoData->cbFrame)
			{
			case FRAME_FPO:
				if (pFpoData->fHasSEH)
				{
					sprintf(g_ExceptionBuffer, "(FPO: [SEH])");
					WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
				}
				else
				{
					sprintf(g_ExceptionBuffer, " (FPO:" );
					WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
					if (pFpoData->fUseBP)
					{
						sprintf(g_ExceptionBuffer, " [EBP 0x%08x]", SAVE_EBP(stk));
						WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
					}

					sprintf(g_ExceptionBuffer, " [%d,%d,%d])", pFpoData->cdwParams, pFpoData->cdwLocals, pFpoData->cbRegs);
					WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
				}
				break;
			case FRAME_NONFPO:
				sprintf(g_ExceptionBuffer, "(FPO: Non-FPO [%d,%d,%d])", pFpoData->cdwParams, pFpoData->cdwLocals, pFpoData->cbRegs);
				WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
				break;
			case FRAME_TRAP:
			case FRAME_TSS:
			default:
				sprintf(g_ExceptionBuffer, "(UNKNOWN FPO TYPE)" );
				WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
				break;
			}
		}

		FPO_DATA *pFPOData = (FPO_DATA *) stk.FuncTableEntry;

		if (pFPOData != NULL)
		{
			sprintf(g_ExceptionBuffer, "0x08X 0x08X, 0x08X, 0x04X", pFPOData->ulOffStart,
				pFPOData->cbProcSize, pFPOData->cdwLocals, pFPOData->cdwParams);
			WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
		}
		sprintf(g_ExceptionBuffer, "\r\n" );
		WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);
	}

	sprintf(g_ExceptionBuffer, "\r\n" );
	WriteFile(hFile, g_ExceptionBuffer, lstrlen(g_ExceptionBuffer), &written, NULL);

	CloseHandle(hFile);

	if (TRUE == g_bSymEngInit)
		SymCleanup(GetCurrentProcess());

	
	return EXCEPTION_EXECUTE_HANDLER;
}

class _InternalExceptionHandlerClass
{
public:
	_InternalExceptionHandlerClass()
	{
		SetUnhandledExceptionFilter(MyExceptionFilter);
	}
};

//static _InternalExceptionHandlerClass g_InternalExceptionHandlerClass;

