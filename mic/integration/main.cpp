// linux/unix standard libraries
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

//----------
// user libraries

//----------
// c/c++ standard libraries
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <deque>
#include <string.h>

//----------
// constants
const int PORT = 20000;
const int BACKLOG = 10;

using namespace std;

void init_daemon()
{
    int pid; 
    int i; 
    if(pid=fork()) 
    {
        //terminate parent process
        exit(0);
    }
    else if(pid< 0) 
    {
        //fork error, exist
        exit(1);
    }

    //child process, go on
    //make child process as session leader and process group leader
    setsid();
    
    //separate from terminal
    if(pid=fork()) 
    {
        exit(0);
    }
    else if(pid< 0) 
    {
        exit(1);
    }
    
    //the process is the second process.
    //and the process is no more session leader and process group leader
    //close all opened file descriptors
    for(i=0;i< NOFILE;++i)
    {
        close(i); 
    }

    //change cwd to /tmp
    chdir("/tmp");

    //set umask to 0
    umask(0);
    return; 
}

int create_tcp_socket(int* psockfd)
{
    int sockfd;
    struct sockaddr_in myaddr;

}

int create_udp_socket()
{
    int sockfd;
    struct sockaddr_in myaddr;

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = PORT;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    memset(&myaddr.sin_zero, 0, sizeof(myaddr.sin_zero));
    
    if( -1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        perror("create udp socket failed");
        exit(-1);
    }

    if( -1 == bind(sockfd, (struct sockaddr*)&myaddr, sizeof(struct sockaddr)))
    {
        perror("bind udp socket failed");
        exit(-1);
    }
    
    return sockfd;
}

int main(int argc, char** argv)
{
    int optarg = 0;
    char* argstr = NULL;
    bool daemon_flag = false;
    
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
        init_daemon();
    }
    
    int sockfd = create_udp_socket();
    while(true)
    {
        //logic
        struct sockaddr_in hisaddr;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        socklen_t socklen = sizeof(hisaddr);

        int data_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, 
                (sockaddr*)&hisaddr, &socklen);
        
        buffer[data_len] = '\0';
        char cmd[1024];
        sprintf(cmd, "echo %s >> log.txt", buffer);
        system("echo >> log.txt");
        sleep(5);
    }
    return 0;
}

