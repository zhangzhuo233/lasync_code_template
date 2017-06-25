// unix/linux headers
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// standard c/c++ headers
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>

// self headers
#include "lib/netutils.h"
#include "lib/cmdutils.h"
#include "lib/configparser.h"
#include "lib/fsutils.h"
#include "lib/nfsutils.h"

using namespace std;

int main(int argc, char** argv)
{
    if(getuid())
    {
        cerr << "Permission denied, only root can run this tool" << endl;
        exit(1);
    }
    int cmd = 0;
    std::string dest;
    std::string client_cmd;
    char* args = "fpuhm:";

    int ret = LACmdUtils::parse_args(argc, argv, args, cmd, dest);
    if(ret || !cmd)
    {
        LACmdUtils::usage();
        exit(1);
    }

    std::string ip, broadcast;
    LAConfigParser* parser = LAConfigParser::create_instance();
    std::string device = parser->read_value("device");
    std::string syncdir = parser->read_value("syncdir");
    std::string abs_syncdir;
    LAFsUtils::realpath(syncdir.c_str(), abs_syncdir);

    ret = LANetUtils::get_nic_info(device.c_str(), ip, broadcast);

    delete parser;
    parser = NULL;

    cout<<"Summary:"<<endl;
    cout<<"    Config Info :" <<endl;
    cout<<"        Device    :" << device <<endl;
    cout<<"        Sync Dir  :" << abs_syncdir <<endl;

    cout<<"    Nic info    :"<<endl;
    cout<<"        IP addr   :"<<ip<<endl;
    cout<<"        Broadcast :"<<broadcast<<endl;

    cout<<"    Parm Info   :"<<endl;
    cout<<"        Cmd type  :"<<cmd<<endl;

    if(dest.empty())
    {
        cout<<"        dest addr :"<<broadcast<<endl;
    }
    else
    {
        cout<<"        dest addr :"<<dest<<endl;
    }

    ret = LANfsUtils::export_nfs(abs_syncdir);
    if(!ret)
    {
        cerr<< "nfs can not be exported, please check /etc/exports" << endl;
    }

    void* pmsg = LANetUtils::build_msg(ip, dest, cmd, abs_syncdir);
    ret = LANetUtils::send_msg(pmsg, broadcast, device);
    
    free(pmsg);

    return 0;
}
