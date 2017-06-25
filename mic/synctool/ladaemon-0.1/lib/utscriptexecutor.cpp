#include "scriptexecutor.h"
#include <iostream>

int main()
{
    cout<< "script executor ut" <<endl;
    LAScriptExecutor* executor = LAScriptExecutor::create_instance();
    
    cout<<endl<<"execute one:"<<endl;
    cout<<"result:"<< executor->execute_one("01.sh") <<endl;;
    cout<<"result:"<< executor->execute_one("02.py") <<endl;

    cout<<endl<<"execute all:"<<endl;
    cout<<"result:"<< executor->execute_all() <<endl;
    
    delete executor;
    executor = NULL;

    return 0;
}
