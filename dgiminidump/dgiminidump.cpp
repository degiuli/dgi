/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include <windows.h>
#include <shlwapi.h>
#include <dbghelp.h>

#include <string>
#include <exception>
#include <cstdlib>
#include <ctime>

#include "dgidefines.h"
#include "dgitracer.h"
#include "dgiminidump.h"

std::string g_basename;
dgi::DGITracer* g_ptracer = NULL;
LPTOP_LEVEL_EXCEPTION_FILTER g_pPreviousExceptionFilter;

void MiniDumpInit(dgi::DGITracer& tracer,std::string basename)
{
    if(basename.size()<=5)
    {
        throw std::invalid_argument("Invalid Base Name");
    }
    g_basename = basename;
    g_ptracer = &tracer;
    g_pPreviousExceptionFilter = SetUnhandledExceptionFilter(&AppUnhandledExceptionFilter);

    RedirectSetUnhandledExceptionFilter();
}

void MiniDumpEnd()
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)g_pPreviousExceptionFilter);
    g_ptracer = NULL;
}
    
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI ReportAttemptsToSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
    if(g_ptracer)
       g_ptracer->SendTrace(__LINE__,"-- Prevented attempt to set unhandled exception filter. lpTopLevelExceptionFilter: 0x%p", lpTopLevelExceptionFilter);
    return NULL;
}

BOOL RedirectSetUnhandledExceptionFilter()
{
    HMODULE hKernel32 = LoadLibrary("kernel32.dll");
    if (hKernel32 == NULL) 
        return FALSE;

    void *pOriginalFunc = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
    if (pOriginalFunc == NULL) 
        return FALSE;

    DWORD dwOriginalAddr = (DWORD) pOriginalFunc;
    dwOriginalAddr += 5; // add 5 for 5 op-codes for jmp far

    void *pDecoyFunc = &ReportAttemptsToSetUnhandledExceptionFilter;
    DWORD dwDecoyAddr = (DWORD) pDecoyFunc;
    DWORD dwRelativeAddr = dwDecoyAddr - dwOriginalAddr;

    unsigned char jump[ 100 ];
    jump[ 0 ] = 0xE9;  // JMP absolute
    memcpy(&jump[ 1 ], &dwRelativeAddr, sizeof(pDecoyFunc));
    SIZE_T bytesWritten;

    BOOL bRet = WriteProcessMemory(GetCurrentProcess(), pOriginalFunc, jump, sizeof(pDecoyFunc) + 1, &bytesWritten);
    return bRet;
}
    
void CreateMiniDump( LPEXCEPTION_POINTERS pExceptionInfo )
{
    time_t dumptime = time(NULL);
    std::string filename = g_basename + ctime((const time_t*)dumptime) + ".dmp";

    // Create the file first.
    HANDLE hFile = CreateFile(filename.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

    if ( hFile != INVALID_HANDLE_VALUE )
    {
        MINIDUMP_EXCEPTION_INFORMATION stMDEI = {0};
        MINIDUMP_EXCEPTION_INFORMATION * pMDEI = NULL;

        if ( pExceptionInfo != NULL )
        {
            stMDEI.ThreadId = GetCurrentThreadId();
            stMDEI.ExceptionPointers = pExceptionInfo;
            stMDEI.ClientPointers = TRUE;
            pMDEI = &stMDEI;
        }

        // Got the file open.  Write it.
        BOOL bRet = MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hFile,MiniDumpWithPrivateReadWriteMemory,pMDEI,NULL,NULL);
        if ( TRUE == bRet )
        {
            if(g_ptracer)
               g_ptracer->SendFatal(__LINE__,"-- CreateMiniDump, Created MiniDump file located at %s",filename);
        }
        else
        {
            if(g_ptracer)
               g_ptracer->SendFatal(__LINE__,"-- CreateMiniDump, Failed to create MiniDump file. Last Error: %u",GetLastError());
        }

        // Close the open file.
        CloseHandle(hFile);
    }
}

LONG WINAPI AppUnhandledExceptionFilter(LPEXCEPTION_POINTERS pExceptionInfo)
{
    if(g_ptracer)
       g_ptracer->SendFatal(__LINE__,"-- AppUnhndldExcptFltr, Attempt to create MiniDump file before exit proc due to unhndld exception.");

    CreateMiniDump(pExceptionInfo);
        
    //terminates the process
    if(g_ptracer)
       g_ptracer->SendFatal(__LINE__,"<< %s Terminte",g_basename.c_str());
    abort();
    return 0;
}
