#include <iostream>
#include <cstdlib>
#include "nfsutils.h"
using namespace std;

int main()
{
    cout<< " nfs utils UT" <<endl;

    cout<< " is_nfs_exported: ./syncdir: " <<endl;
    bool ret = LANfsUtils::is_nfs_exported("./syncdir");
    cout << "result: " << ret <<endl;

    cout<< " is_nfs_exported: /backup: " <<endl;
    ret = LANfsUtils::is_nfs_exported("/backup");
    cout << "result: " << ret <<endl;
    
    //cout<< " config nfs: ./syncdir: " <<endl;
    //ret = LANfsUtils::config_nfs("./syncdir");
    //cout << "result: " << ret <<endl;

    cout<< " config nfs: ./not exist dir: " <<endl;
    ret = LANfsUtils::config_nfs("./not exist dir");
    cout << "result: " << ret <<endl;
    
    cout<< " export nfs: ./syncdir: " <<endl;
    ret = LANfsUtils::export_nfs("./syncdir");
    cout<< "result: " << ret << endl;
    
    cout<< " show nfs:" <<endl;
    LANfsUtils::show_exported_nfs();


    cout<< " mount nfs to /mnt: " <<endl;
    ret = LANfsUtils::mount_nfs("192.168.3.100:/backup/code/leo-test-code/synctool/lasync/syncdir", "/mnt");
    cout<< "mount nfs:"<<ret <<endl;
    system("mount |grep /mnt");

    std::string mp;
    string aastring("192.168.3.100:/backup/code/leo-test-code/synctool/lasync/syncddir");
    ret = LANfsUtils::is_nfs_mounted(aastring, mp);
    cout<<ret <<endl;


    //cout<< " umount nfs: /mnt" <<endl;
    //ret = LANfsUtils::umount_nfs("/mnt");
    //cout << ret << endl;

    return 0;
}
