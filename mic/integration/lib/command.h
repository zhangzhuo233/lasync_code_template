#ifndef __COMMAND_HEADER__
#define __COMMAND_HEADER__

#include <deque>
#include <string>
using namespace std;

class LACommand
{
    public:
        virtual int execute() = 0;
        virtual ~LACommand(){};
};

class LAFileSyncCommand: public LACommand
{
    public:
        static LAFileSyncCommand* create_instance(const std::string& nfsdir);
        LAFileSyncCommand(const std::string& nfsdir);
        virtual int execute();
        virtual ~LAFileSyncCommand();
    private:
        std::string nfsdir;
};

class LAPkgCommand: public LACommand
{
    public:
        static LAPkgCommand* create_instance();
        LAPkgCommand();
        virtual int execute();
        virtual ~LAPkgCommand();
    private:
        //verify whether it is a valid package list line.
        //valid add package line
        //+pkg1 pkg2 pkg3
        //valid remove package line
        //-pkg1 pkg2 pkg3
        bool verify_pkg_list(const std::string& line, std::string& ret, bool& flag);
        bool read_pkg_list();

        std::string install_list;
        std::string remove_list;
};

class LAUpdateCommand: public LACommand
{
    public:
        static LAUpdateCommand* create_instance();
        LAUpdateCommand();
        virtual int execute();
        virtual ~LAUpdateCommand();

};


class LAScriptCommand: public LACommand
{
    public:
        static LAScriptCommand* create_instance();
        virtual int execute();
        virtual ~LAScriptCommand();
};

struct LAMsg;
class LACommandExecuter
{
    public:
        static LACommandExecuter* create_instance();
        LACommandExecuter();
        ~LACommandExecuter();
    public:
        std::deque<LACommand*>* parse_command(const LAMsg& msg);
        LACommand* get_first_command();
        int execute_all();
        bool is_queue_empty();
        void pop_first_command();

    private:
        std::deque<LACommand*> command_queue;
};

#endif
