/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include "dgitracer.h"
//#include <chrono>

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
    //std::thread...
    unsigned threadId = 0;
    m_threadhandle = _beginthreadex(NULL,0,ConsumerThread,this,0,&threadId);
    Sleep(50);      //wait thread to be created

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
    if(m_threadrun.get())
    {
        OutputDebugString("-- DGITracer::dtr() stopping thread\r\n");
        m_threadrun.set(false);
        //std::join(std::thread);

        OutputDebugString("-- DGITracer::dtr() waiting thread finish\r\n");
        WaitForSingleObject((HANDLE)m_threadhandle,INFINITE);
        CloseHandle((HANDLE)m_threadhandle);
        m_threadhandle = 0;
    }

    //cleanup list
    OutputDebugString("-- DGITracer::dtr() cleaunp queue\r\n");
    while(m_first!=NULL)
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

    if(m_threadrun.get()==false)
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

    if(m_threadrun.get()==false)
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

    if(m_threadrun.get()==false)
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

    if(m_threadrun.get()==false)
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

    if(m_threadrun.get()==false)
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

    if(m_threadrun.get()==false)
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

unsigned __stdcall DGITracer::ConsumerThread(void*param)
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

    DGITracer *ptracer = (DGITracer*)param;

    //run until tracer class is about to be unloaded
    OutputDebugString("-- DGITracer::ConsumeThread() before consume queue\r\n");
    ptracer->ConsumeQueue();
    OutputDebugString("<< DGITracer::ConsumeThread() after consume queue\r\n");

    _endthreadex(0);
    return 0;
}

void DGITracer::ConsumeQueue()
{
    OutputDebugString(">> DGITracer::ConsumeQueue()\r\n");

    m_threadrun.set(true);
    while(m_threadrun.get())
    {
        //wait some time for next read
        //std::chrono::milliseconds dura(100);
        //std::this_thread::sleep_for(dura);
        Sleep(100);

        //consumes queue
        if(m_divider.get() != m_last.get())
        {
            OutputDebugString("-- DGITracer::ConsumeQueue() consuming queue\r\n");

            //get message
            std::string* msg = m_divider.get()->m_next->m_msg;

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
            m_divider.set(m_divider.get()->m_next);

            //check file size
            CheckFileSize();
        }
    }

    OutputDebugString("<< DGITracer::ConsumeQueue()\r\n");
}

void DGITracer::AddToQueue(const std::string& msg)
{
    OutputDebugString(">> DGITracer::AddToQueue()\r\n");

    if(m_threadrun.get()==false)
    {
        OutputDebugString("<< DGITracer::AddToQueue() no thrd\r\n");
        return;
    }

    //acquire exclusive acccess
    m_spinlock.lock();

    m_last.get()->m_next = new struct Msg(new std::string(msg));
    m_last.set(m_last.get()->m_next);

    //cleanup processed messages
    while(m_first!=m_divider.get())
    {
        Msg* temp = m_first;
        m_first = temp->m_next;
        delete temp;
    }

    //release
    m_spinlock.unlock();

    OutputDebugString("<< DGITracer::AddToQueue()\r\n");
}
