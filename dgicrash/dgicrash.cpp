/*--
The MIT License (MIT)

Copyright (c) 2012-2024 De Giuli Informática Ltda. (http://www.degiuli.com.br)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

    std::cout << data << "\n\n";

    tracer.SendTrace(__LINE__,"<< DGICrash: StackOverrun");

    OutputDebugString("<< DGICrash: StackOverrun\r\n");
}

void CallingConventionsMismatch(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: CallingConventionsMismatch\r\n");

    tracer.SendWarning(__LINE__,"-- Test has not yet been implemented");
    std::cout << "Test has not yet been implemented\n\n";

    OutputDebugString("<< DGICrash: CallingConventionsMismatch\r\n");
}

int StackOverflow_Aux(int x)
{
    OutputDebugString("-- DGICrash: StackOverflow_Aux\r\n");
    
    std::cout << "-- DGICrash: StackOverflow_Aux(" << x << ") -> " << (sizeof(double)*1000000) << '\n';

    double very_large_array[1000000];
    memset(very_large_array,0x00,sizeof(very_large_array));

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
    std::cout << "Test has not yet been implemented\n\n";

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

        std::cout << pdata << '\n';
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

    tracer.SendTrace(__LINE__,">> DGICrash: FreeingUnallocatedMemeory");

    //char* mem;

    //tracer.SendInformation(__LINE__,"-- DGICrash: FreeingUnallocatedMemeory, addr %p",mem);
    //std::cout << "FreeingUnallocatedMemeory: " << std::hex << mem << "\n\n";

    //delete mem;

    tracer.SendWarning(__LINE__, "Test has not been implemented because newer VS reporst \"error C4700: uninitialized local variable '<var>' used\"");
    std::cout << "Test has not been implemented because newer VS reporst \"error C4700: uninitialized local variable '<var>' used\"\n\n";

    tracer.SendTrace(__LINE__, "<< DGICrash: FreeingUnallocatedMemeory");

    OutputDebugString("<< DGICrash: FreeingUnallocatedMemeory\r\n");
}

void HeapHandleMismatch(dgi::DGITracer& tracer)
{
    OutputDebugString(">> DGICrash: HeapHandleMismatch\r\n");

    tracer.SendWarning(__LINE__,"-- Test has not yet been implemented");
    std::cout << "Test has not yet been implemented\n\n";

    OutputDebugString("<< DGICrash: HeapHandleMismatch\r\n");
}

void Usage()
{
    OutputDebugString(">> DGICrash: Usage\r\n");

    std::cout << "Usage:\n";
    std::cout << "\tDGICrash <option>\n\n";
    std::cout << "Options:\n";
    std::cout << "\t1  - Stack Overrun\n";
    std::cout << "\t2  - Calling Conventions Mismatch\n";
    std::cout << "\t3  - Stack Overflow\n" << '\n';
    std::cout << "\t4  - Accessing Uninitialized Memory\n";
    std::cout << "\t5  - Heap Overruns\n";
    std::cout << "\t6  - Heap Underruns\n";
    std::cout << "\t7  - Accessing Freed Memory\n";
    std::cout << "\t8  - Double Frees\n";
    std::cout << "\t9  - Erroneous Free (1)\n";
    std::cout << "\t10 - Erroneous Free (2)\n";
    std::cout << "\t11 - Erroneous Free (3)\n";
    std::cout << "\t12 - Erroneous Free (4)\n";
    std::cout << "\t13 - Freeing Unallocated Memeory\n";
    std::cout << "\t14 - Heap Handle Mismatch\n\n";

    OutputDebugString("<< DGICrash: Usage\r\n");
}

int _cdecl main( int argc, char* argv[] )
{
    std::cout << ">> DGICrash\n\n";

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
                std::cout << "Invalid Option: " << option << '\n';
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
        std::cout << "<< DGICrash\n\n";
        OutputDebugString("<< DGICrash: main\r\n");
        return 0;
    }
    catch(std::invalid_argument& ia)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << ia.what() << '\n';
        std::cout << "<< DGICrash\n\n";
        OutputDebugString("<< DGICrash: inv arg exception\r\n");
        return -1;
    }
    catch(std::logic_error& le)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << le.what() << '\n';
        std::cout << "<< DGICrash\n\n";
        OutputDebugString("<< DGICrash: logic error exception\r\n");
        return -2;
    }
    catch(std::exception& e)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << e.what() << '\n';
        std::cout << "<< DGICrash\n\n";
        OutputDebugString("<< DGICrash: generic exception\r\n");
        return -3;
    }
    catch(...)
    {
        if(IsDebuggerPresent())
        {
            DebugBreak();
        }

        std::cout << "Unhandled exception\n";
        std::cout << "<< DGICrash\n\n";
        OutputDebugString("<< DGICrash: unhandled exception\r\n");
        abort();
    }
    std::cout << "<< DGICrash\n\n";
    OutputDebugString("<< DGICrash: shouldn't be here\r\n");
    abort();
}
