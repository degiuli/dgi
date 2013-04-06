/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include "dgitracer.h"

/*
** Unicode trace class
*/
DGITracerW::DGITracerW(wstring tracefile)
{
    //check input
    if(tracefile.size()>5)
    {
        throw invalid_argument("Invalid trace file name");
    }
        
    //convert filename from wide char to narrow
    std::setlocale(LC_ALL, "en_US.UTF-8");
    string filename;
    for(auto x=0;x<tracefile.size();x++)
    {
        auto c = wctob(tracefile[x]);
        filename += (char)c;
    }
        
    //open file and to to the end
    tracefile_.open(filename.c_str(), ios_base::out | ios_base::app);
}
    
DGITracerW::DGITracerW(const DGITracerW &tracer)
{
    tracefile_ = tracer.tracefile_;
}

DGITracerW& DGITracerW::operator=(const DGITracerW& tracer)
{
    tracefile_ = tracer.tracefile_;
    return *this;
}

DGITracerW::DGITracerW(const DGITracerW&& tracer)
{
    tracefile_ = move(tracer.tracefile_);
}

DGITracerW& DGITracerW::operator=(const DGITracerW&& tracer)
{
    move(tracefile_,tracer.tracefile_);
    return *this;
}

DGITracerW::~DGITracerW()
{
    //close file if opened
    if(tracefile_.is_open())
    {
        tracefile_.close();
    }
}

void DGITracerW::SendTrace(unsigned int id,const wchar_t *format,...)
{
    wchar_t buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    vswprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|T|" << id << "|" << buffer << "\n";
}
    
void DGITracerW::SendInformation(unsigned int id,const wchar_t *format,...)
{
    wchar_t buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    vswprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|I|" << id << "|" << buffer << "\n";
}
    
void DGITracerW::SendWarning(unsigned int id,const wchar_t *format,...)
{
    wchar_t buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    vswprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|W|" << id << "|" << buffer << "\n";
}
    
void DGITracerW::SendError(unsigned int id,const wchar_t *format,...)
{
    wchar_t buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    vswprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|E|" << id << "|" << buffer << "\n";
}
    
void DGITracerW::SendFatal(unsigned int id,const wchar_t *format,...)
{
    wchar_t buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    vswprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|F|" << id << "|" << buffer << "\n";
}
