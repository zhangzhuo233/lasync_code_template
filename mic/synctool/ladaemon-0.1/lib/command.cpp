#include "command.h"
#include "fsutils.h"
#include "nfsutils.h"
#include "netutils.h"
#include "strutils.h"
#include "logutils.h"
#include "constants.h"
#include "scriptexecutor.h"
#include "exception.h"
#include "configparser.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

LALogUtils* logger = LALogUtils::get_instance_ptr();

//file sync command
LAFileSyncCommand::LAFileSyncCommand(const std::string& nfsdir)
{
    this->nfsdir = nfsdir;
}

LAFileSyncCommand::~LAFileSyncCommand()
{

}

int LAFileSyncCommand::execute()
{
    //cout<< "sync" << endl;
    logger->info_log("file sync command: nfsdir: %s", this->nfsdir.c_str());
    std::string tempdir;
    int ret = LANfsUtils::is_nfs_mounted(this->nfsdir, tempdir);
    if(!ret)
    {
        LAFsUtils::create_temp_dir("/tmp/mountdir-XXXXXX", tempdir);

        //cout<< "not mounted, create temp dir"<< tempdir <<endl;
        //cout<< this->nfsdir <<endl;
        LANfsUtils::mount_nfs(this->nfsdir.c_str(), tempdir);
    }

    std::string copy_cmd("cp -r ");
    copy_cmd += tempdir + "/* ./syncdir";
    ret = system(copy_cmd.c_str());
    logger->info_log("file sync command: retval: %d", ret);

    return ret;
}

LAFileSyncCommand* LAFileSyncCommand::create_instance(const std::string& nfsdir)
{
    return new LAFileSyncCommand(nfsdir);
}

//pkg install/remove command
LAPkgCommand::LAPkgCommand()
{
}

LAPkgCommand::~LAPkgCommand()
{
}

//verify whether it is a valid package list line.
//valid add package line
//+pkg1 pkg2 pkg3
//valid remove package line
//-pkg1 pkg2 pkg3
bool LAPkgCommand::verify_pkg_list(const std::string& line, std::string& ret, bool& flag)
{
    bool retval = LAStrUtils::trim(line, ret);
    
    //cout<< ret <<endl;
    if(LAStrUtils::startswith(ret.c_str(), "+") )
    {
        flag = true;
        return true;
    }
    else if (LAStrUtils::startswith(ret.c_str(), "-"))
    {
        flag = false;
        return true;
    }
    return false;
}

bool LAPkgCommand::read_pkg_list()
{
    std::string pkglistfile("./syncdir/pkg.lst");
    bool ret = LAFsUtils::isfile(pkglistfile.c_str());
    if(!ret)
    {
        return false;
    }
    
    ifstream infile(pkglistfile.c_str());
    std::string line;
    while(!infile.eof())
    {
        std::getline(infile, line);
        if(line.length() == 0)
        {
            continue;
        }
        //cout<< line <<endl;
        std::string trimmed_line;
        bool flag;
        ret = this->verify_pkg_list(line, trimmed_line, flag);
        if(!ret)
        {
            continue;
        }
        trimmed_line = trimmed_line.substr(1, trimmed_line.length() - 1);
        if(flag)
        {
            this->install_list += " " + trimmed_line;
        }
        else
        {
            this->remove_list += " " + remove_list += trimmed_line;
        }
    }
    return true;
}

int LAPkgCommand::execute()
{
    this->read_pkg_list();
    //cout<< "apt pkg" << endl;
    int ret = 0;
    std::string redirect_cmd("  >/dev/null 2>&1");
    std::string install_cmd("apt-get install -y ");
    std::string remove_cmd("apt-get remove -y ");

#ifdef __REDHAT__
    //handle redhat case
    install_cmd = "yum install -y";
    remove_cmd = "yum remove -y";
#endif //__REDHAT__

    if(!this->install_list.empty())
    {
        install_cmd += this->install_list + redirect_cmd;
        //cout <<"install_cmd:" << install_cmd << endl;
        logger->info_log("package install: install_cmd: %s, pkglist: %s",
                install_cmd.c_str(), this->install_list.c_str());
        ret = system(install_cmd.c_str());
    }

    if(!this->remove_list.empty())
    {
        remove_cmd += this->remove_list + redirect_cmd;
        logger->info_log("package remove: remove_cmd: %s, pkglist: %s",
                remove_cmd.c_str(), this->remove_list.c_str());
        //cout <<"remove_cmd:" << remove_cmd <<endl;
        ret = system(remove_cmd.c_str());
    }

    return ret;
}

LAPkgCommand* LAPkgCommand::create_instance()
{
    return new LAPkgCommand();
}

//pkg update command
LAUpdateCommand::LAUpdateCommand()
{

}

LAUpdateCommand::~LAUpdateCommand()
{
}

int LAUpdateCommand::execute()
{
    //cout<< "apt upd" << endl;
    int ret = 0;
#ifndef __REDHAT__
    //for debian ubuntu
    std::string upgrade_cmd("apt-get update -y >/dev/null 2>&1");
    ret = system(upgrade_cmd.c_str());

    std::string dist_upgrade_cmd("apt-get dist-upgrade -y --force-yes >/dev/null 2>&1");
    ret = system(dist_upgrade_cmd.c_str());
#else
    //for redhat based distro
    std::string update_cmd("yum update -y > /dev/null 2>&1");
    ret = system(update_cmd.c_str());
#endif
    logger->info_log("update command: ret: %d", ret);
    return ret;
}

LAUpdateCommand* LAUpdateCommand::create_instance()
{
    return new LAUpdateCommand();
}

LAScriptCommand::~LAScriptCommand()
{
}

LAScriptCommand* LAScriptCommand::create_instance()
{
    return new LAScriptCommand();
}

int LAScriptCommand::execute()
{
    //cout<< "script" << endl;
    //
    LAScriptExecutor* executor = LAScriptExecutor::create_instance();
    int ret = executor->execute_all() ;

    delete executor;
    executor = NULL;
    logger->info_log("script command: ret:%d", ret);

    return ret;
}

//command executer
LACommandExecuter::LACommandExecuter()
{
}

LACommandExecuter::~LACommandExecuter()
{

}

LACommandExecuter* LACommandExecuter::create_instance()
{
    return new LACommandExecuter();
}

std::deque<LACommand*>* LACommandExecuter::parse_command(const LAMsg& msg)
{
    int signal;
    std::string ip;
    std::string nfsdir;
    std::string dest;


    int ret = LANetUtils::extract_msg(&msg, ip, dest, signal, nfsdir);
    logger->info_log("remote info: dest list: %s, remote nfs:%s, remote host:%s, signal:%d",
            dest.c_str(), nfsdir.c_str(), ip.c_str(), signal);
    
    //get local network info
    std::string hostname;
    ret = LANetUtils::get_local_hostname(hostname);
    if(ret)
    {
        perror("Get local hostname:");
        exit(1);
    }
    
    std::string local_ip;
    std::string local_broadcast;
    std::string device;
    
    LAConfigParser* config_parser = LAConfigParser::create_instance("etc/ladaemon.conf");
    try
    {
        device = config_parser->read_value("device");
    }
    catch(LAException &e)
    {
        logger->error_log("warn, can not found device, use eth0 as default");
        device.assign("eth0");
    }
    
    ret = LANetUtils::get_nic_info(device.c_str(), local_ip, local_broadcast);
    if(ret)
    {
        perror("Get local NIC info:");
        exit(1);
    }
    
    logger->info_log("local info: ip:%s, device:%s, hostname:%s, broadcast:%s",
            local_ip.c_str(), device.c_str(), hostname.c_str(), local_broadcast.c_str());
    
    if(!dest.empty())
    {
        if(!LAStrUtils::in_list(dest, local_ip) && !LAStrUtils::in_list(dest, hostname))
        {
            logger->info_log("local host does not in dest list, skip update");
            return NULL;
        }
    }
    else
    {
        logger->info_log("get broadcast from %s.", ip.c_str());
    }

    //parse signal
    int mask = 1;
    if(signal & (mask << 0))
    {
        //cout<< "file update command" <<endl;
        std::string nfsloc(ip + ":" + nfsdir);
        LACommand* file_sync_cmd = LAFileSyncCommand::create_instance(nfsloc);
        this->command_queue.push_back(file_sync_cmd);
    }
    
    if(signal & (mask << 1))
    {
        //cout<< "package install/remove command" <<endl;
        LACommand* pkg_cmd = LAPkgCommand::create_instance();
        this->command_queue.push_back(pkg_cmd);
    }

    if(signal & (mask << 2))
    {
        //cout<< "package update command" <<endl;
        LACommand* update_cmd =  LAUpdateCommand::create_instance();
        this->command_queue.push_back(update_cmd);
    }
    
    LACommand* script_cmd = LAScriptCommand::create_instance();
    this->command_queue.push_back(script_cmd);

    return &(this->command_queue);
}

int LACommandExecuter::execute_all()
{
    int ret = 0;
    for(deque<LACommand*>::iterator it = command_queue.begin();
            it != command_queue.end(); ++it)
    {
        LACommand* pcommand = *it;
        pcommand->execute();
    }
    return ret;
}

bool LACommandExecuter::is_queue_empty()
{
    return this->command_queue.empty();
}

LACommand* LACommandExecuter::get_first_command()
{
    return *(this->command_queue.begin());
}

void LACommandExecuter::pop_first_command()
{
    this->command_queue.pop_front();
    return ;
}
