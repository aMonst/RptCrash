#pragma once
#include <Windows.h>
#include <tchar.h>
#include <DbgHelp.h>
#include <stddef.h>
#include <AtlConv.h>

#pragma comment(lib, "Dbghelp.lib")

typedef struct tag_MSJSYMBOL_INFO
{
	IMAGEHLP_SYMBOL symInfo;
	TCHAR szBuffer[MAX_PATH];
} MSJSYMBOL_INFO, *LPMSJSYMBOL_INFO;

class CMSJExceptionHandler
{
public:

	CMSJExceptionHandler( );
	~CMSJExceptionHandler( );

	void SetLogFileName( PTSTR pszLogFileName );

private:

	// entry point where control comes on an unhandled exception
	static LONG WINAPI MSJUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo );

	// where report info is extracted and generated 
	static void GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );

	// Helper functions
	static LPTSTR GetExceptionString(DWORD dwCode);
	static BOOL GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset);
	static void IntelStackWalk( PCONTEXT pContext );
	static int __cdecl _tprintf(const TCHAR * format, ...);
	static BOOL GetTraceBack(PCONTEXT pContext);

	// Variables used by the class
	static TCHAR m_szLogFileName[MAX_PATH];
	static LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
	static HANDLE m_hReportFile;
	static HANDLE m_hThread;
	static HANDLE m_hProcess;
};

extern CMSJExceptionHandler g_MSJExceptionHandler;   //  global instance of class