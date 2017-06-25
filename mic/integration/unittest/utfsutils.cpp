#include "fsutils.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
    cout << " file system utils ut " <<endl;

    cout << LAFsUtils::isfile("/etc") <<endl;
    cout << LAFsUtils::isfile("/etc/exports") <<endl;
    cout << LAFsUtils::isdirectory("/etc/exports") <<endl;
    cout << LAFsUtils::isdirectory("/etc") <<endl;
    cout << LAFsUtils::exists("none exist file") <<endl;
    cout << LAFsUtils::exists("etc") <<endl;
    cout << LAFsUtils::exists("/etc") <<endl;

    std::string tempdir;
    bool ret = LAFsUtils::create_temp_dir("/tmp/mountdir-XXXXXX", tempdir);
    cout<< ret <<endl;

    std::string rpath;
    LAFsUtils::realpath("log/log.txt", rpath);
    cout<<rpath<<endl;

    cout<<tempdir<<endl;

    return 0;
}
