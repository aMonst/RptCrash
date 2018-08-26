VC++ 程序崩溃日志记录
============================

该程序主要在程序崩溃时获取程序崩溃位置的寄存器和堆栈信息并记录到日志中
使用方式非常简单，只需要将MSJExceptionHandler.h 和 MSJExceptionHandler.cpp放入源码工程中，编译即可。
由于它定义了全局变量，所以原始程序不需要做任何修改，不需要使用#include来包含。
日志最终被记录到生成的EXE同级目录的.rpt文件中，直接使用记事本打开即可看到效果

main.cpp中提供一个简单的触发崩溃的条件
最终产生的文件内容大致如下:
```
//=====================================================
//=====================================================
Exception code: C0000005 ACCESS_VIOLATION
Fault address:  00F31588 02:00000588 e:\Program\Crash\Debug\Crash.exe

Registers:
EAX:00000000
EBX:7EFDE000
ECX:00000000
EDX:00000001
ESI:00000000
EDI:003CFC20
CS:EIP:0023:00F31588
SS:ESP:002B:003CFB48  EBP:003CFC20
DS:002B  ES:002B  FS:0053  GS:002B
Flags:00010212

Call stack:
Address   Frame     Logical addr  Module
00F31588  003CFC20  0002:00000588 e:\Program\Crash\Debug\Crash.exe
00F335B8  003CFC70  0002:000025B8 e:\Program\Crash\Debug\Crash.exe
00F333FF  003CFC78  0002:000023FF e:\Program\Crash\Debug\Crash.exe
769A343D  003CFC84  0001:0000343D C:\windows\syswow64\kernel32.dll
770F9832  003CFCC4  0001:00029832 C:\windows\SysWOW64\ntdll.dll
770F9805  003CFCDC  0001:00029805 C:\windows\SysWOW64\ntdll.dll
TraceBack:
当前调用函数 : 00f31560+wmain(FILE[e:\program\crash\crash\main.cpp]LINE[7])
当前调用函数 : 00f33410+__tmainCRTStartup(FILE[f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c]LINE[583])
当前调用函数 : 00f333f0+wmainCRTStartup(FILE[f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c]LINE[403])
当前调用函数 : 769a342b+BaseThreadInitThunk(FILE[f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c]LINE[403])
当前调用函数 : 770f97cf+RtlInitializeExceptionChain(FILE[f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c]LINE[403])
当前调用函数 : 770f97cf+RtlInitializeExceptionChain(FILE[f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c]LINE[403])
```

最后TraceBack部分记录的是崩溃时程序执行的位置，它能够精确定位到出错位置所在源文件的行数

程序的基础是由一个老外写的MSJExceptionHandler，原文我找不到了，这里只列举出我真实参考到的文章

参考资料:

[记录程序崩溃时的调用堆栈](https://blog.csdn.net/qing666888/article/details/41026375)

[windows平台调用函数堆栈的追踪方法](https://blog.csdn.net/lanuage/article/details/52203447)

## V1.0 版本
在上述基础之上增添了一些功能，改写了全部的代码结构

1. 程序主要分为2个类，CStackWalker 类主要负责获取模块和堆栈信息,CBaseException 主要处理异常的相关操作
2. CBaseException 中所有方法都可以重写，其中最重要的是重写OutputString, 特别是您希望将它写到文件中的时候，只需要重写该方法，将输出写入到文件中即可
3. CBaseException 类提供两种使用方法，可以使用SET_DEFULTER_HANDLER 宏来定义全局的异常处理函数或者使用SET_DEFAUL_EXCEPTION将异常转化为C++异常，具体的使用方法参考main.cpp文件
4. 修改之前版本中由于未加载全部符号表可能导致的获取函数调用堆栈失败的情况
