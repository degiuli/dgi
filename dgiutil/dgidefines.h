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

#ifndef _DGI_DEFINES_
#define _DGI_DEFINES_

//sizes definiitions
#define KBYTES              (unsigned long)(1024UL)
#define MBYTES              (unsigned long)(KBYTES*KBYTES)
#define GBYTES              (unsigned long)(MBYTES*KBYTES)
#define SIZE_KB(size)       ((unsigned long)((size/KBYTES)+0.5))
#define SIZE_MB(size)       ((unsigned long)((size/MBYTES)+0.5))
#define SIZE_GB(size)       ((unsigned long)((size/GBYTES)+0.5))

//time definitions
#define MILLISECOND         (unsigned long long)(1L)
#define SECOND              (unsigned long long)(MILLISECOND*1000UL)
#define MINUTE              (unsigned long long)(SECOND*60UL)
#define HOUR                (unsigned long long)(MINUTE*60UL)
#define DAY                 (unsigned long long)(HOUR*24UL)
#define YEAR                (unsigned long long)(DAY*(365.25))

//frequency definitions
#define Hz                  (1UL)
#define KHz                 (Hz*1000UL)
#define MHz                 (KHz*1000UL)
#define GHz                 (MHz*1000UL)

//exception info for windows thread naming
#define MS_VC_EXCEPTION 0x406D1388

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	unsigned long dwType;       //must be 0x1000
	const char* szName;         //pointer to name (in user addr space)
	unsigned long dwThreadID;   //thread ID (-1=caller thread)
	unsigned long dwFlags;      //reserved for future use, must be zero
} THREADNAME_INFO;
#pragma pack(pop)

#endif //_DGI_DEFINES_
