#ifndef __NET_UTILS_HEADER__
#define __NET_UTILS_HEADER__

#include <string>
using namespace std;
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct LAMsg
{
    int self_len;
    struct in_addr src;
    int cmd;
    int host_list_len;
    char tag[0];
};

class LANetUtils
{
    public:
        static int create_daemon_socket();
        static int send_msg(void* msg, const std::string& deststr, 
                const std::string& nic);
        static int get_nic_info(const char* nic_name, std::string& ip, std::string& broadcast);
        static void* build_msg(const std::string& src_ip, const std::string& dest, 
                const int cmd, const std::string& syncdir);
        static int extract_msg(const void* msg, std::string& src,  
                std::string& dest, int& cmd, std::string& syncdir);
        static int get_local_hostname(std::string& hostname);
};

#endif
