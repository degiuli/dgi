/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include "dgitracer.h"

using namespace dgi;

/*
** Ascii trace class
*/
DGITracer::DGITracer(TypeName tracefile,unsigned int filelimit)
{
    //check input
    if(tracefile.size()<=5)
    {
        throw std::invalid_argument("Invalid trace file name");
    }

    //set the file name
    if(TypeFile::type_size==sizeof(wchar_t))
    {
        //convert filename from wide char to narrow
        std::setlocale(LC_ALL, "en_US.UTF-8");
        for(auto x=0;x<tracefile.size();x++)
        {
            auto c = wctob(tracefile[x]);
            tracefilename_ += (char)c;
        }
    }
    else
    {
        tracefilename_ = tracefile;
    }

    //open file and to to the end
    tracefile_.open(tracefilename_.c_str(), std::ios_base::out | std::ios_base::app);
    tracefile_.seekp(0, std::ios_base::end);        //go to the end of the file
    filelimit_ = filelimit;
    checklimit_ = 0;
}
    
/*
DGITracer::DGITracer(const DGITracer &tracer)
{
    tracefile_ = tracer.tracefile_;
    filelimit_ = tracer.filelimit_;
    tracefilename_ = tracer.tracefilename_;
    //syncher_ = tracer.syncher_;
    checklimit_ = 0;
}

DGITracer& DGITracer::operator=(const DGITracer& tracer)
{
    tracefile_ = tracer.tracefile_;
    filelimit_ = tracer.filelimit_;
    tracefilename_ = tracer.tracefilename_;
    //syncher_ = tracer.syncher_;
    checklimit_ = 0;
    return *this;
}

DGITracer::DGITracer(const DGITracer&& tracer)
{
    tracefile_ = std::move(tracer.tracefile_);
    tracefilename_ = std::more(tracer.tracefilename_);
    //syncher_ = std::move(tracer.syncher_);
    filelimit_ = tracer.filelimit_;
    checklimit_ = 0;
}

DGITracer& DGITracer::operator=(const DGITracer&& tracer)
{
    std::move(tracefile_,tracer.tracefile_);
    std::move(tracefilename_,tracer.tracefilename_);
    //std::move(syncher_,tracer.syncher_);
    filelimit_ = tracer.filelimit_;
    checklimit_ = 0;
    return *this;
}
*/

DGITracer::~DGITracer()
{
    //close file if opened
    if(tracefile_.is_open())
    {
        tracefile_.close();
    }
}
    
void DGITracer::CheckFileSize()
{
    //std::unique_lock<std::mutex> lock(syncher_);

    //the file size check should not be performed on every write, but only after 10 writes
    if(checklimit_<10)
    {
        checklimit_++;
        return;
    }
    
    //reset check time
    checklimit_ = 0;

    //go to the end of the file and get the position
    tracefile_.seekp(0, std::ios_base::end);
    auto pos = tracefile_.tellp();

    if(pos>filelimit_ && filelimit_>0)
    {
        std::string newfilename = tracefilename_;
        newfilename += "_bkp";

        //close current file, rename it to the backup and open a new again
        tracefile_.close();
        rename(tracefilename_.c_str(),newfilename.c_str());
        tracefile_.open(tracefilename_.c_str(), std::ios_base::out | std::ios_base::app);
    }
}

void DGITracer::SendTrace(unsigned int id,const TypeFile *format,...)
{
    if(!tracefile_.is_open())
    {
        throw std::logic_error("Trace file not opened");
    }

    //check size before continue
    CheckFileSize();

    //std::lock_guard<std::mutex> lock(syncher_);
    TypeFile buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)&tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|T|" << id << "|" << buffer << "\n";
}
    
void DGITracer::SendInformation(unsigned int id,const TypeFile *format,...)
{
    if(!tracefile_.is_open())
    {
        throw std::logic_error("Trace file not opened");
    }

    //check size before continue
    CheckFileSize();

    //std::lock_guard<std::mutex> lock(syncher_);
    TypeFile buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)&tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|I|" << id << "|" << buffer << "\n";
}
    
void DGITracer::SendWarning(unsigned int id,const TypeFile *format,...)
{
    if(!tracefile_.is_open())
    {
        throw std::logic_error("Trace file not opened");
    }

    //check size before continue
    CheckFileSize();

    //std::lock_guard<std::mutex> lock(syncher_);
    TypeFile buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)&tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|W|" << id << "|" << buffer << "\n";
}
    
void DGITracer::SendError(unsigned int id,const TypeFile *format,...)
{
    if(!tracefile_.is_open())
    {
        throw std::logic_error("Trace file not opened");
    }

    //check size before continue
    CheckFileSize();

    //std::lock_guard<std::mutex> lock(syncher_);
    TypeFile buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)&tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|E|" << id << "|" << buffer << "\n";
}
    
void DGITracer::SendFatal(unsigned int id,const TypeFile *format,...)
{
    if(!tracefile_.is_open())
    {
        throw std::logic_error("Trace file not opened");
    }

    //check size before continue
    CheckFileSize();

    //std::lock_guard<std::mutex> lock(syncher_);
    TypeFile buffer[KBYTES] = {0};
    va_list argptr;
        
    va_start(argptr,format);
    //vsnprintf(buffer,sizeof(buffer)-1,format,argptr);
	vsprintf(buffer,format,argptr);
    va_end(argptr);

    time_t tracetime = time(NULL);
    struct tm *current_time = localtime((const time_t*)&tracetime);
    tracefile_ << current_time->tm_year+1900 << current_time->tm_mon+1
                << current_time->tm_mday << current_time->tm_hour
                << current_time->tm_min << current_time->tm_sec
                << "|F|" << id << "|" << buffer << "\n";
}
