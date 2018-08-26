#include "BaseException.h"

CBaseException::CBaseException(HANDLE hProcess, WORD wPID, LPCTSTR lpSymbolPath, PEXCEPTION_POINTERS pEp):
	CStackWalker(hProcess, wPID, lpSymbolPath)
{
	if (NULL != pEp)
	{
		m_pEp = new EXCEPTION_POINTERS;
		CopyMemory(m_pEp, pEp, sizeof(EXCEPTION_POINTERS));
	}
}

CBaseException::~CBaseException(void)
{
}

void CBaseException::OutputString(LPCTSTR lpszFormat, ...)
{
	TCHAR szBuf[1024] = _T("");
	va_list args;
	va_start(args, lpszFormat);
	_vsntprintf_s(szBuf, 1024, lpszFormat, args);
	va_end(args);

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szBuf, _tcslen(szBuf), NULL, NULL);
}

void CBaseException::ShowLoadModules()
{
	LoadSymbol();
	LPMODULE_INFO pHead = GetLoadModules();
	LPMODULE_INFO pmi = pHead;

	TCHAR szBuf[MAX_COMPUTERNAME_LENGTH] = _T("");
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH;
	GetUserName(szBuf, &dwSize);
	OutputString(_T("Current User:%s\r\n"), szBuf);
	OutputString(_T("BaseAddress:\tSize:\tName\tPath\tSymbolPath\tVersion\r\n"));
	while (NULL != pmi)
	{
		OutputString(_T("%08x\t%d\t%s\t%s\t%s\t%s\r\n"), (unsigned long)(pmi->ModuleAddress), pmi->dwModSize, pmi->szModuleName, pmi->szModulePath, pmi->szSymbolPath, pmi->szVersion);
		pmi = pmi->pNext;
	}

	FreeModuleInformations(pHead);
}

void CBaseException::ShowCallstack(HANDLE hThread, const CONTEXT* context)
{
	OutputString(_T("Show CallStack:\r\n"));
	LPSTACKINFO phead = StackWalker(hThread, context);
	FreeStackInformations(phead);
}

void CBaseException::ShowExceptionResoult(DWORD dwExceptionCode)
{
	OutputString(_T("Exception Code :%08x "), dwExceptionCode);
	switch (dwExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		{
			OutputString(_T("ACCESS_VIOLATION(%s)\r\n"), _T("��д�Ƿ��ڴ�"));
		}
		return ;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		{
			OutputString(_T("DATATYPE_MISALIGNMENT(%s)\r\n"), _T("�߳���ͼ�ڲ�֧�ֶ����Ӳ���϶�дδ���������"));
		}
		return ;
	case EXCEPTION_BREAKPOINT:
		{
			OutputString(_T("BREAKPOINT(%s)\r\n"), _T("����һ���ϵ�"));
		}
		return ;
	case EXCEPTION_SINGLE_STEP:
		{
			OutputString(_T("SINGLE_STEP(%s)\r\n"), _T("����")); //һ���Ƿ����ڵ����¼���
		}
		return ;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		{
			OutputString(_T("ARRAY_BOUNDS_EXCEEDED(%s)\r\n"), _T("�������Խ��"));
		}
		return ;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		{
			OutputString(_T("FLT_DENORMAL_OPERAND(%s)\r\n"), _T("���������һ�������������棬�����ĸ������޷���ʾ")); //������������
		}
		return ;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		{
			OutputString(_T("FLT_DIVIDE_BY_ZERO(%s)\r\n"), _T("��������0����"));
		}
		return ;
	case EXCEPTION_FLT_INEXACT_RESULT:
		{
			OutputString(_T("FLT_INEXACT_RESULT(%s)\r\n"), _T("�����������Ľ���޷���ʾ")); //�޷���ʾһ��������̫С��������������ʾ�ķ�Χ, ����֮������Ľ���쳣
		}
		return ;
	case EXCEPTION_FLT_INVALID_OPERATION:
		{
			OutputString(_T("FLT_INVALID_OPERATION(%s)\r\n"), _T("�����������쳣"));
		}
		return ;
	case EXCEPTION_FLT_OVERFLOW:
		{
			OutputString(_T("FLT_OVERFLOW(%s)\r\n"), _T("���������ָ����������Ӧ���͵����ֵ"));
		}
		return ;
	case EXCEPTION_FLT_STACK_CHECK:
		{
			OutputString(_T("STACK_CHECK(%s)\r\n"), _T("ջԽ�����ջ�������"));
		}
		return ;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		{
			OutputString(_T("INT_DIVIDE_BY_ZERO(%s)\r\n"), _T("������0�쳣"));
		}
		return ;
	case EXCEPTION_INVALID_HANDLE:
		{
			OutputString(_T("INVALID_HANDLE(%s)\r\n"), _T("�����Ч"));
		}
		return ;
	case EXCEPTION_PRIV_INSTRUCTION:
		{
			OutputString(_T("PRIV_INSTRUCTION(%s)\r\n"), _T("�߳���ͼִ�е�ǰ����ģʽ��֧�ֵ�ָ��"));
		}
		return ;
	case EXCEPTION_IN_PAGE_ERROR:
		{
			OutputString(_T("IN_PAGE_ERROR(%s)\r\n"), _T("�߳���ͼ����δ���ص������ڴ�ҳ���߲��ܼ��ص������ڴ�ҳ"));
		}
		return ;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		{
			OutputString(_T("ILLEGAL_INSTRUCTION(%s)\r\n"), _T("�߳���ͼִ����Чָ��"));
		}
		return ;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		{
			OutputString(_T("NONCONTINUABLE_EXCEPTION(%s)\r\n"), _T("�߳���ͼ��һ�����ɼ���ִ�е��쳣���������ִ��"));
		}
		return ;
	case EXCEPTION_STACK_OVERFLOW:
		{
			OutputString(_T("STACK_OVERFLOW(%s)\r\n"), _T("ջ���"));
		}
		return ;
	case EXCEPTION_INVALID_DISPOSITION:
		{
			OutputString(_T("INVALID_DISPOSITION(%s)\r\n"), _T("�쳣���������쳣������������һ����Ч����")); //ʹ�ø߼����Ա�д�ĳ�����Զ������������쳣
		}
		return ;
	case EXCEPTION_FLT_UNDERFLOW:
		{
			OutputString(_T("FLT_UNDERFLOW(%s)\r\n"), _T("������������ָ��С����Ӧ���͵���Сֵ"));
		}
		return ;
	case EXCEPTION_INT_OVERFLOW:
		{
			OutputString(_T("INT_OVERFLOW(%s)\r\n"), _T("��������Խ��"));
		}
		return ;
	}

	TCHAR szBuffer[512] = { 0 };

	FormatMessage(  FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
		GetModuleHandle( _T("NTDLL.DLL") ),
		dwExceptionCode, 0, szBuffer, sizeof( szBuffer ), 0 );

	OutputString(_T("%s"), szBuffer);
	OutputString(_T("\r\n"));
}

LONG WINAPI CBaseException::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo )
{
	CBaseException base(GetCurrentProcess(), GetCurrentProcessId(), NULL, pExceptionInfo);
	base.ShowExceptionInformation();
	
	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL CBaseException::GetLogicalAddress(
	PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset )
{
	MEMORY_BASIC_INFORMATION mbi;

	if ( !VirtualQuery( addr, &mbi, sizeof(mbi) ) )
		return FALSE;

	DWORD hMod = (DWORD)mbi.AllocationBase;

	if ( !GetModuleFileName( (HMODULE)hMod, szModule, len ) )
		return FALSE;

	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

	DWORD rva = (DWORD)addr - hMod;

	//���㵱ǰ��ַ�ڵڼ�����
	for (unsigned i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++ )
	{
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		if ( (rva >= sectionStart) && (rva <= sectionEnd) )
		{
			section = i+1;
			offset = rva - sectionStart;
			return TRUE;
		}
	}

	return FALSE;   // Should never get here!
}

void CBaseException::ShowRegistorInformation(PCONTEXT pCtx)
{
#ifdef _M_IX86  // Intel Only!
	OutputString( _T("\nRegisters:\r\n") );

	OutputString(_T("EAX:%08X\r\nEBX:%08X\r\nECX:%08X\r\nEDX:%08X\r\nESI:%08X\r\nEDI:%08X\r\n"),
		pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx, pCtx->Esi, pCtx->Edi );

	OutputString( _T("CS:EIP:%04X:%08X\r\n"), pCtx->SegCs, pCtx->Eip );
	OutputString( _T("SS:ESP:%04X:%08X  EBP:%08X\r\n"),pCtx->SegSs, pCtx->Esp, pCtx->Ebp );
	OutputString( _T("DS:%04X  ES:%04X  FS:%04X  GS:%04X\r\n"), pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
	OutputString( _T("Flags:%08X\r\n"), pCtx->EFlags );

#endif

	OutputString( _T("\r\n") );
}

void CBaseException::STF(unsigned int ui,  PEXCEPTION_POINTERS pEp)
{
	CBaseException base(GetCurrentProcess(), GetCurrentProcessId(), NULL, pEp);
	throw base;
}

void CBaseException::ShowExceptionInformation()
{
	OutputString(_T("Exceptions:\r\n"));
	ShowExceptionResoult(m_pEp->ExceptionRecord->ExceptionCode);
	TCHAR szFaultingModule[MAX_PATH];
	DWORD section, offset;
	GetLogicalAddress(m_pEp->ExceptionRecord->ExceptionAddress, szFaultingModule, sizeof(szFaultingModule), section, offset );
	OutputString( _T("Fault address:  %08X %02X:%08X %s\r\n"), m_pEp->ExceptionRecord->ExceptionAddress, section, offset, szFaultingModule );

	ShowRegistorInformation(m_pEp->ContextRecord);

	ShowCallstack(GetCurrentThread(), m_pEp->ContextRecord);
}