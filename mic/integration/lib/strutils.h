#ifndef __STR_UTILS_HEADER__
#define __STR_UTILS_HEADER__

#include <vector>
#include <string>
using namespace std;

class LAStrUtils
{
    public:
        static bool startswith(const char* str, const char* start);
        static bool endswith(const char* str, const char* end);

        //remove leading space and tab
        //src string: "  test  "
        //return string "test"
        static bool trim(const std::string&, std::string& ret);
        static std::string trim(const std::string&);

        //check whether a key is in a list of words separated by"," or not. like following
        //src line host001,host002,host003
        //key host001 is in the list
        //key 001,host is not in the list
        static bool in_list(const std::string& src, const std::string& key);

        //split a string with a separator and return a vector of strings
        static bool split(const std::string& str, std::vector<std::string>& ret, const std::string sep=",");
        

};

#endif
