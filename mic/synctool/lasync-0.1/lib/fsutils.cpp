//system libraries 
#include <unistd.h>
#include <sys/stat.h>

//c/c++ headers
#include <iostream>
#include <cstdlib>
#include <string.h>

//our headers
#include "fsutils.h"

bool LAFsUtils::realpath(const char* path, std::string& realpath)
{
    char* result = ::realpath(path, NULL);
    realpath.assign(result);
    return true;
}

bool LAFsUtils::create_temp_dir(const char* prefix, std::string& tempdir)
{
    if(strlen(prefix) >= 32)
    {
        return false;
    }
    char buff[32];
    strcpy(buff, prefix);

    char* tmpl = mktemp(buff);

    if(!tmpl)
    {
        return false;
    }

    tempdir.assign(buff, strlen(buff)); 

    int ret = mkdir(tempdir.c_str(), 0755);
    if(ret)
    {
        return false;
    }
    return true;
}

bool LAFsUtils::exists(const char* path)
{
    return !access(path, F_OK);
}

bool LAFsUtils::isfile(const char* path)
{
    struct stat statinfo;
    int ret = stat(path, &statinfo);
    if(ret < 0)
    {
        return false;
    }

    return S_ISREG(statinfo.st_mode);
}


bool LAFsUtils::isdirectory(const char* path)
{
    struct stat statinfo;
    int ret = stat(path, &statinfo);
    if(ret < 0)
    {
        return false;
    }

    return S_ISDIR(statinfo.st_mode);
}

bool LAFsUtils::isdirectory_exists(const char* path)
{
    if(!LAFsUtils::exists(path) || !LAFsUtils::isdirectory(path))
    {
        return false;
    }
    return true;
}
