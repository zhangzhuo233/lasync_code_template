#include "strutils.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

int main()
{
    char* pa = "this is a cap";
    char* pb = "this";
    char* pc = "thi";
    char* pd = "is";

    char* pe = "cap";
    char* pf = "ap";
    char* pg = "asdf";

    cout<< "test start with"<<endl;
    cout<< LAStrUtils::startswith(pa, pb) << endl;;
    cout<< LAStrUtils::startswith(pa, pc) << endl;;
    cout<< LAStrUtils::startswith(pa, pd) << endl;;

    cout<< "test endswith"<<endl;
    cout<< LAStrUtils::endswith(pa, pe) << endl;
    cout<< LAStrUtils::endswith(pa, pf) << endl;
    cout<< LAStrUtils::endswith(pa, pg) << endl;


    std::string str("host001,host003,host004");
    std::vector<std::string> str_vector;
    bool ret = LAStrUtils::split(str, str_vector, ",");
    copy(str_vector.begin(), str_vector.end(), ostream_iterator<std::string>(cout, " "));
    cout<< endl;

    std::string key("host003");
    ret = LAStrUtils::in_list(str, key);
    cout<< "in_list:" << ret << endl;

    std::string line("    +asdfasdfas  asdf assf        ");
    cout<< "before:" << line << "len:" << line.size() << endl;
    ret = LAStrUtils::trim(line, str);
    cout<< "trim  :" << ret << endl;
    cout<< "after :" << str << "len:" << str.size() << endl;

    line = "    -asdfasdfas  asdf assf";
    cout<< "before:" << line << endl;
    ret = LAStrUtils::trim(line, str);
    cout<< "trim  :" << ret << endl;
    cout<< "after :" << str << endl;

    line = "    asdfasdfas  asdf assf";
    cout<< "before:" << line << endl;
    ret = LAStrUtils::trim(line, str);
    cout<< "trim  :" << ret << endl;
    cout<< "after :" << str << endl;

    return 0;
}
