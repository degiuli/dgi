/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <iostream>
#include "dgiminidump.h"
#include "dgitracer.h"

void StackOverrun(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: StackOverrun\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: StackOverrun");

    std::string str("123456789-123456789-");
    char data[10] = {0};            //stack memory
    strcpy(data,str.c_str());       //copy 20 bytes to a 10-byte area

    std::cout << data << "\n" << std::endl;

    tracer.SendTrace(__LINE__,"<< DGICrash: StackOverrun");

    OutputDebugString("<< DGICrash: StackOverrun\r\n");
}

void CallingConventionsMismatch(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: CallingConventionsMismatch\r\n");

    tracer.SendWarning(__LINE__,"-- Test has not yet been implemented");
    std::cout << "Test has not yet been implemented\n" << std::endl;

    OutputDebugString("<< DGICrash: CallingConventionsMismatch\r\n");
}

int StackOverflow_Aux(int x)
{
    OutputDebugString("-- DGICrash: StackOverflow_Aux\r\n");
    
    std::cout << "-- DGICrash: StackOverflow_Aux(" << x << ") -> " << (sizeof(double)*1000000) << std::endl;

    double very_large_array[1000000];
    //do something ...

    if(x>0)
        return StackOverflow_Aux(--x);
    else
        return 0;
}

void StackOverflow(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: StackOverflow\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: StackOverrun");

    StackOverflow_Aux(5);

    tracer.SendTrace(__LINE__,"<< DGICrash: StackOverrun");

    OutputDebugString("<< DGICrash: StackOverflow\r\n");
}

void AccessingUninitializedMemory(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: AccessingUninitializedMemory\r\n");

    tracer.SendWarning(__LINE__,"Test has not yet been implemented");
    std::cout << "Test has not yet been implemented\n" << std::endl;

    OutputDebugString("<< DGICrash: AccessingUninitializedMemory\r\n");
}

void HeapOverruns(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: HeapOverruns\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: HeapOverruns");

    char *pdata = new char[100];
    if(pdata)
    {
        //overrun
        memset(pdata,0x00,110);

        delete [] pdata;
    }

    tracer.SendTrace(__LINE__,"<< DGICrash: HeapOverruns");

    OutputDebugString("<< DGICrash: HeapOverruns\r\n");
}

void HeapUnderruns(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: HeapUnderruns\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: HeapUnderruns");

    char *pdata = new char[100];
    if(pdata)
    {
        //underrun
        memset(&pdata[-10],0x00,100);

        delete [] pdata;
    }

    tracer.SendTrace(__LINE__,"<< DGICrash: HeapUnderruns");

    OutputDebugString("<< DGICrash: HeapUnderruns\r\n");
}

void AccessingFreedMemory(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: AccessingFreedMemory\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: AccessingFreedMemory");

    char *pdata = new char[10];
    if(pdata)
    {
        //do something...
        delete [] pdata;
        
        //do something more...
        std::string str("XPTO");
        strcpy(pdata,str.c_str());

        std::cout << pdata << std::endl;
    }

    tracer.SendTrace(__LINE__,"<< DGICrash: AccessingFreedMemory");

    OutputDebugString("<< DGICrash: AccessingFreedMemory\r\n");
}

void DoubleFrees(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: DoubleFrees\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: DoubleFrees");

    char *pdata = new char[10];
    if(pdata)
    {
        //do something...
        delete [] pdata;
    }
    delete [] pdata;

    tracer.SendTrace(__LINE__,"<< DGICrash: DoubleFrees");

    OutputDebugString("<< DGICrash: DoubleFrees\r\n");
}

void ErroneousFree_1(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: ErroneousFree_1\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: ErroneousFree_1");

    int * int_new = new int;
    delete [] int_new;

    tracer.SendTrace(__LINE__,"<< DGICrash: ErroneousFree_1");

    OutputDebugString("<< DGICrash: ErroneousFree_1\r\n");
}

void ErroneousFree_2(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: ErroneousFree_2\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: ErroneousFree_2");

    char * char_array = new char[10];
    delete char_array;

    tracer.SendTrace(__LINE__,"<< DGICrash: ErroneousFree_2");

    OutputDebugString("<< DGICrash: ErroneousFree_2\r\n");
}

void ErroneousFree_3(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: ErroneousFree_3\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: ErroneousFree_3");

    long * long_malloc = (long*)malloc(sizeof(long));
    delete long_malloc;

    tracer.SendTrace(__LINE__,"<< DGICrash: ErroneousFree_3");

    OutputDebugString("<< DGICrash: ErroneousFree_3\r\n");
}

void ErroneousFree_4(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: ErroneousFree_4\r\n");

    tracer.SendTrace(__LINE__,">> DGICrash: ErroneousFree_4");

    double *double_new = new double;
    free(double_new);

    tracer.SendTrace(__LINE__,"<< DGICrash: ErroneousFree_4");

    OutputDebugString("<< DGICrash: ErroneousFree_4\r\n");
}

void FreeingUnallocatedMemeory(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: FreeingUnallocatedMemeory\r\n");

    /*
    tracer.SendTrace(__LINE__,">> DGICrash: FreeingUnallocatedMemeory");

    char * mem;

    tracer.SendInformation(__LINE__,"-- DGICrash: FreeingUnallocatedMemeory, addr %p",mem);
    std::cout << "FreeingUnallocatedMemeory: " << std::hex << mem << "\n" << std::endl;

    delete mem;

    tracer.SendTrace(__LINE__,"<< DGICrash: FreeingUnallocatedMemeory");
    */
    tracer.SendWarning(__LINE__,"-- Test has not yet been implemented");
    std::cout << "Test has not yet been implemented\n" << std::endl;

    OutputDebugString("<< DGICrash: FreeingUnallocatedMemeory\r\n");
}

void HeapHandleMismatch(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: HeapHandleMismatch\r\n");

    tracer.SendWarning(__LINE__,"-- Test has not yet been implemented");
    std::cout << "Test has not yet been implemented\n" << std::endl;

    OutputDebugString("<< DGICrash: HeapHandleMismatch\r\n");
}

void Usage()
{
    OutputDebugString(">> DGICrash: Usage\r\n");

    std::cout << "Usage:" << std::endl;
    std::cout << "\tDGICrash <option>\n" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t1  - Stack Overrun" << std::endl;
    std::cout << "\t2  - Calling Conventions Mismatch" << std::endl;
    std::cout << "\t3  - Stack Overflow" << std::endl;
    std::cout << "\t4  - Accessing Uninitialized Memory" << std::endl;
    std::cout << "\t5  - Heap Overruns" << std::endl;
    std::cout << "\t6  - Heap Underruns" << std::endl;
    std::cout << "\t7  - Accessing Freed Memory" << std::endl;
    std::cout << "\t8  - Double Frees" << std::endl;
    std::cout << "\t9  - Erroneous Free (1)" << std::endl;
    std::cout << "\t10 - Erroneous Free (2)" << std::endl;
    std::cout << "\t11 - Erroneous Free (3)" << std::endl;
    std::cout << "\t12 - Erroneous Free (4)" << std::endl;
    std::cout << "\t13 - Freeing Unallocated Memeory" << std::endl;
    std::cout << "\t14 - Heap Handle Mismatch\n" << std::endl;

    OutputDebugString("<< DGICrash: Usage\r\n");
}

int _cdecl main( int argc, char* argv[] )
{
    std::cout << ">> DGICrash\n" << std::endl;

    if(IsDebuggerPresent())
    {
        DebugBreak();
    }

    OutputDebugString(">> DGICrash: main\r\n");

    try
    {
        char temp[KBYTES] = {0};
        GetCurrentDirectory(sizeof(temp)-1,temp);
        if(temp[strlen(temp)-1]!='\\' && strlen(temp)<sizeof(temp)-1)
            temp[strlen(temp)-1] = '\\';
        std::string appdir = temp;

        OutputDebugString("-- DGICrash: dir ");
        OutputDebugString(temp);
        OutputDebugString("\r\n");

        std::string file = appdir;
        file += "dgicrash.log";
        dgi::DGITracer tracer(file);

        OutputDebugString("-- DGICrash: trace file ");
        OutputDebugString(file.c_str());
        OutputDebugString("\r\n");

        file = appdir;
        file += "dgicrash";
        MiniDumpInit(tracer,file);

        OutputDebugString("-- DGICrash: minidump file ");
        OutputDebugString(file.c_str());
        OutputDebugString("\r\n");
    
        tracer.SendInformation(__LINE__,">> DGICrash");
    
        if(argc>1)
        {
            OutputDebugString("-- DGICrash: args ");
		    for(int x=1;x<argc;x++)
		    {
			    tracer.SendTrace(__LINE__,"-- Option %s",argv[x]);
                OutputDebugString(argv[x]);
                OutputDebugString(" ");
		    }
            OutputDebugString("\r\n");
		
            int option = atoi(argv[1]);
            switch(option)
            {
            case 1:
                StackOverrun(tracer);
                break;
            case 2:
                CallingConventionsMismatch(tracer);
                break;
            case 3:
                StackOverflow(tracer);
                break;
            case 4:
                AccessingUninitializedMemory(tracer);
                break;
            case 5:
                HeapOverruns(tracer);
                break;
            case 6:
                HeapUnderruns(tracer);
                break;
            case 7:
                AccessingFreedMemory(tracer);
                break;
            case 8:
                DoubleFrees(tracer);
                break;
            case 9:
                ErroneousFree_1(tracer);
                break;
            case 10:
                ErroneousFree_2(tracer);
                break;
            case 11:
                ErroneousFree_3(tracer);
                break;
            case 12:
                ErroneousFree_4(tracer);
                break;
            case 13:
                FreeingUnallocatedMemeory(tracer);
                break;
            case 14:
                HeapHandleMismatch(tracer);
                break;
            default:
                tracer.SendError(__LINE__,"-- Invalid Option: %i",option);
                std::cout << "Invalid Option: " << option << std::endl;
                Usage();
                break;
            }
        }
        else
        {
            Usage();
        }
    
        MiniDumpEnd();
        tracer.SendInformation(__LINE__,"<< DGICrash");
        std::cout << "<< DGICrash\n" << std::endl;
        OutputDebugString("<< DGICrash: main\r\n");
        return 0;
    }
    catch(std::invalid_argument& ia)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << ia.what() << std::endl;
        std::cout << "<< DGICrash\n" << std::endl;
        OutputDebugString("<< DGICrash: inv arg exception\r\n");
        return -1;
    }
    catch(std::logic_error& le)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << le.what() << std::endl;
        std::cout << "<< DGICrash\n" << std::endl;
        OutputDebugString("<< DGICrash: logic error exception\r\n");
        return -2;
    }
    catch(std::exception& e)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << e.what() << std::endl;
        std::cout << "<< DGICrash\n" << std::endl;
        OutputDebugString("<< DGICrash: generic exception\r\n");
        return -3;
    }
    catch(...)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << "Unhandled exception" << std::endl;
        std::cout << "<< DGICrash\n" << std::endl;
        OutputDebugString("<< DGICrash: unhandled exception\r\n");
        abort();
    }
    std::cout << "<< DGICrash\n" << std::endl;
    OutputDebugString("<< DGICrash: shouldn't be here\r\n");
    abort();
}
