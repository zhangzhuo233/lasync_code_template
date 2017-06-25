#include <iostream>
#include "command.h"
#include "netutils.h"
#include "nfsutils.h"
using namespace std;

int main()
{
    cout<<"ut1:"<<endl;
    std::string nfsdir("/backup/code/leo-test-code/synctool/lasync/syncdir");
    LAMsg* pmsg = (LAMsg*)LANetUtils::build_msg("192.168.2.5", "localhost", 7, nfsdir);

    LACommandExecuter* executer = LACommandExecuter::create_instance();
    executer->parse_command(*pmsg);
    executer->execute_all();

    std::string mp;
    int ret = LANfsUtils::is_nfs_mounted(nfsdir, mp);
    cout<< "mount point"<< mp <<endl;
    cout<< "ret:" << ret <<endl;
    ret = LANfsUtils::umount_nfs(mp);
    cout<< "ret:" << ret <<endl;
    cout<<"ut1:----"<<endl;
    delete pmsg;
    pmsg = NULL;
    delete executer;
    executer = NULL;

    return 0;
}
