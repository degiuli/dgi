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

#ifndef _DGI_SPINLOCK_
#define _DGI_SPINLOCK_

#include <windows.h>

#ifdef _HAS_CPP0X
#include <thread>
#endif

namespace dgi
{
    class DGISpinLock
    {
    public:
        DGISpinLock() : m_state(FALSE) {}
        ~DGISpinLock() {}

        //copy constructor
        DGISpinLock(const DGISpinLock& sl)
        {
            m_state = sl.m_state;
        }
    
        //assign contructor
        DGISpinLock& DGISpinLock::operator=(const DGISpinLock& sl)
        {
            m_state = sl.m_state;
            return *this;
        }
    
        //locker method
        void lock()
        {
            do
            {
                while (m_state==TRUE)
                {
#ifdef _HAS_CPP0X
                    std::this_thread::yield();
#else
                    Sleep(1);
#endif
                }

                long prev_state = InterlockedExchange(&m_state,TRUE);
                if(prev_state==FALSE)
                    break;

            } while (true);
        }

        //unlocker method
        void unlock()
        {
            InterlockedExchange(&m_state,FALSE);
        }

    private:
        volatile long m_state;
    };

    //lock guard class
    class DGILockGuard
    {
    public:
        DGILockGuard(const DGISpinLock& locker) : m_locker(locker)
        {
            m_locker.lock();
        }
        ~DGILockGuard()
        {
            m_locker.unlock();
        }

    private:
        DGISpinLock m_locker;
        DGILockGuard(const DGILockGuard& locker);
        DGILockGuard(const DGILockGuard&& locker);
        DGILockGuard& DGILockGuard::operator=(const DGILockGuard& locker);
        DGILockGuard&& DGILockGuard::operator=(const DGILockGuard&& locker);
    };
}

#endif  //_DGI_SPINLOCK_
