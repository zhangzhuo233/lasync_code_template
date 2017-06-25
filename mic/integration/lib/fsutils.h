#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

#include <string>
using namespace std;

class LAFsUtils
{
    public:
        static bool exists(const char* path);
        static bool isfile(const char* path);
        static bool isdirectory(const char* path);
        static bool isdirectory_exists(const char* path);
        static bool create_temp_dir(const char* prefix, std::string& tempdir);
        static bool realpath(const char* path, std::string& realpath);
};

#endif
