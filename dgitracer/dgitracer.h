/*--
The MIT License (MIT)

Copyright (c) 2012-2019 Fabio Lourencao De Giuli (http://degiuli.github.io)
Copyright (c) 2012-2019 De Giuli Informatica Ltda. (http://www.degiuli.com.br)

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

#ifndef _DGI_TRACER_
#define _DGI_TRACER_

#include <string>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <stdexcept>

#include <process.h>
#include <windows.h>
#include <thread>
#include <atomic>

#include "dgidefines.h"
#include "dgiatomic.h"

namespace dgi
{
    class DGITracer
    {
    private:
        std::ofstream m_tracefile;
        std::string m_tracefilename;
        size_t m_filelimit;
        std::atomic<bool> m_threadrun;
        std::thread m_thread;

        //check whether the file reaches the limit and backup it
        void CheckFileSize();
        short m_checklimit;

        //trace queue
        void ConsumeQueue();
        void AddToQueue(const std::string& msg);
        struct Msg
        {
            Msg(std::string* msg) : m_msg(msg), m_next(NULL) {}
            std::string* m_msg;
            Msg* m_next;
        };
        struct Msg* m_first;
        std::atomic<struct Msg*> m_divider;
        std::atomic<struct Msg*> m_last;
        DGISpinLock m_spinlock;

    public:
        //default constructor
        DGITracer(std::string tracefile, size_t filelimit=0);

    public:
        //destructor
        ~DGITracer();
    
        void SendTrace(const uint32_t id,char const* format,...);
        void SendInformation(const uint32_t id,char const* format,...);
        void SendWarning(const uint32_t id,char const* format,...);
        void SendError(const uint32_t id,char const* format,...);
        void SendFatal(const uint32_t id,char const* format,...);
    };
}   //end namespace dgi

#endif //_DGI_TRACER_
