#include <stdio.h>
#include <tchar.h>
#include "BaseException.h"

void Func5()
{
	CBaseException sw;
	sw.ShowCallstack();
}

void Func4()
{
	Func5();
}
void Func3()
{
	Func4();
}
void Func2()
{
	Func3();
}
void Func1()
{
	Func2();
}

void StackWalkTest()
{
	Func1();
}

void ExceptionTest()
{
	int i = 1;
	int c = i / 0;
}

void TryTest()
{
	try
	{
		SET_DEFAUL_EXCEPTION();
		int *p = NULL;
		*p = 0;
	}
	catch (CBaseException &e)
	{
		e.ShowExceptionInformation();
	}
}
int _tmain(int argc, TCHAR* argv[])
{
	SET_DEFULTER_HANDLER();
	CBaseException be;
	be.ShowLoadModules();
	StackWalkTest();
	TryTest();

	ExceptionTest();
	return 0;
}