#ifndef __LATHREAD_HEADER__
#define __LATHREAD_HEADER__


class LAThread
{
    public:
        static int parse_msg(void* msg );
        static void* execute_commands(void* arg); 
};

#endif
