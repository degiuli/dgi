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

#ifndef _DGI_QUEUE_
#define _DGI_QUEUE_

#include "dgidefines.h"
#include "dgiatomic.h"

namespace dgi
{
    template<class T>
    class DGIQueue
    {
    private:
        struct Node
        {
            Node(T* val) : m_value(val), m_next(NULL) {}
            T* m_value;
            dgi::atomic<Node*> m_next;

            //to ensure that two Node will not occupy the same cache line
            char pad[CACHE_LINE_SIZE - sizeof(T*) - sizeof(dgi::atomic<struct Node*>)];
        };

        //queue controlled member
        char pad0[CACHE_LINE_SIZE];

        struct Node* m_first;

        char pad1[CACHE_LINE_SIZE - sizeof(struct Node*)];

        dgi::atomic<bool> m_consumerLock;

        char pad2[CACHE_LINE_SIZE - sizeof(bool)];

        struct Node* m_last;

        char pad3[CACHE_LINE_SIZE - sizeof(struct Node*)];

        dgi::atomic<bool> m_producerLock;

        char pad4[CACHE_LINE_SIZE - sizeof(bool)];

    public:
        //default constructor
        DGIQueue()
        {
            m_first = m_last = new struct Node(NULL);
            m_consumerLock = m_producerLock = false;
        }

        //destructor
        ~DGIQueue()
        {
            while(m_first!=NULL)
            {
                struct Node* temp = m_first;
                m_first = temp->m_next;
                if(temp->m_value)
                    delete temp->m_value;
                delete temp;
            }
        }

        //method responsible to add into the queue
        void Producer(const T& val)
        {
            struct Node* temp = new struct Node(new T(val));

            //acquire exclusive acccess
            while(m_producerLock.exchange(true))
            {}

            //add new into queue
            m_last->m_next = temp;
            m_last = temp;

            //release
            m_producerLock = false;
        }

        //method responsible to consume the queue
        bool Consumer(T& result)
        {
            //acquire exclusive acccess
            while(m_consumerLock.exchange(true))
            {}

            //consume the queue if there is anything to be consumed
            struct Node* first = m_first;
            struct Node* next = m_first->m_next;
            if(next!=NULL)
            {
                //take out of the queue
                T* val = next->m_value;
                next->m_value = NULL;

                //move forward in the queue
                m_first = next;

                //release
                m_consumerLock = false;

                //copy into result
                result = *val;
                delete val;
                delete first;

                //indicate the value was recovered from queue
                return true;
            }
            //release
            m_consumerLock = false;

            //indicate the list is empty
            return false;
        }
    };
}

#endif //_DGI_QUEUE_
