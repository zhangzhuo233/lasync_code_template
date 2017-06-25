#include "netutils.h"
#include <iostream>
using namespace std;

int main()
{
    cout<< "ut: get_local_hostname " << endl;
    std::string hostname;
    LANetUtils::get_local_hostname(hostname);
    cout<< hostname << endl;
    
    std::string ret="";

    LANetUtils::get_ip_by_hostname(hostname, ret);
    LANetUtils::get_ip_by_hostname(string("lacentos"), ret);
    int cc = LANetUtils::get_ip_by_hostname(string(""), ret);
    cout << "cc is:" << cc << endl;


    return 0;
}
