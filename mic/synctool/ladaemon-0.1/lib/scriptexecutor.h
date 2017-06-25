#ifndef __SCRIPT_EXECUTOR_HEADER__
#define __SCRIPT_EXECUTOR_HEADER__

#include <string>
#include <vector>
using namespace std;

class LAScriptExecutor
{
    public:
        LAScriptExecutor(char* path);
        LAScriptExecutor(std::string path);
        ~LAScriptExecutor();

    public:
        static LAScriptExecutor* create_instance(std::string conf="etc/ladaemon.conf");
        int execute_one(const std::string& scriptname);
        int execute_all();

    private:
        void init();
        void filter_scripts();
        bool inline is_python_script(const std::string& filename);
        bool inline is_shell_script(const std::string& filename);
        std::string plugindir;
        std::string confpath;
        std::vector<std::string>* scripts;
};

#endif
