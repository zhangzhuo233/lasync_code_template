#ifndef __CONFIG_PARSER_HEADER__
#define __CONFIG_PARSER_HEADER__

#include <string>
#include <map>
#include <vector>
using namespace std;

class LAConfigParser
{
    public:
        ~LAConfigParser();

    public:
        static LAConfigParser* create_instance(const char* conf="etc/lasync.conf", char* delimiter="=",bool allow_duplicate_keys=true);

    public:
        const char* read_plugin_dir();
        const char* read_value(char* key);
        const std::string read_value(const std::string& key);
        static bool map_has_key(std::map<std::string, std::string>* map_ptr, std::string key);
        void dump();

    private:
        std::string path;
        std::string delimiter;
        bool allow_duplicate_keys;

        LAConfigParser(char* path, char* delimiter="=", bool allow_duplicate_keys=true);
        LAConfigParser(std::string path, std::string delimiter="=", bool allow_duplicate_keys=true);

        std::map<std::string, std::string>* read_all();
        std::map<std::string, std::string>* key_value_map;
        vector<std::string>* lines;
        void read_config();
};

#endif
