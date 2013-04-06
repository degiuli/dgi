/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef _DGI_DEFINES_
#define _DGI_DEFINES_

//sizes definiitions
#define KBYTES              (unsigned long)(1024)
#define MBYTES              (unsigned long)(KBYTES*KBYTES)
#define GBYTES              (unsigned long)(MBYTES*KBYTES)
#define SIZE_KB(size)       ((unsigned long)((size/KBYTES)+0.5))
#define SIZE_MB(size)       ((unsigned long)((size/MBYTES)+0.5))
#define SIZE_GB(size)       ((unsigned long)((size/GBYTES)+0.5))

//time definitions
#define MILLISECOND         (unsigned long long)(1L)
#define SECOND              (unsigned long long)(MILLISECOND*1000L)
#define MINUTE              (unsigned long long)(SECOND*60L)
#define HOUR                (unsigned long long)(MINUTE*60L)
#define DAY                 (unsigned long long)(HOUR*24)
#define YEAR                (unsigned long long)(DAY*(365.25))

//frequency definitions
#define Hz                  (1L)
#define KHz                 (Hz*1000L)
#define MHz                 (KHz*1000L)
#define GHz                 (MHz*1000L)

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
