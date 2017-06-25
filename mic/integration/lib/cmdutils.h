#ifndef __CMD_UTILS_HEADER__
#define __CMD_UTILS_HEADER__

#include <string>
using namespace std;
class LACmdUtils
{
    public:
        static int parse_args(int argc, char** argv, char* args, int& cmd, std::string& dest);
        static void usage();
};

#endif
