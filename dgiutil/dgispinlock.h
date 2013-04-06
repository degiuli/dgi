/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef _DGI_SPINLOCK_
#define _DGI_SPINLOCK_

#include <windows.h>

namespace dgi
{
    class DGISpinLock
    {
    public:
        DGISpinLock() : m_state(1) {}
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
    
        void lock()
        {
            do
            {
                while (m_state==0) {}

                long prev_state = InterlockedExchange(&m_state,0);
                if(prev_state==1)
                    break;

            } while (true);
        }

        void unlock()
        {
            InterlockedExchange(&m_state,1);
        }

    private:
        volatile long m_state;
    };
}

#endif  //_DGI_SPINLOCK_