#include "netutils.h"
#include <iostream>
using namespace std;

int main()
{
    cout<< "ut: get_local_hostname " << endl;
    std::string hostname;
    LANetUtils::get_local_hostname(hostname);
    cout<< hostname << endl;
    return 0;
}
