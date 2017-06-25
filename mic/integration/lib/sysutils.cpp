#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "sysutils.h"
#include "configparser.h"
#include "exception.h"

ostream& operator<<(ostream& cout,const CpuInfo& cpuinfo)
{
    cout << "model_name:" << cpuinfo.model_name << endl;
    cout << "logic_cores:" << cpuinfo.logic_cores << endl;
    cout << "logic_threads:" << cpuinfo.logic_threads << endl;
    cout << "cache_size:" << cpuinfo.cache_size << endl;
    cout << "frequency:" << cpuinfo.frequency << endl;
    cout << "flags:" << cpuinfo.flags << endl;
    return cout;
}

ostream& operator<<(ostream& cout,const MemInfo& meminfo)
{
    cout << "maxmem:" << meminfo.maxmem << endl;
    cout << "freemem:" << meminfo.freemem << endl;
    cout << "cachedmem:" << meminfo.cachedmem << endl;
    cout << "maxswap:" << meminfo.maxswap << endl;
    cout << "freeswap:" << meminfo.freeswap << endl;
    return cout;
}

ostream& operator<<(ostream& cout,const WorkloadInfo& winfo)
{
    cout << "c15sl:" << winfo.c15sl << endl;
    cout << "c1ml:" << winfo.c1ml << endl;
    cout << "c5ml:" << winfo.c5ml << endl;
    cout << "freemem:" << winfo.freemem << endl;
    cout << "freeswap:" << winfo.freeswap << endl;
    return cout;
}

CpuInfo LAHWInfoCollector::getcpuinfo()
{
    LAConfigParser* parser = NULL;

    try
    {
        parser = LAConfigParser::create_instance("/proc/cpuinfo", ":");
        //parser->dump();

    }
    catch(LAException &e)
    {
        throw LAHardwareException(" CPU");
        delete parser;
        parser = NULL;
    }
    
    CpuInfo info;
    std::string key;
    
    try
    {
        //char* ckeys [] = {"model name", "cache size", "cpu MHz", "flags", "cpu cores", "processor"};
        
        key = "model name";
        info.model_name = parser->read_value(key);

        key = "cache size";
        info.cache_size = parser->read_value(key);

        key = "processor";
        info.logic_threads = parser->read_value(key);

        key = "cpu cores";
        info.logic_cores = parser->read_value(key);

        key = "cpu MHz";
        info.frequency = parser->read_value(key);

        key = "flags";
        info.flags = parser->read_value(key);
    }
    catch(LAException &e)
    {
        throw LAKeyNotFoundException(key);
        delete parser;
        parser = NULL;
    }
    return info;
}

MemInfo LAHWInfoCollector::getmeminfo()
{
    LAConfigParser* parser = NULL;

    try
    {
        parser = LAConfigParser::create_instance("/proc/meminfo", ":");
        //parser->dump();

    }
    catch(LAException &e)
    {
        throw LAHardwareException(" Memory");
        delete parser;
        parser = NULL;
    }

    MemInfo info;
    std::string key;

    try
    {
        //char* ckeys [] = {"Cached", "MemTotal", "MemFree", "SwapTotal", "SwapFree"};
        
        
        key = "Cached";
        info.cachedmem = parser->read_value(key);
    
        key = "MemTotal";
        info.maxmem = parser->read_value(key);

        key = "SwapTotal";
        info.maxswap = parser->read_value(key);

        key = "MemFree";
        info.freemem = parser->read_value(key);

        key = "SwapFree";
        info.freeswap = parser->read_value(key);
    }
    catch(LAException &e)
    {
        throw LAKeyNotFoundException(key);
        delete parser;
        parser = NULL;
    }

    return info;
}

WorkloadInfo LAHWInfoCollector::getworkloadinfo()
{

    FILE* fp = fopen("/proc/loadavg", "r");
    if(fp == NULL)
    {
        throw LAFileCanNotAccessException("/proc/loadavg");
    }

    WorkloadInfo winfo;
    int ret = fscanf(fp, "%f %f %f", &winfo.c15sl, &winfo.c1ml, &winfo.c5ml);
    if(ret != 3)
    {
        fclose(fp);
        throw LAFileCanNotAccessException("/proc/loadavg");
    }
    
    try
    {
        MemInfo minfo = LAHWInfoCollector::getmeminfo();
        
        winfo.freeswap = minfo.freeswap;
        winfo.freemem =  minfo.freemem;
    }
    catch(LAException &e)
    {
        throw LAWorkloadException(e.info);
    }

    return winfo;

}

int LAHWInfoCollector::getdiskinfo(DiskInfo *diskinfo)
{
    cout << "in get disk info" <<endl;
    return 0;
}

int LAHWInfoCollector::getpciinfo(PciInfo *pciinfo)
{
    cout << "in get pci info" << endl;
    return 0;
}

int LAHWInfoCollector::getvgainfo(VideoInfo *videoinfo)
{
    cout << "in get vga info " << endl;
    return 0;
}
