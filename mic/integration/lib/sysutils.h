#ifndef __HW_UTILS_HEADER__
#define __HW_TUILS_HEADER__

#include <string>
using namespace std;

struct CpuInfo 
{
    string model_name;
    string logic_cores;
    string logic_threads;
    string cache_size;
    string frequency;
    string flags;
    friend ostream& operator<<(ostream& cout,const CpuInfo& cpuinfo);
};

struct MemInfo 
{
    string maxmem;
    string freemem;
    string cachedmem;
    string maxswap;
    string freeswap;
    friend ostream& operator<<(ostream& cout,const MemInfo& meminfo);
};

struct WorkloadInfo
{
    float c15sl;      // cpu load for past 15 seconds
    float c1ml;       // cpu load for past 1 min
    float c5ml;      // cpu load for past 15 mins
    string freemem;    // free memeory
    string freeswap;   // free swap
    friend ostream& operator<<(ostream& cout,const WorkloadInfo& winfo);
};

struct DiskInfo
{
    string modelname;
    string size;
    string partitions;
};

struct VideoInfo
{
    string modelname;
};

struct PciInfo
{
    string modelname;
};

class LAHWInfoCollector 
{
public:
    static CpuInfo getcpuinfo();
    static MemInfo getmeminfo();
    static WorkloadInfo getworkloadinfo();
    static int getdiskinfo(DiskInfo *diskinfo);
    static int getpciinfo(PciInfo *pciinfo);
    static int getvgainfo(VideoInfo *videoinfo);
};

#endif
