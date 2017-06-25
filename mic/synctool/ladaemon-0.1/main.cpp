// linux/unix standard libraries
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

//----------
// user libraries

//----------
// c/c++ standard libraries
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <deque>
#include <string.h>

// our headers
#include "lib/constants.h"
#include "lib/netutils.h"
#include "lib/mscutils.h"
#include "lib/command.h"
#include "lib/lathread.h"
#include "lib/logutils.h"

using namespace std;

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

LACommandExecuter* executer = LACommandExecuter::create_instance();

void cleanup()
{
    if(executer)
    {
        delete executer;
        executer = NULL;
    }
}

int main(int argc, char** argv)
{
    if(getuid())
    {
        cerr << "Permission denied, only root can run this tool" << endl;
        exit(1);
    }
    int optarg = 0;
    char* argstr = NULL;
    bool daemon_flag = false;
    int ret = atexit(cleanup);
    pthread_t tid = 0;
    if(ret)
    {
        perror("atexit:");
        exit(1);
    }
   

    //parse args
    while((optarg = getopt(argc, argv, "d")) != -1)
    {
        switch(optarg)
        {
            case 'd':
                daemon_flag = true;
                break;

            case '?':
                cerr << "Argument error" <<endl;
                break;
        }
    }

    //run as background or foreground
    if(daemon_flag)
    {
        LAMscUtils::init_daemon();
    }
    
    int sockfd = LANetUtils::create_daemon_socket();
    LALogUtils* logger = LALogUtils::get_instance_ptr();

    ret = pthread_create(&tid, NULL, LAThread::execute_commands, NULL);
    if(ret)
    {
        perror("create thread:");
        exit(1);
    }

    while(true)
    {
        struct sockaddr_in hisaddr;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        socklen_t socklen = sizeof(hisaddr);

        int data_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, 
                (sockaddr*)&hisaddr, &socklen);
        
        void* msg = malloc(data_len + 1);
        memcpy(msg, buffer, data_len);

        int cmd = 0;
        std::string src;
        std::string dest;
        std::string syncdir;
        int ret = LANetUtils::extract_msg(msg, src, dest, cmd, syncdir);
        cout<< "src :" << src <<endl;
        cout<< "dest:" << dest <<endl;
        cout<< "cmd :" << cmd <<endl;
        cout<< "syncdir:" << syncdir <<endl;
        logger->info_log("dest: %s, cmd: %d, syncdir: %s",
                dest.c_str(), cmd, syncdir.c_str());

        LAThread::parse_msg(msg);
    
        free(msg);
    }

    LALogUtils::remove_instance();
    return 0;
}
