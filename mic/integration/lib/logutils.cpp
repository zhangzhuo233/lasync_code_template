#include "fsutils.h"
#include "logutils.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>

#include <string.h>
#include <stdarg.h>
#include <cstdio>

LALogUtils::LALogUtils()
{
}

void LALogUtils::log(const int& loglevel, const char* format, ...)
{
    static char buff[4096];

    va_list vl;
    va_start(vl, format);
    vsprintf(buff,format, vl);
    va_end(vl);
    
    openlog("laworks", LOG_CONS | LOG_PID, 0);
    syslog(loglevel | LOG_LOCAL1, buff);
    closelog();
}

void LALogUtils::info_log(const char* format, ...)
{
    static char buff[4096];

    va_list vl;
    va_start(vl, format);
    vsprintf(buff,format, vl);
    va_end(vl);

    this->log(LOG_INFO, buff); 
}

void LALogUtils::debug_log(const char* format, ...)
{
    static char buff[4096];

    va_list vl;
    va_start(vl, format);
    vsprintf(buff,format, vl);
    va_end(vl);

    this->log(LOG_DEBUG, buff);
}

void LALogUtils::error_log(const char* format, ...)
{
    static char buff[4096];

    va_list vl;
    va_start(vl, format);
    vsprintf(buff,format, vl);
    va_end(vl);

    this->log(LOG_ERR, buff);
}

