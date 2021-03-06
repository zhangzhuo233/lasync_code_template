#include "strutils.h"
#include <string>
#include <algorithm>
#include <iterator>
#include <regex.h>
#include <vector>
#include <iostream>
using namespace std;

bool LAStrUtils::startswith(const char* str, const char* start)
{
    if(NULL == str && NULL == start)
    {
        return true;
    }

    if(NULL == str && NULL != start)
    {
        return false;
    }

    std::string src(str);
    std::string dst(start);

    if(0 == src.length() && 0 == dst.length())
    {
        return true;
    }

    if(0 == src.length() && 0 != dst.length())
    {
        return false;
    }

    if(src.length() < dst.length())
    {
        return false;
    }

    std::string substr = src.substr(0, dst.length());

    return (substr == dst);
}

bool LAStrUtils::endswith(const char* str, const char* end)
{
    std::string src(str);
    std::string dst(end);

    std::reverse(src.begin(), src.end());
    std::reverse(dst.begin(), dst.end());

    return LAStrUtils::startswith(src.c_str(), dst.c_str());
}

std::string LAStrUtils::trim(const std::string& line)
{
    if (line.empty())
    {
        return line;
    }

    char* delimiters[] = {" ", "\t"};
    std::string ret;
    ret.assign(line.begin(), line.end());
    
    for(int i = 0; i < (sizeof(delimiters)/sizeof(char*)) ; i++) 
    {
        int index = 0;
        index = ret.find_first_not_of(delimiters[i]);
        if(index > 0) 
        {
            ret.erase(0,index);  
        }
        index = 0;

        index = ret.find_last_not_of(delimiters[i]);
        if(index > 0)
        {
            ret.erase(index + 1);  
        }
    }
    
    return ret;
}

bool LAStrUtils::trim(const std::string& line, std::string& ret)
{
    ret.assign(line.begin(), line.end());
    if (ret.empty())   
    {  
        return true;  
    }  
    
    char* delimiters[] = {" ", "\t"};
    
    for(int i = 0; i < (sizeof(delimiters)/sizeof(char*)) ; i++) 
    {
        int index = 0;
        index = ret.find_first_not_of(delimiters[i]);
        if(index > 0) 
        {
            ret.erase(0,index);  
        }
        index = 0;

        index = ret.find_last_not_of(delimiters[i]);
        if(index > 0)
        {
            ret.erase(index + 1);  
        }
    }
    
    return true;
}

//check whether a key is in a list of words separated by"," or not. like following
//src line host001,host002,host003
//key host001 is in the list
//key 001,host is not in the list
bool LAStrUtils::in_list(const std::string& src, const std::string& key)
{
    std::vector<std::string> str_vector;
    bool ret = LAStrUtils::split(src, str_vector, ",");
    if(str_vector.empty())
    {
        return false;
    }

    std::vector<std::string>::iterator it = find(str_vector.begin(), str_vector.end(), key);
    return it != str_vector.end();
}

//split a string with a separator and return a vector of strings
bool LAStrUtils::split(const std::string& str, std::vector<std::string>& ret, 
        const std::string sep)
{
    if(str.empty())
    {
        return true;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret.push_back(tmp);
            tmp.clear();
        }
    }
    return true;
}

