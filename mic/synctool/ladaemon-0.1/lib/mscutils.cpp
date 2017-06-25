#include "mscutils.h"
#include "constants.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/wait.h>

#include <cstdlib>
#include <cstdio>

void LAMscUtils::init_daemon()
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
    for(i=0;i< NOFILE; ++i)
    {
        close(i); 
    }

    //change cwd to /tmp
    //chdir("/tmp");

    //set umask to 0
    umask(0);
    return; 
}

