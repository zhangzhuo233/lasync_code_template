#include "configparser.h"
#include "fsutils.h"
#include "strutils.h"
#include "exception.h"

#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <regex.h>
#include <string.h>

LAConfigParser::LAConfigParser(char* path)
{
    this->path = std::string(path);
    this->key_value_map = new map<std::string, std::string>;
    this->lines = new vector<std::string>;
    this->read_config();
    this->read_all();
}

LAConfigParser::LAConfigParser(std::string path)
{
    this->path = path;
    this->key_value_map = new map<std::string, std::string>;
    this->lines = new vector<std::string>;
    this->read_config();
    this->read_all();
}

LAConfigParser::~LAConfigParser()
{
    if(this->key_value_map)
    {
        delete this->key_value_map;
        this->key_value_map = NULL;
    }

    if(this->lines)
    {
        delete this->lines;
        this->lines = NULL;
    }
}

LAConfigParser* LAConfigParser::create_instance(const char* conf)
{
    if(!LAFsUtils::exists(conf) || !LAFsUtils::isfile(conf))
    {
        throw(LAFileNotFoundException(conf));
    }

    return (new LAConfigParser(conf));
}

const char* LAConfigParser::read_plugin_dir()
{
    return this->read_value("plugindir");
}

const char* LAConfigParser::read_value(char* key)
{
    if(!map_has_key(this->key_value_map, key))
    {
        throw(LAKeyNotFoundException(key));
    }

    return this->key_value_map->find(key)->second.c_str();
}

void LAConfigParser::read_config()
{
    ifstream conf(this->path.c_str());
    std::string line;
    while(!conf.eof())
    {
        std::getline(conf, line);
        if(line.length() == 0 )
        {
            continue;
        }
        lines->push_back(line);
    }
}

map<std::string, std::string>* LAConfigParser::read_all()
{

    char* blank_line_pattern = "^[ |\t|\n]$";
    regex_t blank_line_regex;
    if(regcomp(&blank_line_regex, blank_line_pattern, REG_EXTENDED | REG_NEWLINE | REG_NOSUB))
    {
        regfree(&blank_line_regex);
    }

    char* conf_line_pattern = "[ |\t]*([a-zA-Z_]+)[ |\t]*=[ |\t]*'([a-z|A-Z|0-9|/|\\.|_|,|-]?+)'.*";
    regex_t conf_line_regex;
    regmatch_t match[3];
    if(regcomp(&conf_line_regex, conf_line_pattern, REG_EXTENDED))
    {
        regfree(&conf_line_regex);
    }

    vector<std::string>::iterator cur;
    std::string line;

    for(cur = this->lines->begin(); cur != this->lines->end(); ++cur)
    {
        line = *cur;

        if(LAStrUtils::startswith(line.c_str(), "#"))
        {
            continue;
        }


        if(0 == regexec(&blank_line_regex, line.c_str(), 0, NULL, 0))
        {
            continue;
        }


        if(0 == regexec(&conf_line_regex, line.c_str(), 3, match, 0))
        {
            std::string key(line.c_str() + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            std::string value(line.c_str() + match[2].rm_so, match[2].rm_eo - match[2].rm_so);

            std::map<std::string, std::string>::iterator it;

            if(map_has_key(this->key_value_map, key))
            {
                throw(LADuplicateKeyException(key));
            }
            this->key_value_map->insert(pair<std::string, std::string>(key, value));
        }
    }

    regfree(&blank_line_regex);
    regfree(&conf_line_regex);
    return this->key_value_map;
}

bool LAConfigParser::map_has_key(std::map<std::string, std::string>* map_ptr, std::string key)
{
    std::map<std::string, std::string>::iterator it;
    it = map_ptr->find(key);
    return (!(it == map_ptr->end()));
}

