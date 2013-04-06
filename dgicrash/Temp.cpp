/*
 Stack Overrun
*/

#include <cstdio>
#include <iostream>

void foo(char *pdata)
{
    char data[10] = {0};    //stack memory
    strcpy(data,pdata);
    std::cout << data << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc>1)
    {
        //pass first argument to function
        foo(argv[1]);
    }
    else
    {
        foo("None");
    }
    return 0;
}


/*
 Calling Conventions Mismatch
*/
void __stdcall DllProc(int);

typedef int (__cdecl *DLLPROC)(int);

void CallProc(DLLPROC pProc,int arg)
{
    pProc(arg);
}

int __cdecl main(int argc,char *argv[])
{
    HMODULE hMod = LoadLibrary("DLL.dll");
    if(mod)
    {
        DLLPROC pProc = (DLLPROC)GetProcAddress(hMod,"DllProc");
        if(pProc)
        {
            CallProc(pProc,argc);
        }
        FreeLibrary(hMod);
    }
    return 0;
}

/*
  Stack Overflow
*/
int foo();
{
    double very_large_array[1000000];
    //do something ...
    return foo();
}
int main()
{
    return foo();
}

/*
 Accessing Uninitialized Memory
*/
int main(int argc,char argv[])
{
    //allocate stack variable
    size_t size;
        
    //dynamically allocate a memory
    //using the uninitialized size
    char *pdata = new char[size];
    if(pdata)
    {
        std::cout << "Data: :"<< pdata << ", Size: " << size << std::endl;
        //do something...
        delete [] pdata;
    }
}

/*
 Heap Overruns and Underruns
*/
int main(int argc,char argv[])
{
    char *pdata = new char[100];
    if(pdata)
    {
        //overrun
        memset(pdata,0x00,110);
        //underrun
        memset(&pdata[-10],0x00,100);
    }
}
int main(int argc,char argv[])
{
    char *pdata = new char[10];
    if(pdata && argc>1)
    {
        strcpy(pdata,argv[1]);
        //do something...
    }
    if(pdata)
    {
        delete [] pdata;
    }
    return 0;
}

/*
 Accessing Freed Memory
*/
int main(int argc,char argv[])
{
    char *pdata = new char[10];
    if(pdata)
    {
        //do something...
        delete [] pdata;
        
        //do something more...
        if(argc>1)
        {
            strcpy(pdata,argv[1]);
        }
        std::cout << pdata << std::endl;
    }
    return 0;
}

/*
 Double Frees
*/
int main(int argc, char argv[])
{
    char *pdata = new char[10];
    if(pdata)
    {
        //do something...
        delete [] pdata;
    }
    delete [] pdata;
    return 0;
}

/*
 Erroneous Free
*/
int main(int argc,char argv[])
{
    int * int_new = new int;
    delete [] int_new;
    
    char * char_array = new char[10];
    delete char_array;
    
    long * long_malloc = (long*)malloc(sizeof(long));
    delete long_malloc;
    
    double *double_new = new double;
    free(double_new);
}

/*
 Heap Handle Mismatch
*/

