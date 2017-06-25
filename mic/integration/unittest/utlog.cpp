#include "logutils.h"

#include <iostream>

#include <syslog.h>
using namespace std;

int main()
{
    LALogUtils* logger = LALogUtils::get_instance_ptr();
    cout<< LALogUtils::get_instance_ptr()<<endl;
    logger->log(LOG_DEBUG, "test\n");
    logger->log(LOG_DEBUG, "this is a test %s\n", "hahas");

    logger->info_log("this is a info log");
    logger->debug_log("this is a debug log");
    logger->error_log("this is an error log");

    cout<< LALogUtils::get_instance_ptr()<<endl;
    LALogUtils::remove_instance();
    return 0;
}
