// unix/linux headers
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>

// standard c/c++ headers
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>

// our headers
#include "netutils.h"
#include "strutils.h"
#include "constants.h"

int LANetUtils::create_daemon_socket()
{
    int sockfd;
    struct sockaddr_in myaddr;

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT);
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

int LANetUtils::extract_msg(const void* msg, std::string& src_ip, 
        std::string& dest, int& cmd, std::string& syncdir)
{

    LAMsg* pmsg = (LAMsg*)msg;
    cmd = pmsg->cmd;
    struct in_addr src = pmsg->src;

    src_ip.assign(inet_ntoa(src));

    int host_list_len = pmsg->host_list_len;

    char* pstart = (char*)(pmsg->tag);

    char* phost_list = (char*)malloc(host_list_len + 1);
    memcpy(phost_list, pstart, host_list_len);

    phost_list[host_list_len] = '\0';
    dest.assign(phost_list, host_list_len);
    
    int syncdir_len = strlen(pstart) - host_list_len;
    char* psyncdir = (char*)malloc(syncdir_len);
    memcpy(psyncdir, pstart + host_list_len, syncdir_len);
    syncdir.assign(psyncdir, syncdir_len);
    free(psyncdir);

    return 0;
}

void* LANetUtils::build_msg(const std::string& src_ip, const std::string& dest, 
        const int cmd, const std::string& syncdir)
{
    int buffer_len = sizeof(LAMsg) + dest.length() + syncdir.length() + 1; 

    LAMsg* buffer = (LAMsg*)malloc(buffer_len);

    buffer->self_len = buffer_len;
    buffer->cmd = cmd;

    struct in_addr src;
    inet_aton(src_ip.c_str(), &src);
    buffer->src = src;

    //rember len of dest
    buffer->host_list_len = dest.length();

    //get the tail of the struct
    char* start = buffer->tag;

    //copy dest str to the end of the struct 
    strcpy(start, dest.c_str()); 

    //copy syncdir to the end of buffer
    strcpy(start + dest.length(), syncdir.c_str());

    return buffer;
}

int LANetUtils::get_nic_info(const char* nic_name, std::string& ip, std::string& broadcast)
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        return -1;
    }

    struct ifreq ifr;
    unsigned char mac[6];
    unsigned long ipaddr, netmask, broad_addr;

    //comment out mac address
    //printf("%s:\n", nic_name);
    //strcpy(ifr.ifr_name, nic_name);
    //if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0)
    //{
    //    return -1;
    //}

    //memcpy(mac, ifr.ifr_hwaddr.sa_data, sizeof(mac));
    //printf("\tMAC: %02x-%02x-%02x-%02x-%02x-%02x\n",
    //        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    strcpy(ifr.ifr_name, nic_name);
    if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
    {
        ipaddr = 0;
    }
    else
    {
        ipaddr = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
    }
    //printf("\tIP: %s\n", inet_ntoa(*(in_addr*)&ipaddr));
    ip = inet_ntoa(*(in_addr*)&ipaddr);

    strcpy(ifr.ifr_name, nic_name);
    if (ioctl(s, SIOCGIFBRDADDR, &ifr) < 0)
    {
        broad_addr = 0;
    }
    else
    {
        broad_addr = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
    }
    //printf("\tBroadIP: %s\n", inet_ntoa(*(in_addr*)&broad_addr));
    broadcast = inet_ntoa(*(in_addr*)&broad_addr);

    //strcpy(ifr.ifr_name, nic_name);
    //if (ioctl(s, SIOCGIFNETMASK, &ifr) < 0)
    //{
    //    netmask = 0;
    //}
    //else
    //{
    //    netmask = *(unsigned long*)&ifr.ifr_netmask.sa_data[2];
    //}
    //printf("\tNetmask: %s\n", inet_ntoa(*(in_addr*)&netmask));

    close(s);
    return 0;
}

int LANetUtils::send_msg(void* msg, const std::string& deststr, 
        const std::string& nic)
{
    int sockfd;
    struct sockaddr_in dest;
    struct hostent* host_entry;

    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    dest.sin_addr.s_addr = inet_addr(deststr.c_str());
    memset(&dest.sin_zero, 0, sizeof(dest.sin_zero));

    if( -1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) )
    {
        perror("create socket");
        exit(1);
    }

    //if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE,
    //            nic.c_str(), (nic.length() + 1)) < 0) 
    //{
    //    perror("setsockopt");
    //    exit(1);
    //}
    
    int so_broadcast = 1;

    if( -1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast)))
    {
        perror("set socket opt");
        exit(1);
    }

    //if((bind(sockfd,(struct sockaddr *)&dest,
    //                sizeof(struct sockaddr)))==-1) 
    //{
    //    perror("bind");
    //    exit(1);
    //}
    
    LAMsg* pmsg = (LAMsg*)msg;
    if( -1 == sendto(sockfd, msg, pmsg->self_len , 0, (struct sockaddr*)(&dest), sizeof(dest)) )
    {
        perror("send msg");
        exit(1);
    }

    return 0;
}

int LANetUtils::get_local_hostname(std::string& hostname)
{
    char buffer[128];
    int ret = gethostname(buffer, sizeof(buffer));
    if(ret)
    {
        return ret;
    }
    hostname.assign(buffer);
    return 0;
}


int LANetUtils::get_ip_by_hostname(const std::string& hostname, std::string &ip)
{     
    std::string trimedname = LAStrUtils::trim(hostname);
    if(hostname.empty() || hostname == ""){
        return -1;
    }

    struct hostent* hptr;
    hptr = gethostbyname(hostname.c_str());
    if(hptr == NULL)
    {
        return -1;
    }
    
    printf("official hostname:%s\n",hptr->h_name); 

    char **pptr;
    char str[32];

    for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
        printf(" alias:%s\n",*pptr);

    //print all address according to the address type
    switch(hptr->h_addrtype)
    {
        case AF_INET:
        case AF_INET6:
            pptr=hptr->h_addr_list;

            //print all the address, the inet_ntop is called
            for(;*pptr!=NULL;pptr++)
                printf(" address:%s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            break;
        default:
            printf("unknown address type\n");
            break;
    }
    return 0;
}
