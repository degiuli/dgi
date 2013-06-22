/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef _DGI_ATOMIC_
#define _DGI_ATOMIC_

#include "dgispinlock.h"

//this atomic class has been developed to be used when C++/11 is no available
//and, as a consequence, the std::mutex and str::atomic is not available either

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
        inline T get()
        {
            m_syncher.lock();

            T value = m_value;

            m_syncher.unlock();

            return value;
        }

        //set method
        //sets the new value
        inline void set(T value)
        {
            m_syncher.lock();

            m_value = value;

            m_syncher.unlock();
        }

        //exchange method
        //this method replaces the m_value with argument and
        //returns the old value of the atomic 
        inline bool exchange(T newValue)
        {
            bool ret;

            m_syncher.lock();

            ret = m_value;
            m_value = newValue;

            m_syncher.unlock();

            return ret;
        }

        //compare_exchange method
        //this method compares whether the m_value is the expected value
        //if so, new value is set and then method returns true
        //otherwise, false is returned
        inline bool compare_exchange(T expectedValue, T newValue)
        {
            bool ret;

            m_syncher.lock();

            if(m_value==expectedValue)
            {
                m_value = newValue;
                ret = true;
            }
            else
            {
                ret = false;
            }

            m_syncher.unlock();

            return ret;
        }

        //copy constructor
        atomic(const atomic& a)
        {
            m_value = a.get();
            m_syncher = a.m_syncher;
        }

        //assign constructor
        atomic& operator=(const atomic& a)
        {
            m_value = a.m_value;
            m_syncher = a.m_syncher;

            return *this;
        }

        //incremental operator
        inline void operator++()
        {
            m_syncher.lock();

            m_value++;

            m_syncher.unlock();
        }

        //decremental operator
        inline void operator--()
        {
            m_syncher.lock();

            m_value--;

            m_syncher.unlock();
        }

        //compare operators
        inline void operator==(T value)
        {
            m_syncher.lock();

            bool ret = (m_value==value);

            m_syncher.unlock();
            return ret;
        }
        inline void operator!=(T value)
        {
            m_syncher.lock();

            bool ret = (m_value!=value);

            m_syncher.unlock();
            return ret;
        }
        inline void operator>(T value)
        {
            m_syncher.lock();

            bool ret = (m_value>value);

            m_syncher.unlock();
            return ret;
        }
        inline void operator<(T value)
        {
            m_syncher.lock();

            bool ret = (m_value<value);

            m_syncher.unlock();
            return ret;
        }
        inline void operator>=(T value)
        {
            m_syncher.lock();

            bool ret = (m_value>=value);

            m_syncher.unlock();
            return ret;
        }
        inline void operator<=(T value)
        {
            m_syncher.lock();

            bool ret = (m_value<=value);

            m_syncher.unlock();
            return ret;
        }

        //add operator
        inline void operator+=(T value)
        {
            m_syncher.lock();

            m_value += value;

            m_syncher.unlock();
        }

        //subtract operator
        inline void operator-=(T value)
        {
            m_syncher.lock();

            m_value -= value;

            m_syncher.unlock();
        }

        //multiplier operator
        inline void operator*=(T value)
        {
            m_syncher.lock();

            m_value *= value;

            m_syncher.unlock();
        }

        //divider operator
        inline void operator/=(T value)
        {
            m_syncher.lock();

            m_value /= value;

            m_syncher.unlock();
        }

        //bitwise AND operator
        inline void operator&=(T value)
        {
            m_syncher.lock();

            m_value &= value;

            m_syncher.unlock();
        }

        //bitwise OR operator
        inline void operator|=(T value)
        {
            m_syncher.lock();

            m_value |= value;

            m_syncher.unlock();
        }

        //bitwise XOR operator
        inline void operator^=(T value)
        {
            m_syncher.lock();

            m_value ^= value;

            m_syncher.unlock();
        }
    };
}

#endif //_DGI_ATOMIC_
