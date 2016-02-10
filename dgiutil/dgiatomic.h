/*--
The MIT License (MIT)

Copyright (c) 2012-2015 Fabio Lourencao De Giuli (http://degiuli.github.io)
Copyright (c) 2012-2015 De Giuli Informatica Ltda. (http://www.degiuli.com.br)

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

#ifndef _DGI_ATOMIC_
#define _DGI_ATOMIC_

#include "dgispinlock.h"

//this atomic class has been developed to be used when C++/11 is no available
//or it is not fully implemented missing the std::atomic.

namespace dgi
{
    template<class T>
    class atomic
    {
    private:
        DGISpinLock m_syncher;
        volatile T m_value;
    public:
        //defaul contructors
        atomic()
        {
            m_value = T();
        }
        atomic(T value)
        {
            m_value = value;
        }
        //destructor
        ~atomic()
        {
        }

        //get method
        //gets the correct value
        T load()
        {
            DGILockGuard locker(m_syncher);
            T value = m_value;
            return value;
        }

        //set method
        //sets the new value
        void store(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value = value;
        }

        //fetch_add
        //add a T value
        T fetch_add(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value += value;
            return m_value;
        }

        //fetch_sub
        //subtract a T value
        T fetch_sub(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value -= value;
            return m_value;
        }

        //incremental operator
        T operator++()
        {
            return fetch_add(1);
        }
        T operator++(int)
        {
            return fetch_add(1);
        }

        //decremental operator
        T operator--()
        {
            return fetch_sub(1);
        }
        T operator--(int)
        {
            return fetch_sub(1);
        }

        //assign operator
        T operator=(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value = value;
            return m_value;
        }

        //compare operators
        bool operator==(T value)
        {
            DGILockGuard locker(m_syncher);
            return (m_value==value);
        }
        bool operator!=(T value)
        {
            DGILockGuard locker(m_syncher);
            return (m_value!=value);
        }
        bool operator>(T value)
        {
            DGILockGuard locker(m_syncher);
            return (m_value>value);
        }
        bool operator<(T value)
        {
            DGILockGuard locker(m_syncher);
            return (m_value<value);
        }
        bool operator>=(T value)
        {
            DGILockGuard locker(m_syncher);
            return (m_value>=value);
        }
        bool operator<=(T value)
        {
            DGILockGuard locker(m_syncher);
            return (m_value<=value);
        }

        //add operator
        T operator+=(T value)
        {
            return fetch_add(value);
        }

        //subtract operator
        T operator-=(T value)
        {
            return fetch_sub(value);
        }

        //multiplier operator
        T operator*=(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value *= value;
            return m_value;
        }

        //divider operator
        T operator/=(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value /= value;
            return m_value;
        }

        //bitwise AND operator
        T operator&=(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value &= value;
            return m_value;
        }

        //bitwise OR operator
        T operator|=(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value |= value;
            return m_value;
        }

        //bitwise XOR operator
        T operator^=(T value)
        {
            DGILockGuard locker(m_syncher);
            m_value ^= value;
            return m_value;
        }
    };
}

#endif //_DGI_ATOMIC_
