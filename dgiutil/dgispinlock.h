/*--
The MIT License (MIT)

Copyright (c) 2012-2024 Fabio Lourencao De Giuli (http://degiuli.github.io)
Copyright (c) 2012-2024 De Giuli Informatica Ltda. (http://www.degiuli.com.br)

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


#ifdef _HAS_CPP0X
#include <thread>
#include <atomic>
#else
#include <windows.h>
#endif

namespace dgi
{
    class DGISpinLock
    {
    public:
        DGISpinLock() : m_state(FALSE) {}
        ~DGISpinLock() {}

        //locker method
        void lock()
        {
#ifndef _HAS_CPP0X
            do
            {
                while (m_state != 0)
                {
                    Sleep(1);
                }

                long prev_state = InterlockedExchange(&m_state, 1);
                if (prev_state == 0)
                {
                    break;
                }

            } while (true);
#else
            while(m_state.test_and_set(std::memory_order_acquire))
            {
                std::this_thread::yield();
            }
#endif
        }

        //unlocker method
        void unlock()
        {
#ifndef _HAS_CPP0X
            InterlockedExchange(&m_state,FALSE);
#else
            m_state.clear(std::memory_order_release);
#endif
        }

    private:
#ifndef _HAS_CPP0X
        volatile long m_state;
#else
        std::atomic_flag m_state = ATOMIC_FLAG_INIT;
#endif
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
    };
}

#endif  //_DGI_SPINLOCK_
