#include "nfsutils.h"
#include "fsutils.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <string.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

bool LANfsUtils::is_nfs_mounted(const std::string& nfsdir, std::string& mp)
{
    std::string mount_cmd("mount |grep ");
    mount_cmd += "'" + nfsdir + "'";
    //cout << "mount cmd string:" << mount_cmd <<endl;
    //cout << "mount cmd c_str:" << mount_cmd.c_str() <<endl;
    int ret = system((mount_cmd + " >/dev/null 2>&1").c_str());
    if(ret)
    {
        return false;
    }
    mount_cmd += " |awk '{print $3}'";
    FILE* output = popen(mount_cmd.c_str(), "r");
    char buffer[256];
    fgets(buffer, 256, output);
    mp.assign(buffer, strlen(buffer) - 1);
    //cout<<"mount point:"<<buffer<<endl;

    pclose(output);

    return true;
}

bool LANfsUtils::is_nfs_exported(const std::string& nfsdir)
{
    if(!LAFsUtils::isdirectory_exists(nfsdir.c_str()))
    {
        return false;
    }

    std::string realpath;
    LAFsUtils::realpath(nfsdir.c_str(), realpath);

    std::string cmd("exportfs |awk '{print $1}'");

    FILE* output = popen(cmd.c_str(), "r");
    char buffer[256];

    while(fgets(buffer, 256, output) != NULL)
    {
        std::string export_dir(buffer);
        if(export_dir == realpath + "\n")
        {
            pclose(output);
            return true;
        }
    }

    pclose(output);
    return false;
}

bool LANfsUtils::config_nfs(const std::string nfsdir,
 const std::string& config_file)
{
    if(!LAFsUtils::isdirectory_exists(nfsdir.c_str()))
    {
        return false;
    }

    std::string realpath;
    LAFsUtils::realpath(nfsdir.c_str(), realpath);

    int nfs_conf_fd = open(config_file.c_str(), O_WRONLY | O_APPEND);
    std::string line(realpath + " *(rw,sync,no_subtree_check,no_root_squash)\n");
    write(nfs_conf_fd, line.c_str(), line.length());
    close(nfs_conf_fd);

    return true;
}

bool LANfsUtils::export_nfs(const std::string& nfsdir)
{
    if(LANfsUtils::is_nfs_exported(nfsdir))
    {
        return true;
    }

    LANfsUtils::config_nfs(nfsdir);
    
#ifdef __REDHAT__
    std::string reloadnfs_cmd("service nfs reload > /dev/null 2>&1");
#else
    std::string reloadnfs_cmd("service nfs-kernel-server reload > /dev/null 2>&1");
#endif
    
    int ret = system(reloadnfs_cmd.c_str());

    return !ret;
}

bool LANfsUtils::mount_nfs(const std::string& nfsdir, const std::string& dest)
{
    if(!LAFsUtils::isdirectory_exists(dest.c_str()))
    {
        return false;
    }

    std::string realpath;
    LAFsUtils::realpath(dest.c_str(), realpath);

    std::string mountnfs_cmd("mount ");
    mountnfs_cmd += nfsdir + " " + realpath + " -t nfs > /dev/null 2>&1";
    //cout<< mountnfs_cmd <<endl;
    //cout<< mountnfs_cmd.c_str() <<endl;
    int ret = system(mountnfs_cmd.c_str());
    return !ret;
}

bool LANfsUtils::umount_nfs(const std::string& dest)
{
    if(!LAFsUtils::isdirectory_exists(dest.c_str()))
    {
        return false;
    }

    std::string realpath;
    LAFsUtils::realpath(dest.c_str(), realpath);

    std::string umountnfs_cmd("umount ");
    umountnfs_cmd += realpath;

    int ret = system(umountnfs_cmd.c_str());

    return !ret;
}

void LANfsUtils::show_exported_nfs()
{
    std::string cmd("exportfs |awk '{print $1}'");

    FILE* output = popen(cmd.c_str(), "r");
    char buffer[256];

    while(fgets(buffer, 256, output) != NULL)
    {
        cout << buffer << endl;
    }

    pclose(output);
}
