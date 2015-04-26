/*--
The MIT License (MIT)

Copyright (c) 2012-2013 De Giuli Informática Ltda. (http://www.degiuli.com.br)

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

#include "dgitracer.h"
#include <chrono>

using namespace dgi;

/*
** Ascii trace class
*/
DGITracer::DGITracer(std::string tracefile,unsigned int filelimit)
{
    OutputDebugString(">> DGITracer::ctr()\r\n");

    //check input
    if(tracefile.size()<=5)
    {
        OutputDebugString("<< DGITracer::ctr() inv arg\r\n");
        throw std::invalid_argument("Invalid trace file name");
    }

    //open file and to to the end
    m_tracefilename = tracefile;
    m_tracefile.open(m_tracefilename.c_str(), std::ios_base::app);
    m_tracefile.seekp(0, std::ios_base::end);        //go to the end of the file
    m_filelimit = filelimit;
    m_checklimit = 0;

    //start queue
    m_first = new struct Msg(new std::string());
    m_last = m_divider = m_first;

    //start thread
    m_thread = std::thread([&]()
    {
        OutputDebugString(">> DGITracer::ConsumeThread()\r\n");

        THREADNAME_INFO info;
	    info.dwType = 0x1000;
	    info.szName = "DGITracerConsumerThread";
	    info.dwThreadID = -1;   //caller thread
	    info.dwFlags = 0;

	    __try
	    {
		    RaiseException(MS_VC_EXCEPTION,0,sizeof(info)/sizeof(ULONG_PTR),(ULONG_PTR*)&info);
	    }
	    __except (EXCEPTION_CONTINUE_EXECUTION)
	    {
	    }

        OutputDebugString("-- DGITracer::ConsumeThread() name set\r\n");

        //run until tracer class is about to be unloaded
        OutputDebugString("-- DGITracer::ConsumeThread() before consume queue\r\n");
        ConsumeQueue();
        OutputDebugString("<< DGITracer::ConsumeThread() after consume queue\r\n");
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    OutputDebugString("<< DGITracer::ctr()\r\n");
}

//temporary: the current builder does not support copy constructor nor move constructor
/*
DGITracer::DGITracer(const DGITracer &tracer)
{
    m_tracefile = tracer.m_tracefile;
    m_filelimit = tracer.m_filelimit;
    m_tracefilename = tracer.m_tracefilename;
    //syncher_ = tracer.syncher_;
    m_checklimit = 0;
}

DGITracer& DGITracer::operator=(const DGITracer& tracer)
{
    m_tracefile = tracer.m_tracefile;
    m_filelimit = tracer.m_filelimit;
    m_tracefilename = tracer.m_tracefilename;
    //syncher_ = tracer.syncher_;
    m_checklimit = 0;
    return *this;
}

DGITracer::DGITracer(const DGITracer&& tracer)
{
    m_tracefile = std::move(tracer.m_tracefile);
    m_tracefilename = std::more(tracer.m_tracefilename);
    //syncher_ = std::move(tracer.syncher_);
    m_filelimit = tracer.m_filelimit;
    m_checklimit = 0;
}

DGITracer& DGITracer::operator=(const DGITracer&& tracer)
{
    std::move(m_tracefile,tracer.m_tracefile);
    std::move(m_tracefilename,tracer.m_tracefilename);
    //std::move(syncher_,tracer.syncher_);
    m_filelimit = tracer.m_filelimit;
    m_checklimit = 0;
    return *this;
}
*/

DGITracer::~DGITracer()
{
    OutputDebugString(">> DGITracer::dtr()\r\n");

    //stop thread
    if (m_threadrun.load(std::memory_order_acquire))
    {
        OutputDebugString("-- DGITracer::dtr() stopping thread\r\n");
        m_threadrun.store(false, std::memory_order_release);
        OutputDebugString("-- DGITracer::dtr() waiting thread finish\r\n");
        m_thread.join();
    }

    //cleanup list
    OutputDebugString("-- DGITracer::dtr() cleaunp queue\r\n");
    while(m_first!=nullptr)
    {
        Msg* temp = m_first;
        m_first = temp->m_next;
        delete temp;
    }

    //close file if opened
    OutputDebugString("-- DGITracer::dtr() closing file\r\n");
    if(m_tracefile.is_open())
    {
        m_tracefile.close();
    }

    OutputDebugString("<< DGITracer::dtr()\r\n");
}
    
void DGITracer::CheckFileSize()
{
    OutputDebugString(">> DGITracer::CheckFileSize()\r\n");

    if(m_threadrun.load(std::memory_order_acquire)==false)
    {
        OutputDebugString("<< DGITracer::CheckFileSize() no thrd\r\n");
        return;
    }

    if(!m_tracefile.is_open())
    {
        OutputDebugString("<< DGITracer::CheckFileSize() not opn\r\n");
        return;
    }

    //the file size check should not be performed on every write, but only after 10 writes
    if(m_checklimit<10)
    {
        m_checklimit++;
        OutputDebugString("<< DGITracer::CheckFileSize() no chk\r\n");
        return;
    }
    
    //reset check time
    m_checklimit = 0;

    //force write by flushing
    m_tracefile.flush();

    //go to the end of the file and get the position
    m_tracefile.seekp(0, std::ios_base::end);
    auto pos = m_tracefile.tellp();

    if(pos>m_filelimit && m_filelimit>0)
    {
        std::string newfilename = m_tracefilename;
        newfilename += "_bkp";

        //close current file, rename it to the backup and open a new again
        m_tracefile.close();
        rename(m_tracefilename.c_str(),newfilename.c_str());
        m_tracefile.open(m_tracefilename.c_str(), std::ios_base::out | std::ios_base::app);
    }

    OutputDebugString("<< DGITracer::CheckFileSize()\r\n");
}

void DGITracer::SendTrace(unsigned int id,const char *format,...)
{
    OutputDebugString(">> DGITracer::SendTrace()\r\n");

    if(m_threadrun.load()==false)
    {
        OutputDebugString("<< DGITracer::SendTrace() no thrd\r\n");
        return;
    }

    char buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    //format trace line
    char msgfmt[KBYTES*2] = {0};
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    _snprintf(msgfmt,sizeof(msgfmt)-1,"%04d.%02d.%02d|%02d:%02d:%02d.%03d|T|PID%05i|TID%05u|T|%05u|%s\r\n",
        sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,
        sysTime.wMilliseconds,_getpid(),GetCurrentThreadId(),id,buffer);

    //send trace line to the queue
    std::string msg(msgfmt);
    AddToQueue(msg);

    OutputDebugString("<< DGITracer::SendTrace()\r\n");
}
    
void DGITracer::SendInformation(unsigned int id,const char *format,...)
{
    OutputDebugString(">> DGITracer::SendInformation()\r\n");

    if(m_threadrun.load()==false)
    {
        OutputDebugString("<< DGITracer::SendInformation() no thrd\r\n");
        return;
    }

    char buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    //format trace line
    char msgfmt[KBYTES*2] = {0};
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    _snprintf(msgfmt,sizeof(msgfmt)-1,"%04d.%02d.%02d|%02d:%02d:%02d.%03d|I|PID%05i|TID%05u|T|%05u|%s\r\n",
        sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,
        sysTime.wMilliseconds,_getpid(),GetCurrentThreadId(),id,buffer);

    //send trace line to the queue
    std::string msg(msgfmt);
    AddToQueue(msg);

    OutputDebugString("<< DGITracer::SendInformation()\r\n");
}
    
void DGITracer::SendWarning(unsigned int id,const char *format,...)
{
    OutputDebugString(">> DGITracer::SendWarning()\r\n");

    if(m_threadrun.load()==false)
    {
        OutputDebugString("<< DGITracer::SendWarning() no thrd\r\n");
        return;
    }

    char buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    //format trace line
    char msgfmt[KBYTES*2] = {0};
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    _snprintf(msgfmt,sizeof(msgfmt)-1,"%04d.%02d.%02d|%02d:%02d:%02d.%03d|W|PID%05i|TID%05u|T|%05u|%s\r\n",
        sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,
        sysTime.wMilliseconds,_getpid(),GetCurrentThreadId(),id,buffer);

    //send trace line to the queue
    std::string msg(msgfmt);
    AddToQueue(msg);

    OutputDebugString("<< DGITracer::SendWarning()\r\n");
}
    
void DGITracer::SendError(unsigned int id,const char *format,...)
{
    OutputDebugString(">> DGITracer::SendError()\r\n");

    if(m_threadrun.load()==false)
    {
        OutputDebugString("<< DGITracer::SendError() no thrd\r\n");
        return;
    }

    char buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    //format trace line
    char msgfmt[KBYTES*2] = {0};
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    _snprintf(msgfmt,sizeof(msgfmt)-1,"%04d.%02d.%02d|%02d:%02d:%02d.%03d|E|PID%05i|TID%05u|T|%05u|%s\r\n",
        sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,
        sysTime.wMilliseconds,_getpid(),GetCurrentThreadId(),id,buffer);

    //send trace line to the queue
    std::string msg(msgfmt);
    AddToQueue(msg);

    OutputDebugString("<< DGITracer::SendError()\r\n");
}
    
void DGITracer::SendFatal(unsigned int id,const char *format,...)
{
    OutputDebugString(">> DGITracer::SendFatal()\r\n");

    if(m_threadrun.load()==false)
    {
        OutputDebugString("<< DGITracer::SendFatal() no thrd\r\n");
        return;
    }

    char buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    //format trace line
    char msgfmt[KBYTES*2] = {0};
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    _snprintf(msgfmt,sizeof(msgfmt)-1,"%04d.%02d.%02d|%02d:%02d:%02d.%03d|F|PID%05i|TID%05u|T|%05u|%s\r\n",
        sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,
        sysTime.wMilliseconds,_getpid(),GetCurrentThreadId(),id,buffer);

    //send trace line to the queue
    std::string msg(msgfmt);
    AddToQueue(msg);

    OutputDebugString("<< DGITracer::SendFatal()\r\n");
}

void DGITracer::ConsumeQueue()
{
    OutputDebugString(">> DGITracer::ConsumeQueue()\r\n");

    m_threadrun.store(true);
    while(m_threadrun.load(std::memory_order_acquire))
    {
        //wait some time for next read
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        //consumes queue
        if (m_divider.load(std::memory_order_acquire) != m_last.load(std::memory_order_acquire))
        {
            OutputDebugString("-- DGITracer::ConsumeQueue() consuming queue\r\n");

            //get message
            std::string* msg = m_divider.load(std::memory_order_acquire)->m_next->m_msg;

            //write on file
            if(m_tracefile.is_open())
            {
                m_tracefile << *msg;
            }
            else
            {
                OutputDebugString(msg->c_str());
                m_tracefile.open(m_tracefilename.c_str(), std::ios_base::app);
                m_tracefile.seekp(0, std::ios_base::end);        //go to the end of the file
            }

            //indicates it was processed
            m_divider.store(m_divider.load(std::memory_order_acquire)->m_next, std::memory_order_release);

            //check file size
            CheckFileSize();
        }
    }

    OutputDebugString("<< DGITracer::ConsumeQueue()\r\n");
}

void DGITracer::AddToQueue(const std::string& msg)
{
    OutputDebugString(">> DGITracer::AddToQueue()\r\n");

    if (m_threadrun.load(std::memory_order_acquire) == false)
    {
        OutputDebugString("<< DGITracer::AddToQueue() no thrd\r\n");
        return;
    }

    //acquire exclusive acccess
    DGILockGuard locker(m_spinlock);

    m_last.load(std::memory_order_acquire)->m_next = new struct Msg(new std::string(msg));
    m_last.store(m_last.load(std::memory_order_acquire)->m_next, std::memory_order_release);

    //cleanup processed messages
    while (m_first != m_divider.load(std::memory_order_acquire))
    {
        Msg* temp = m_first;
        m_first = temp->m_next;
        delete temp;
    }
    OutputDebugString("<< DGITracer::AddToQueue()\r\n");
}
