#include "scriptexecutor.h"
#include "dirreader.h"
#include "configparser.h"
#include "fsutils.h"
#include "strutils.h"

#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <cstdlib>
using namespace std;

LAScriptExecutor* LAScriptExecutor::create_instance(std::string conf)
{
    return (new LAScriptExecutor(conf));
}

LAScriptExecutor::LAScriptExecutor(char* confpath):scripts(NULL)
{
    this->confpath = std::string(confpath);
    this->init();
}

LAScriptExecutor::LAScriptExecutor(std::string confpath):scripts(NULL)
{
    this->confpath = confpath;
    this->init();
}

LAScriptExecutor::~LAScriptExecutor()
{
    if(this->scripts)
    {
        delete this->scripts;
        this->scripts = NULL;
    }
}

void LAScriptExecutor::init()
{
    LAConfigParser* parser = LAConfigParser::create_instance(this->confpath.c_str());
    
    std::string plugindir;
    try
    {
        plugindir = parser->read_plugin_dir();
    }
    catch(LAKeyNotFoundException &e)
    {
        cerr<<e.info<<endl;
        exit(1);
    }
    this->plugindir = plugindir;
    
    vector<std::string>* pvector = LADirReader::create_instance(plugindir)->read_dir();

    this->scripts = new vector<std::string>(*pvector);
    pvector = NULL;
    
    this->filter_scripts();
    copy(this->scripts->begin(), this->scripts->end(), 
            ostream_iterator<std::string>(cout,"\n"));

}

void LAScriptExecutor::filter_scripts()
{
    vector<std::string>::iterator it;
    std::string filename; 
    for(it = scripts->begin(); it != scripts->end(); )
    {
        const char* pfilename = it->c_str();
        if(LAStrUtils::startswith(pfilename, ".") 
                || ((!this->is_python_script(pfilename))
                && (!this->is_shell_script(pfilename)))
                )
        {
            it = scripts->erase(it);
        }
        else
        {
            ++it;
        }
    }

    vector<std::string> tmp(*this->scripts);
    tmp.swap(*(this->scripts));

    sort(scripts->begin(), scripts->end());
}

inline bool LAScriptExecutor::is_python_script(const std::string& filename)
{
    return LAStrUtils::endswith(filename.c_str(), ".py");
}

inline bool LAScriptExecutor::is_shell_script(const std::string& filename)
{
    return LAStrUtils::endswith(filename.c_str(), ".sh"); 
}

int LAScriptExecutor::execute_one(const std::string& scriptname)
{
    vector<std::string>::iterator it = find(this->scripts->begin(), 
                            this->scripts->end(), scriptname);
    if(it == this->scripts->end())
    {
        throw(LAFileNotFoundException(scriptname));
    }

    int ret = 0;
    std::string logstr(" >> log/plugin.log 2>&1");
    if(is_python_script(scriptname))
    {
        std::string cmd("python " + this->plugindir + "/" + scriptname + logstr);
        //cout<< cmd <<endl;
        ret = system(cmd.c_str());
    }
    else if(is_shell_script(scriptname))
    {
        std::string cmd("bash " + this->plugindir + "/" + scriptname + logstr);
        //cout<< cmd <<endl;
        ret = system(cmd.c_str());
    }
    else
    {
        //here can not be reached!!!
    }
    return ret;
}

int LAScriptExecutor::execute_all()
{
    vector<std::string>::iterator it = this->scripts->begin();
    int ret = 0;
    for( ; it != this->scripts->end(); ++it)
    {
        if(this->execute_one(*it))
        {
            ret = 1;
        }
    }
    return ret;
}
