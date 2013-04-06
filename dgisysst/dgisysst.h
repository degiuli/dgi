/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef _SYS_STATUS_GLOBALS_
#define _SYS_STATUS_GLOBALS_

#include <iostream>
#include <string>
#include <memory>

#include "dgidefines.h"
#include "dgitracer.h"

#define APP_STATUS_NONE         0
#define APP_STATUS_IDLE         1
#define APP_STATUS_STARTING     2
#define APP_STATUS_STOPPING     3
#define APP_STATUS_PAUSED       4

class CGlobals
{
private:
    dgi::DGITracer* m_ptracer;
    bool m_sysdetails;
    unsigned long long m_timer;
    unsigned long m_logsize;
    std::string m_logpath;
    std::string m_logfile;
    unsigned short m_appstatus;

public:
    //default constructor
    CGlobals() : m_ptracer(NULL), 
                 m_sysdetails(false),
                 m_timer(MINUTE),
                 m_logsize(MBYTES),
                 m_appstatus(APP_STATUS_NONE)
    {}
    
    //copy constructors
    CGlobals(const CGlobals &g)
    {
        m_ptracer = g.m_ptracer;
        m_sysdetails = g.m_sysdetails;
        m_timer = g.m_timer;
        m_logsize = g.m_logsize;
        m_logpath = g.m_logpath;
        m_logfile = g.m_logfile;
        m_appstatus = g.m_appstatus;
    }
    CGlobals& operator=(const CGlobals& g)
    {
        m_ptracer = g.m_ptracer;
        m_sysdetails = g.m_sysdetails;
        m_timer = g.m_timer;
        m_logsize = g.m_logsize;
        m_logpath = g.m_logpath;
        m_logfile = g.m_logfile;
        m_appstatus = g.m_appstatus;
        return *this;
    }
/*    
    //move constructores
    CGlobals(const CGlobals&& g)
    {
        m_ptracer = move(g.m_ptracer);
        m_sysdetails = g.m_sysdetails;
        m_timer = g.m_timer;
        m_logsize = g.m_logsize;
        m_logpath = move(g.m_logpath);
        m_logfile = move(g.m_logfile);
        m_appstatus = g.m_appstatus;
    }
    
    CGlobals& operator=(const CGlobals&& g)
    {
        m_ptracer = move(g.m_ptracer);
        m_sysdetails = g.m_sysdetails;
        m_timer = g.m_timer;
        m_logsize = g.m_logsize;
        m_logpath = move(g.m_logpath);
        m_logfile = move(g.m_logfile);
        m_appstatus = g.m_appstatus;
        return *this;
    }
*/    

    //destructor
    ~CGlobals() {}

    //tracer
    void setTracer(dgi::DGITracer* ptracer)
    {
        m_ptracer = ptracer;
    }
    dgi::DGITracer* getTracer()
    {
        return m_ptracer;
    }

    //system details
    void setSysDetails(bool sysdetails)
    {
        m_sysdetails = sysdetails;
    }
    bool getSysDetails()
    {
        return m_sysdetails;
    }

    //timer
    void setTimer(unsigned long long timer)
    {
        m_timer = timer;
    }
    unsigned long long getTimer()
    {
        return m_timer;
    }

    //logsize
    void setLogSize(unsigned long logsize)
    {
        m_logsize = logsize;
    }
    unsigned long getLogSize()
    {
        return m_logsize;
    }

    //logfile
    void setLogFile(std::string logfile)
    {
        m_logfile = logfile;
    }
    std::string getLogFile()
    {
        return m_logfile;
    }

    //logpath
    void setLogPath(std::string logpath)
    {
        m_logpath = logpath;
    }
    std::string getLogPath()
    {
        return m_logpath;
    }

    //appstatus
    void setAppStatus(unsigned short appstatus)
    {
        m_appstatus = appstatus;
    }
    unsigned short getAppStatus()
    {
        return m_appstatus;
    }
};

#endif _SYS_STATUS_GLOBALS_
