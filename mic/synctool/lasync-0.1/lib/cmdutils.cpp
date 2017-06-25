#include "cmdutils.h"
#include <unistd.h>
#include <iostream>

int LACmdUtils::parse_args(int argc, char** argv, char* args, int& cmd, std::string& dest)
{
    int opt_char = 0;

    while((opt_char  = getopt(argc, argv, args)) != -1)
    {
        switch(opt_char)
        {
            // file sync
            case 'f':
                cmd |= 1 << 0;
                break;

            // package install
            case 'p':
                cmd |= 1 << 1;
                break;

            // upgrade
            case 'u':
                cmd |= 1 << 2;
                break;

            // node list
            case 'm':
                dest = optarg;
                break;

            case 'h':
                return 1;
                break;
            //argument error
            //show error message
            case '?':
                return 1;
        }
    }
    return 0;
}

void LACmdUtils::usage()
{
    char* usage = "\t-f: synchronize file \n\t-p: synchronize packages \n\t-u: update packages \n\t-m: node list to do above actions,if no -m all node will be synchronized \n";
    cerr << usage << endl;
}
