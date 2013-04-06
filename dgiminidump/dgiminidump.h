/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef _DGI_MINIDUMP_
#define _DGI_MINIDUMP_

#include <windows.h>
#include <memory>
#include "dgitracer.h"

void MiniDumpInit(dgi::DGITracer& tracer,std::string basename);
void MiniDumpEnd();
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI ReportAttemptsToSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
BOOL RedirectSetUnhandledExceptionFilter();
void CreateMiniDump(LPEXCEPTION_POINTERS pExceptionInfo);
long WINAPI AppUnhandledExceptionFilter(LPEXCEPTION_POINTERS pExceptionInfo);

#endif  //_DGI_MINIDUMP_