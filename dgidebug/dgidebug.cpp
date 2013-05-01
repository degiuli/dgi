/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <dbghelp.h>
#include <eh.h>
#include <sstream>

PROCESS_INFORMATION processInfo = {0};

WCHAR * ReadRemoteString(HANDLE process, LPVOID address, WORD length, WORD unicode)
{
    wchar_t * msg = new wchar_t[length];
    if (!msg) return NULL;
    memset(msg, 0, sizeof(wchar_t)*(length));

    if ( unicode )
    {
        ReadProcessMemory(process, address, msg, length*sizeof(wchar_t),NULL);  
        return msg;
    } 
    else
    {
        char * originalMsg = new char[length];
        if (!originalMsg)
        {
            delete[] msg;
            return NULL;
        }
        memset(originalMsg, 0, sizeof(BYTE)*(length));
        
        ReadProcessMemory(process, address, originalMsg, length,NULL);

        //convert to unicode
        std::mbstate_t state = std::mbstate_t();
        std::mbsrtowcs(msg,(const char**)&originalMsg,length,&state);
        return msg;
    }
}

WCHAR ReadCharW(HANDLE process, LPVOID address)
{
    wchar_t result = '\0';
    ReadProcessMemory(process, address, &result, sizeof(WCHAR),NULL);
    return result;
};

WCHAR ReadCharA(HANDLE process, LPVOID address)
{
    wchar_t result = '\0';
    ReadProcessMemory(process, address, &result, sizeof(CHAR),NULL);
    return result;
};

WCHAR * ReadRemoteSZ(HANDLE process, LPVOID address, WORD unicode)
{
    if (NULL == address) return NULL;
    wchar_t * msg = 0 ;
    if ( unicode )
    {
        wchar_t * ptr = (wchar_t*)address;
        size_t stringLen = 0;
        while(ReadCharW(process, ptr++)) stringLen++; 
        msg = new wchar_t[stringLen+1];
        if (!msg) return NULL;
        memset(msg, 0, sizeof(wchar_t)*(stringLen+1));
        ReadProcessMemory(process, address, msg, (stringLen+1)*sizeof(wchar_t),NULL);  
        return msg;
    } 
    else
    {
        char * ptr = (char*)address;
        size_t stringLen = 0;
        while(ReadCharA(process, ptr++)) stringLen++; 

        char * originalMsg = new char[stringLen+1];
        if (!originalMsg) return NULL;
        memset(originalMsg, 0, sizeof(BYTE)*(stringLen+1));
        
        msg = new wchar_t[stringLen+1]; 
        if (!msg) 
        {
            delete[] originalMsg;
            return NULL;
        }
        memset(msg, 0, sizeof(wchar_t)*(stringLen+1));

        ReadProcessMemory(process, address, originalMsg, stringLen+1,NULL);    

        //convert to unicode
        std::mbstate_t state = std::mbstate_t();
        std::mbsrtowcs(msg,(const char**)&originalMsg,stringLen,&state);
        return msg;
    }
}

VOID * ReadRemotePtr(HANDLE process, LPVOID address)
{
    if (NULL == address) return NULL;
    void* newPointer = 0 ;
    if (!ReadProcessMemory(process, address ,&newPointer, sizeof(void *),NULL))
    {
        newPointer=NULL;
    };  
    return newPointer;
}

char* GetExceptionString(DWORD dwExceptionCode)
{
	switch (dwExceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:
			return "Access Violation";
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			return "Datatype Misalignment";
		case EXCEPTION_BREAKPOINT:
			return "Breakpoint";
		case EXCEPTION_SINGLE_STEP:
			return "Single Step";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			return "Array Bounds Exceeded";
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			return "Flt Denormal Operand";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			return "Flt Divide By Zero";
		case EXCEPTION_FLT_INEXACT_RESULT:
			return "Flt Inexact Result";
		case EXCEPTION_FLT_INVALID_OPERATION:
			return "Flt Invalid Operation";
		case EXCEPTION_FLT_OVERFLOW:
			return "Flt Overflow";
		case EXCEPTION_FLT_STACK_CHECK:
			return "Flt Stack Check";
		case EXCEPTION_FLT_UNDERFLOW:
			return "Flt Underflow";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			return "Int Divide By Zero";
		case EXCEPTION_INT_OVERFLOW:
			return "Int Overflow";
		case EXCEPTION_PRIV_INSTRUCTION:
			return "Priv Instruction";
		case EXCEPTION_IN_PAGE_ERROR:
			return "In Page Error";
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			return "Illegal Instruction";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			return "Noncontinuable Exception";
		case EXCEPTION_STACK_OVERFLOW:
			return "Stack Overflow";
		case EXCEPTION_INVALID_DISPOSITION:
			return "Invalid Disposition";
		case EXCEPTION_GUARD_PAGE:
			return "Guard Page";
		case EXCEPTION_INVALID_HANDLE:
			return "Invalid Handle";
		//case EXCEPTION_POSSIBLE_DEADLOCK:
		//	return "Possible Deadlock";
		default:
			return "Unknown Win32 Exception";
	}
}

std::string SetExceptionDescription(EXCEPTION_RECORD& exceptionRecord)
{
    std::stringstream stringBuf;

    stringBuf << GetExceptionString(exceptionRecord.ExceptionCode);
    stringBuf << " (0x";
    stringBuf << std::hex << exceptionRecord.ExceptionCode;
    stringBuf << ") at 0x";
    stringBuf << std::hex << exceptionRecord.ExceptionAddress;

    if((exceptionRecord.ExceptionCode == EXCEPTION_ACCESS_VIOLATION) ||
       (exceptionRecord.ExceptionCode == EXCEPTION_IN_PAGE_ERROR))
    {
        switch (exceptionRecord.ExceptionInformation[0])
        {
        case 0:
            stringBuf << " reading";
            break;
        case 1:
            stringBuf << " writing";
            break;
        case 8:
            stringBuf << " DEP violation";
            break;
        default:
            stringBuf << " unknown operation (" << exceptionRecord.ExceptionInformation[0] << ")";
            break;
        }

        stringBuf << " location 0x" << std::hex << exceptionRecord.ExceptionInformation[1];
    }

    if (exceptionRecord.ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
    {
        stringBuf << " with NTSTATUS code of 0x" << std::hex << exceptionRecord.ExceptionInformation[2];
    }

    std::string exceptionDescription = stringBuf.str();
    return exceptionDescription;
}

void CreateMinidump(DEBUG_EVENT& dbgEvent)
{
    if(IsDebuggerPresent())
    {
        DebugBreak();
    }

    SYSTEMTIME st = {0};
    GetLocalTime(&st);
    std::wstringstream ss;
    ss << L"PID_" << std::dec << processInfo.dwProcessId;
    ss << L"_TID_" << std::dec << dbgEvent.dwThreadId;
    ss << L"_" << std::dec << st.wYear;
    ss << L"y" << std::dec << st.wMonth;
    ss << L"m" << std::dec << st.wDay;
    ss << L"d_" << std::dec << st.wHour;
    ss << L"h" << std::dec << st.wMinute;
    ss << L"m" << std::dec << st.wSecond;
    ss << L"s" << std::dec << st.wMilliseconds;
    ss << L"ms.dmp";
    std::wstring dumpfile = ss.str();

    //attempt to create dump file
    HANDLE hFile = CreateFile(dumpfile.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(hFile!=INVALID_HANDLE_VALUE )
    {
        MINIDUMP_EXCEPTION_INFORMATION stMDEI = {0};
        stMDEI.ThreadId = dbgEvent.dwThreadId;
        stMDEI.ExceptionPointers = reinterpret_cast<PEXCEPTION_POINTERS>(&dbgEvent.u.Exception.ExceptionRecord);
        stMDEI.ClientPointers = TRUE;
        MINIDUMP_EXCEPTION_INFORMATION * pMDEI = &stMDEI;

        //write the dump file
        BOOL bRet = MiniDumpWriteDump(processInfo.hProcess,processInfo.dwProcessId,hFile,MiniDumpWithPrivateReadWriteMemory,pMDEI,NULL,NULL);
        if ( TRUE == bRet )
        {
            std::wcout << L"\tCreated " << dumpfile.c_str() << L" minidump file" << std::endl;
        }
        else
        {
            std::wcout << L"\tFailed to create " << dumpfile.c_str() << L" minidump file, last error " << GetLastError() << std::endl;
        }

        //close opened file
        CloseHandle(hFile);
    }
    else
    {
        std::wcout << L"\t" << dumpfile.c_str() << L" minidump file not created, last error " << GetLastError() << std::endl;
    }
}

DWORD ProcessEvent(DEBUG_EVENT& dbgEvent)
{
    std::cout << "-- New event generated by PID.TID=" << dbgEvent.dwProcessId << "." << dbgEvent.dwThreadId << std::endl;

    switch (dbgEvent.dwDebugEventCode)
    {
        case EXCEPTION_DEBUG_EVENT:
            {
                //typedef struct _EXCEPTION_DEBUG_INFO {
                //    EXCEPTION_RECORD ExceptionRecord;
                //    DWORD dwFirstChance;
                //} EXCEPTION_DEBUG_INFO;

                std::string exceptionDescription = SetExceptionDescription(dbgEvent.u.Exception.ExceptionRecord);
                std::cout << "\tExceptionEvent:\n\t" << exceptionDescription << std::endl;

                switch (dbgEvent.u.Exception.ExceptionRecord.ExceptionCode)
                {
                    case EXCEPTION_BREAKPOINT:
                    case EXCEPTION_SINGLE_STEP:
                    case 0x4000001f:
                    case 0x4000001e:
                        return DBG_CONTINUE;
                    default:
                        CreateMinidump(dbgEvent);
                        break;
                }
            }
            return DBG_EXCEPTION_NOT_HANDLED;

        case CREATE_THREAD_DEBUG_EVENT:

            //typedef struct _CREATE_THREAD_DEBUG_INFO {
            //    HANDLE hThread;
            //    LPVOID lpThreadLocalBase;
            //    LPTHREAD_START_ROUTINE lpStartAddress;
            //} CREATE_THREAD_DEBUG_INFO;
            std::cout << "\tCreateThreadEvent TID=" << dbgEvent.dwThreadId << std::endl;
            break;

        case CREATE_PROCESS_DEBUG_EVENT:
            //typedef struct _CREATE_PROCESS_DEBUG_INFO {
            //    HANDLE hFile;
            //    HANDLE hProcess;
            //    HANDLE hThread;
            //    LPVOID lpBaseOfImage;
            //    DWORD dwDebugInfoFileOffset;
            //    DWORD nDebugInfoSize;
            //    LPVOID lpThreadLocalBase;
            //    LPTHREAD_START_ROUTINE lpStartAddress;
            //    LPVOID lpImageName;
            //    WORD fUnicode;
            //} CREATE_PROCESS_DEBUG_INFO, *LPCREATE_PROCESS_DEBUG_INFO;
            std::cout << "\tCreateProcessEvent PID=" << dbgEvent.dwProcessId << std::endl;
            break;

        case EXIT_THREAD_DEBUG_EVENT:
            //typedef struct _EXIT_THREAD_DEBUG_INFO {
            //    DWORD dwExitCode;
            //} EXIT_THREAD_DEBUG_INFO, *LPEXIT_THREAD_DEBUG_INFO;
            std::cout << "\tExitThreadEvent TID=" << dbgEvent.dwThreadId << " ExitCode=" << dbgEvent.u.ExitThread.dwExitCode << std::endl;
            break;

        case EXIT_PROCESS_DEBUG_EVENT:
            //typedef struct _EXIT_PROCESS_DEBUG_INFO {
            //    DWORD dwExitCode;
            //} EXIT_PROCESS_DEBUG_INFO, *LPEXIT_PROCESS_DEBUG_INFO;
            std::cout << "\tExitProcessEvent ExitCode=" << std::hex << dbgEvent.u.ExitProcess.dwExitCode << std::endl;
            return 0 ;
            break;

        case LOAD_DLL_DEBUG_EVENT:
            //typedef struct _LOAD_DLL_DEBUG_INFO {
            //    HANDLE hFile;
            //    LPVOID lpBaseOfDll;
            //    DWORD dwDebugInfoFileOffset;
            //    DWORD nDebugInfoSize;
            //    LPVOID lpImageName;
            //    WORD fUnicode;
            //} LOAD_DLL_DEBUG_INFO, *LPLOAD_DLL_DEBUG_INFO;
            {    
                std::cout << "\tLoadDllEvent ";
                LPVOID imageAddress = ReadRemotePtr(processInfo.hProcess, dbgEvent.u.LoadDll.lpImageName);
                WCHAR * msg = ReadRemoteSZ(processInfo.hProcess, imageAddress, dbgEvent.u.LoadDll.fUnicode);

                if (NULL == msg)
                {
                    std::wcout << L"ImageName: cannot be read"<< std::endl;
                }
                else
                {
                    std::wcout << L"ImageName: " << msg << std::endl;
                    delete[] msg;
                }
            }
            break;

        case UNLOAD_DLL_DEBUG_EVENT:
            
            //typedef struct _UNLOAD_DLL_DEBUG_INFO {
            //    LPVOID lpBaseOfDll;
            //} UNLOAD_DLL_DEBUG_INFO, *LPUNLOAD_DLL_DEBUG_INFO;

            std::cout << "\tUnloadDllEvent Address=" << std::hex << dbgEvent.u.UnloadDll.lpBaseOfDll <<std::endl;
            break;

        case OUTPUT_DEBUG_STRING_EVENT:

            //typedef struct _OUTPUT_DEBUG_STRING_INFO {
            //    LPSTR lpDebugStringData;
            //    WORD fUnicode;
            //    WORD nDebugStringLength;
            //} OUTPUT_DEBUG_STRING_INFO, *LPOUTPUT_DEBUG_STRING_INFO;

            {
                std::cout << "\tOutputDebugEvent ";
                OUTPUT_DEBUG_STRING_INFO& OutputDebug = dbgEvent.u.DebugString;
                WCHAR * msg = ReadRemoteString(processInfo.hProcess, OutputDebug.lpDebugStringData, OutputDebug.nDebugStringLength, OutputDebug.fUnicode);
                if (NULL == msg)
                {
                    std::wcout << L"Cannot read OutputDebug"<< std::endl;
                }
                else
                {
                    std::wcout << L"OutputDebug:\t"<< msg << std::endl;
                    delete[] msg;
                }
                break;
            }

        case RIP_EVENT:

            //typedef struct _RIP_INFO {
            //    DWORD dwError;
            //    DWORD dwType;
            //} RIP_INFO, *LPRIP_INFO;

            std::cout << "RipEvent Error = "<< std::hex << dbgEvent.u.RipInfo.dwError
                      << ", Type = " << std::hex << dbgEvent.u.RipInfo.dwType << std::endl;
            break;
    }
    return DBG_CONTINUE ;
}

void _cdecl wmain(int argc, wchar_t* argv[])
{
    std::cout << ">> dgidebug\n" << std::endl;

    if (argc < 2)
    {
        std::cout << "-- Usage: dgidebug apptarget <apptarget aruments>\n" << std::endl;
        std::cout << "<< dgidebug\n" << std::endl;
        return;
    }

    STARTUPINFO startupInfo={0}; 
    startupInfo.cb = sizeof(startupInfo);

    std::wstring commandLine;
    for(int i=1;i<argc;i++)
    {
        commandLine.append(argv[i]);
        commandLine.append(L" ");
    }
    
    std::cout << "-- Starting " << commandLine.c_str() << "\n" << std::endl;
    BOOL res = CreateProcess(NULL, (LPWSTR)commandLine.c_str(), NULL, NULL, FALSE, DEBUG_PROCESS, NULL, NULL, &startupInfo, &processInfo);
    if (FALSE == res)
    {
        std::cout << "-- CreateProcess failed, " << GetLastError() << "\n" << std::endl;
        std::cout << "<< dgidebug\n" << std::endl;
        return;
    }
    DEBUG_EVENT debugEvent = { 0 } ;

    DWORD endDisposition = DBG_CONTINUE;
    for(;endDisposition != 0;)
    {
        if (!WaitForDebugEvent(&debugEvent, INFINITE))
        {
            std::cout << "-- WaitForDebugEvent failed, " << GetLastError() << "\n" << std::endl;
            break;
        }
        endDisposition = ProcessEvent(debugEvent);
        if (0 == endDisposition) break;
        if (!ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, endDisposition))
        {
            std::cout << "-- ContinueDebugEvent failed, " << GetLastError() << "\n" << std::endl;
            break;
        };
    }
    std::cout << "<< dgidebug\n" << std::endl;
}

