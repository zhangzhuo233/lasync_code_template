#include "lathread.h"
#include "command.h"
#include "netutils.h"
#include <pthread.h>

#include <iostream>
using namespace std;


extern pthread_mutex_t g_mutex;
extern pthread_cond_t g_cond; 
extern LACommandExecuter* executer;

int LAThread::parse_msg(void* msg)
{
    LAMsg* pmsg = (LAMsg*)msg;
    pthread_mutex_lock(&g_mutex);
    executer->parse_command(*pmsg);
    pthread_cond_signal(&g_cond);
    pthread_mutex_unlock(&g_mutex);
}

void* LAThread::execute_commands(void* arg)
{
    while(true)
    {
        pthread_mutex_lock(&g_mutex);
        while(!executer->is_queue_empty())
        {
            cout<< "before execute" << endl;
            //cout<< "execute command" <<endl;
            LACommand* command = executer->get_first_command();
            command->execute();
            executer->pop_first_command();
            delete command;
            command = NULL;
            cout<< "after execute" << endl;
        }
        pthread_cond_wait(&g_cond, &g_mutex);
        pthread_mutex_unlock(&g_mutex);
    }
}
