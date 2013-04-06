/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef _DGI_TRACER_
#define _DGI_TRACER_

#include <string>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <stdexcept>

#include <process.h>
#include <windows.h>
//#include <thread>
//#include <atomic>

#include "dgidefines.h"
#include "dgiatomic.h"

namespace dgi
{
    class DGITracer
    {
    private:
        std::basic_ofstream<char> m_tracefile;
        std::string m_tracefilename;
        unsigned int m_filelimit;
        unsigned int m_threadhandle;
        /*std*/dgi::atomic<bool> m_threadrun;

        //check whether the file reaches the limit and backup it
        void CheckFileSize();
        short m_checklimit;

        //trace queue
        static unsigned __stdcall ConsumerThread(void*param);
        void ConsumeQueue();
        void AddToQueue(const std::string& msg);
        struct Msg
        {
            Msg(std::string* msg) : m_msg(msg), m_next(NULL) {}
            std::string* m_msg;
            Msg* m_next;
        };
        struct Msg* m_first;
        /*std*/dgi::atomic<struct Msg*> m_divider;
        /*std*/dgi::atomic<struct Msg*> m_last;
        DGISpinLock m_spinlock;

    public:
        //default constructor
        DGITracer(std::string tracefile,unsigned int filelimit=0);

//temporary: the current builder does not support copy constructor nor move constructor
/*
        //copy constructor
        DGITracer(const DGITracer& tracer);
    
        //assign contructor
        DGITracer& DGITracer::operator=(const DGITracer& tracer);
    
        //move constructor
        DGITracer(const DGITracer&& tracer);
        DGITracer& operator=(const DGITracer&& tracer);
*/

    public:
        //destructor
        ~DGITracer();
    
        void SendTrace(unsigned int id,const char *format,...);
        void SendInformation(unsigned int id,const char *format,...);
        void SendWarning(unsigned int id,const char *format,...);
        void SendError(unsigned int id,const char *format,...);
        void SendFatal(unsigned int id,const char *format,...);
    };
}   //end namespace dgi

#endif //_DGI_TRACER_
