#include "configparser.h"
#include "exception.h"
#include <iostream>
using namespace std;

int main()
{
    cout<<"config parser with param"<<endl;
    LAConfigParser* parser = LAConfigParser::create_instance("etc/ladaemon.conf");
    delete parser;
    parser = NULL;


    cout<<"config parser with default param"<<endl;
    parser = LAConfigParser::create_instance("etc/ladaemon.conf");

    cout<<parser->read_plugin_dir()<<endl;
    cout<<parser->read_value("loglevel")<<endl;
    cout<<parser->read_value("plugindir")<<endl;


    try
    {
        std::string dummy_key = parser->read_value("none exists key"); 
        cout<< dummy_key <<endl;
    }
    catch (LAKeyNotFoundException &e)
    {
        cerr << e.info <<endl;
    }

    delete parser;
    parser = NULL;


    return 0;
}
