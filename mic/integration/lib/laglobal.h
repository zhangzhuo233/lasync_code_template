#ifndef __CMD_UTILS_HEADER__
#define __CMD_UTILS_HEADER__
#include "command.h"
#include <pthread.h>

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;
LACommandExecuter* executer;

#endif
