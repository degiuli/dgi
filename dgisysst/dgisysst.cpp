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

#include "dgisysst.h"
#include "dgiminidump.h"

SERVICE_STATUS ServiceStatus; 
SERVICE_STATUS_HANDLE ServiceStatusHandle; 

dgi::DGITracer* g_ptracer;
CGlobals g_globals;

void Usage()
{
    //       123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-
    std::cout << "Usage:" << std::endl;
    std::cout << "\tDGICrash [runonce] [execmode] [nowindow] [instsrv|uninstsrv] " << std::endl;
    std::cout << "           [sysdetails] [timer:<min>] [logfile:<filename>] \n" << std::endl;
    std::cout << "           [logpath:<filepath>] [logsize:<mbytes>]\n" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\trunonce:" << std::endl;
    std::cout << "\t\trun only one time" << std::endl;
    std::cout << "\texecmode" << std::endl;
    std::cout << "\t\texecute as normal application" << std::endl;
    std::cout << "\tnowindow" << std::endl;
    std::cout << "\t\tdo not show window for execmode" << std::endl;
    std::cout << "\tinstsrvc" << std::endl;
    std::cout << "\t\tinstall the service" << std::endl;
    std::cout << "\tuninstsrv" << std::endl;
    std::cout << "\t\tuninstall the service" << std::endl;
    std::cout << "\tsysinfo\t\tcollect as much details as possible" << std::endl;
    std::cout << "\ttimer:<min>" << std::endl;
    std::cout << "\t\tlog system status in even <min> minutes" << std::endl;
    std::cout << "\tlogfile:<filename>" << std::endl;
    std::cout << "\t\tsave the system status in the <filename>" << std::endl;
    std::cout << "\tlogpath:<filepath>" << std::endl;
    std::cout << "\t\tsave the system status log files in the <filepath>\n" << std::endl;
    std::cout << "\tlogsize:<mbytes>" << std::endl;
    std::cout << "\t\tsize of each log file storage in megabytes\n" << std::endl;
    std::cout << "logpath, logfile, logsize, timer and sysdetails without other" << std::endl;
    std::cout << "options configure the service, which needs to be restarted.\n" << std::endl;
}

void WriteConfigurationFile()
{
    //write the content of globals into the dgisysst.cfg file
}

bool ReadConfigurationFile()
{
    //read the dgisysst.cfg file and put the content into globals
    return false;
}

void ServiceInstall(char *path)
{
    g_ptracer->SendTrace(__LINE__,">> ServiceInstall, %s",path);

    SC_HANDLE mansch = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
    if (!mansch)
    {
        g_ptracer->SendError(__LINE__,"-- ServiceInstall, Unable to OpenSCManager - %u",GetLastError());
        return;
    }

    SC_HANDLE sch = CreateService(mansch,"DGISysStatus","DGI System Status",SERVICE_ALL_ACCESS,
                                  SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,
                                  SERVICE_AUTO_START,SERVICE_ERROR_IGNORE,path,NULL,NULL,NULL,NULL,NULL);
    if (!sch)
        g_ptracer->SendError(__LINE__,"-- ServiceInstall, Unable to CreateService for %s - %u",path,GetLastError());
    if (sch)
        CloseServiceHandle(sch);
    if (mansch)
        CloseServiceHandle(mansch);

    g_ptracer->SendTrace(__LINE__,"<< ServiceInstall");
}

void ServiceUninstall()
{
    g_ptracer->SendTrace(__LINE__,">> ServiceUninstall");

    SC_HANDLE mansch = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
    if (!mansch)
    {
        g_ptracer->SendError(__LINE__,"-- ServiceUninstall, Unable to OpenSCManager - %u",GetLastError());
        return;
    }

    SC_HANDLE sch = OpenService(mansch, "DGISysStatus", STANDARD_RIGHTS_REQUIRED);
    if (!sch)
    {
        g_ptracer->SendError(__LINE__,"-- ServiceUninstall, Unable to OpenService - %u",GetLastError());
    }
    else
    {
        if (!DeleteService(sch))
            g_ptracer->SendError(__LINE__,"-- ServiceUninstall, Unable to DeleteService");
    }
    if (sch)
        CloseServiceHandle(sch);
    if (mansch)
        CloseServiceHandle(mansch);

    g_ptracer->SendTrace(__LINE__,"<< ServiceUninstall");
}

void WINAPI ServiceCtrlHandler(DWORD opcode)
{
    g_ptracer->SendTrace(__LINE__,">> ServiceCtrlHandler");

    switch(opcode)
    {
        case SERVICE_CONTROL_PAUSE:
            //pause main thread loop
            g_ptracer->SendInformation(__LINE__,"-- ServiceCtrlHandler, Pause req");
            ServiceStatus.dwCurrentState = SERVICE_PAUSED;
            g_globals.setAppStatus(APP_STATUS_PAUSED);
            break;

        case SERVICE_CONTROL_CONTINUE:
            //restart main thread loop
            g_ptracer->SendInformation(__LINE__,"-- ServiceCtrlHandler, Continue req");
            ServiceStatus.dwCurrentState = SERVICE_RUNNING;
            g_globals.setAppStatus(APP_STATUS_IDLE);
            break;

        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
            //start thread to process stop so we can return to SCM
            g_ptracer->SendInformation(__LINE__,"-- ServiceCtrlHandler, Stop/Shutdown req");
            
            //stop main thread and its dependencies
            g_globals.setAppStatus(APP_STATUS_STOPPING);
            //(...)

            //tell SCM the service is terminating
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOP_PENDING;
            ServiceStatus.dwCheckPoint    = 0;
            ServiceStatus.dwWaitHint      = 5000l;
            break;

        case SERVICE_CONTROL_INTERROGATE:
            g_ptracer->SendInformation(__LINE__,"-- ServiceCtrlHandler, Interrogate req");
            break;

        default:
            g_ptracer->SendWarning(__LINE__,"-- ServiceCtrlHandler, Unrecognized req, controlCode %ld",opcode);
    } 

    //send current status
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

    g_ptracer->SendTrace(__LINE__,"<< ServiceCtrlHandler");
    return;
}

void WINAPI ServiceStart(DWORD dwNumServicesArgs,LPSTR *lpServiceArgVectors)
{
    g_ptracer->SendTrace(__LINE__,">> ServiceStart");

    //register service control handler
    ServiceStatusHandle = RegisterServiceCtrlHandler("DGISysStatus",ServiceCtrlHandler);

    if(ServiceStatusHandle == NULL)
    {
        g_ptracer->SendFatal(__LINE__,"-- ServiceStart, RegisterServiceCtrlHandler fail, LastErrCode %u",GetLastError());
        return;
    }

    //update status information
    ServiceStatus.dwServiceType             = SERVICE_WIN32;
    ServiceStatus.dwCurrentState            = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted        = SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode           = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint              = 0;
    ServiceStatus.dwWaitHint                = 20001;

    if (!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
        g_ptracer->SendFatal(__LINE__,"-- ServiceStart, SetServiceStatus fail, LastErrCode %u",GetLastError());

    //indicates the service is starting
    g_globals.setAppStatus(APP_STATUS_STARTING);
    
    //load dgisysst.cfg file to CGlobal
    g_globals.setTracer(g_ptracer);
    
    if(!ReadConfigurationFile())
    {
        //failed to load the configuration
        //stop service
        //  Start thread to process stop so we can return to SCM
        g_ptracer->SendInformation(__LINE__,"-- ServiceStart, Unable to load cfg; stop service");
        
        //stop main thread and its dependencies
        g_globals.setAppStatus(APP_STATUS_NONE);
        
        //tell SCM the service is terminating
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState  = SERVICE_STOP_PENDING;
        ServiceStatus.dwCheckPoint    = 0;
        ServiceStatus.dwWaitHint      = 5001;
        
        g_ptracer->SendTrace(__LINE__,"<< ServiceStart");
        return;
    }
    //(...)

    //initialization complete - report running status
    ServiceStatus.dwCurrentState       = SERVICE_RUNNING;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 0;
    if (!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
        g_ptracer->SendError(__LINE__,"-- ServiceStart, SetServiceStatus fail, LastErrCode %u",GetLastError());

    g_globals.setAppStatus(APP_STATUS_IDLE);
    g_ptracer->SendTrace(__LINE__,"<< ServiceStart");
    return;
}

int __cdecl main(int argc,char *argv[])
{
    char temp[1024] = {0};
    GetCurrentDirectory(sizeof(temp)-1,temp);
    if(temp[strlen(temp)-1]!='\\' && strlen(temp)<sizeof(temp)-1)
        temp[strlen(temp)-1] = '\\';
    std::string appdir = temp;

    std::string file = appdir;
    file += "dgisysst.dbg";
    dgi::DGITracer tracer(file);

    file = appdir;
    file += "dgisysst";
    MiniDumpInit(tracer,file);

    g_ptracer->SendTrace(__LINE__,">> DGISysSt");

    g_globals.setTracer(&tracer);

    //consist arguments
    if(argc<=1)
    {
        static SERVICE_TABLE_ENTRY DispatchTable[] = { {"DGISysStatus", ServiceStart}, {NULL, NULL} };

		if (!StartServiceCtrlDispatcher(DispatchTable))
        {
			g_ptracer->SendFatal(__LINE__,"StartSrvcCtrlDisp fail, LastErrCode %u",GetLastError());

            //print usage and exit
            Usage();
            MiniDumpEnd();
            g_ptracer->SendTrace(__LINE__,"<< DGISysSt");
            return 1;
        }
    }
    else
    {
        //collect and consist all arguments
        bool instsrv = false;
        bool uninstsrv = false;
        bool execmode = false;
        bool nowindow = true;                       //has window in execmode
        unsigned long long timer = MINUTE;          //default 1 minute
        unsigned long logsize = MBYTES;             //default 1 mbytes
        std::string logpath = appdir;                    //default path is the app dir
        std::string logfile = appdir + "dgisysst.log";   //default file is in the app dir

        std::string args;
        g_ptracer->SendTrace(__LINE__,"-- Options (%i):",argc);
        for(int x=1;x<argc;x++)
        {
            g_ptracer->SendTrace(__LINE__,"\t%s",argv[x]);
            args.append(argv[x]);
            args += " ";

            //is log execmode?
            if(strstr(argv[x],"execmode"))
            {
                execmode = true;
                continue;
            }

            //is log nowindow?
            if(strstr(argv[x],"nowindow"))
            {
                nowindow = true;
                continue;
            }

            //is log instsrv?
            if(strstr(argv[x],"instsrv"))
            {
                instsrv = true;
                continue;
            }

            //is log uninstsrv?
            if(strstr(argv[x],"uninstsrv"))
            {
                uninstsrv = true;
                continue;
            }

        }

        //consist exclusive options
        if((nowindow && instsrv) || (nowindow && uninstsrv) ||
           (execmode && instsrv) || (execmode && uninstsrv) ||
           (instsrv && uninstsrv))
        {
            Usage();
            MiniDumpEnd();
            g_ptracer->SendTrace(__LINE__,"<< DGISysSt");
            return 2;
        }

        //install service?
        if(instsrv)
        {
            ServiceInstall(argv[0]);
            MiniDumpEnd();
            g_ptracer->SendTrace(__LINE__,"<< DGISysSt");
            return 0;
        }
        //uninstall service?
        if(uninstsrv)
        {
            ServiceUninstall();
            MiniDumpEnd();
            g_ptracer->SendTrace(__LINE__,"<< DGISysSt");
            return 0;
        }

        //configuration
        //is log sysdetails?
        if(strstr(args.c_str(),"sysdetails"))
        {
            g_globals.setSysDetails(true);
        }

        //is log size?
        char *plogsize = strstr(args.c_str(),"logsize");
        if(plogsize)
        {
            plogsize += 8;
            logsize = (unsigned long)(atol(plogsize)*MBYTES);
            g_globals.setLogSize(logsize);
        }

        //is log path?
        char *plogpath = strstr(args.c_str(),"logpath");
        if(plogpath)
        {
            plogpath += 8;
            logpath = plogpath;
            g_globals.setLogPath(logpath);
        }

        //is log file?
        char *plogfile = strstr(args.c_str(),"logfile");
        if(plogfile)
        {
            plogfile += 8;
            logfile = logpath + plogfile;
            g_globals.setLogFile(logfile);
        }

        //is timer?
        char *ptimer = strstr(args.c_str(),"timer");
        if(ptimer)
        {
            plogsize += 6;
            timer = (unsigned long)(atol(ptimer)*MINUTE);
            g_globals.setTimer(timer);
        }
        
        //write configuration (globals) into the dgisysst.cfg configuration file
        WriteConfigurationFile();
    }

    MiniDumpEnd();
    g_ptracer->SendTrace(__LINE__,"<< DGISysSt");
    return 0;
}