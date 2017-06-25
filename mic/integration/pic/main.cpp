// linux/unix standard libraries
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <stdarg.h>

// c/c++ standard libraries
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <deque>
#include <string.h>

// user libs
#include "logutils.h"

using namespace std;

#define MAX_PROC_ENT 2048
#define MAXFILENAMELEN 4096

enum LA_ProcessState
{
    LA_PSTAT_RUNNING,
    LA_PSTAT_INTERRUPTIBLE,
    LA_PSTAT_UNINTERRUPTIBLE,
    LA_PSTAT_ZOMBIE,
    LA_PSTAT_STOPPED,
    LA_PSTAT_SWAPPED,
    LA_PSTAT_SLEEP,
    LA_PSTAT_EXITING
};

enum LA_LogLevel
{
    LA_LLEVEL_DEBUG,
    LA_LLEVEL_INFO,
    LA_LLEVEL_WARN,
    LA_LLEVEL_ERROR
};

struct LA_ProcessInfo
{
    int pid;
    int ppid;
    int pgid;
    int jobid;
    int utime;
    int stime;
    int cutime;
    int cstime;
    int proc_size;
    int resident_size;
    int stack_size;
    enum LA_ProcessState state;
    char command[PATH_MAX];
};

static int parse_proc_stat(char *buf, struct LA_ProcessInfo *pinfo);
static int get_proc_info(int pid, struct LA_ProcessInfo  *rec);
static void log_proc_info(void);
static void la_syslog(string str);

//static vars
static struct LA_ProcessInfo procs[MAX_PROC_ENT];
static int numprocs;

//global vars
char infofile[PATH_MAX];
int pimPort;

//la_syslog
void la_syslog(LA_LogLevel loglevel, const char* format, ...)
{
    LALogUtils* logger = LALogUtils::get_instance_ptr();

    static char buff[4096];

    va_list vl;
    va_start(vl, format);
    vsprintf(buff,format, vl);
    va_end(vl);

    switch(loglevel)
    {
        case LA_LLEVEL_DEBUG:
            logger->debug_log(format, buff);
            break;
        case LA_LLEVEL_INFO:
            logger->info_log(format, buff);
            break;
        case LA_LLEVEL_WARN:
            logger->error_log(format, buff); 
            break;
        case LA_LLEVEL_ERROR:
            logger->error_log(format, buff);
            break;
        default:
            logger->error_log(format, buff);
            break;
    }
}

//scan_procs
int scan_procs(void)
{
    DIR *dir;
    struct dirent *process;
    struct LA_ProcessInfo rec;

    dir = opendir("/proc");
    if (dir == NULL) {
        la_syslog(LA_LLEVEL_ERROR, "\%s: opendir(/proc) failed: %m.", __func__);
        return -1;
    }

    numprocs = 0;
    while ((process = readdir(dir))) {

        if (! isdigit(process->d_name[0]))
            continue;

        if (get_proc_info(atoi(process->d_name), &rec) != 0)
            continue;

        if (rec.pgid == 1)
            continue;

        procs[numprocs].pid = rec.pid;
        procs[numprocs].ppid = rec.ppid;
        procs[numprocs].pgid = rec.pgid;

        procs[numprocs].utime = rec.utime/100;
        procs[numprocs].stime = rec.stime/100;
        procs[numprocs].cutime = rec.cutime/100;
        procs[numprocs].cstime = rec.cstime/100;

        procs[numprocs].proc_size = rec.proc_size;
        procs[numprocs].resident_size
            = rec.resident_size * (sysconf(_SC_PAGESIZE)/1024);
        procs[numprocs].stack_size = rec.stack_size;
        if ( procs[numprocs].stack_size < 0 )
            procs[numprocs].stack_size = 0;
        procs[numprocs].state = rec.state;

        ++numprocs;
        if (numprocs == MAX_PROC_ENT - 1) {
            la_syslog(LA_LLEVEL_ERROR, "\%s: maximum number of processes %d reached.", __func__, numprocs);
            break;
        }
    }

    closedir(dir);

    log_proc_info();

    return 0;
}
//end scan_procs

//log_proc_info
static void log_proc_info(void)
{
    int i;
    FILE *fp;
    static char wfile[MAXFILENAMELEN];

    sprintf(wfile, "%s.%d", infofile, getpid());

    fp = fopen(wfile, "w");
    if (fp == NULL) {
        la_syslog(LA_LLEVEL_ERROR, "%s: fopen() %s failed %m.", wfile);
        return;
    }

    fprintf(fp, "%d\n", pimPort);
    for (i = 0; i < numprocs; i++) {

        fprintf(fp, "\
%d %d %d %d %d %d %d %d %d %d %d %d\n",
                procs[i].pid, procs[i].ppid, procs[i].pgid, procs[i].jobid,
                procs[i].utime, procs[i].stime, procs[i].cutime,
                procs[i].cstime, procs[i].proc_size,
                procs[i].resident_size, procs[i].stack_size,
                (int) procs[i].state);
    }
    fclose(fp);

    if (unlink(infofile) < 0 && errno != ENOENT) {
        la_syslog(LA_LLEVEL_ERROR, "\%s: unlink(%s) failed: %m.", __func__, infofile);
    }

    if (rename(wfile, infofile) < 0) {
        la_syslog(LA_LLEVEL_ERROR, "\%s: rename() %s to %s failed: %m.", __func__, wfile, infofile);
    }
    unlink(wfile);

    la_syslog(LA_LLEVEL_ERROR, "\%s: process table updated.", __func__);

}
//end log_proc_info

//get_proc_info
static int get_proc_info(int pid, struct LA_ProcessInfo  *rec)
{
    int fd;
    static char filename[PATH_MAX];
    static char buffer[BUFSIZ];

    sprintf(filename, "/proc/%d/stat", pid);

    fd = open(filename, O_RDONLY, 0);
    if (fd == -1) {
        la_syslog(LA_LLEVEL_ERROR, "\%s: open() failed %s %m.", __func__, filename );
        return -1;
    }

    if (read(fd, buffer, sizeof(buffer) - 1) <= 0) {
        la_syslog(LA_LLEVEL_ERROR, "\%s: read() failed %s %m.", __func__, filename);
        close(fd);
        return -1;
    }
    close(fd);

    if (parse_proc_stat(buffer, rec) < 0) {
        la_syslog(LA_LLEVEL_ERROR, "\%s: parse_proc_stat() failed process %d.", __func__, pid);
        return -1;
    }

    return 0;
}
//end get_proc_info

//parse_proc_stat
static int parse_proc_stat(char *buf, struct LA_ProcessInfo *pinfo)
{
    unsigned int rss_rlim;
    unsigned int start_code;
    unsigned int end_code;
    unsigned int start_stack;
    unsigned int end_stack;
    unsigned char state;
    unsigned long vsize;

    sscanf(buf, "\
%d %s %c %d %d %*d %*d %*d %*u %*u %*u %*u %*u %d %d %d "
           "%d %*d %*d %*u %*u %*d %lu %u %u %u %u %u %u",
           &pinfo->pid, pinfo->command, &state, &pinfo->ppid, &pinfo->pgid,
           &pinfo->utime, &pinfo->stime, &pinfo->cutime, &pinfo->cstime,
           &vsize, &pinfo->resident_size, &rss_rlim, &start_code,
           &end_code, &start_stack, &end_stack);

    if (pinfo->pid == 0) {
        la_syslog(LA_LLEVEL_ERROR, "\%s: invalid process 0 found: %s", __func__, buf);
        return -1;
    }

    pinfo->stack_size = start_stack - end_stack;
    pinfo->proc_size = vsize/1024;

    switch (state) {
        case 'R' :
            pinfo->state = LA_PSTAT_RUNNING;
            break;
        case 'S' :
            pinfo->state = LA_PSTAT_SLEEP;
            break;
        case 'D' :
            pinfo->state = LA_PSTAT_SLEEP;
            break;
        case 'T' :
            pinfo->state = LA_PSTAT_STOPPED;
            break;
        case 'Z' :
            pinfo->state = LA_PSTAT_ZOMBIE;
            break;
        case 'W' :
            pinfo->state = LA_PSTAT_SWAPPED;
            break;
        default :
            pinfo->state = LA_PSTAT_RUNNING;
            break;
    }

    return 0;
}
//end parse_proc_stat

int main() 
{
    //init logger
    sprintf(infofile, "/tmp/pim.info.mine");
    while(1) {
        scan_procs();
        sleep(1);
    }
    return 0;
}
