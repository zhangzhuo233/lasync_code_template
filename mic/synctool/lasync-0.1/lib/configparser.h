#ifndef __CONFIG_PARSER_HEADER__
#define __CONFIG_PARSER_HEADER__

#include <string>
#include <map>
#include <vector>
using namespace std;

class LAConfigParser
{
    public:
        LAConfigParser(char* path);
        LAConfigParser(std::string path);
        ~LAConfigParser();

    public:
        static LAConfigParser* create_instance(const char* conf="etc/lasync.conf");

    public:
        const char* read_plugin_dir();
        const char* read_value(char* key);
        static bool map_has_key(std::map<std::string, std::string>* map_ptr, std::string key);

    private:
        std::map<std::string, std::string>* read_all();
        void read_config();
        std::string path;
        std::map<std::string, std::string>* key_value_map;
        vector<std::string>* lines;
};

#endif
