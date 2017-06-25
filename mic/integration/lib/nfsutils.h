#ifndef __LANFSUTILS_HEADER___
#define __LANFSUTILS_HEADER___

#include <string>
using namespace std;

class LANfsUtils
{
    public:
        static bool is_nfs_exported(const std::string& nfsdir);

        static bool is_nfs_mounted(const std::string& nfsdir, std::string& mp);

        static bool config_nfs(const std::string nfsdir = "./syncdir",
                const std::string& config_file = "/etc/exports");

        static bool export_nfs(const std::string& nfsdir);

        static bool mount_nfs(const std::string& nfsdir, const std::string& dest);

        static bool umount_nfs(const std::string& dest);

        static void show_exported_nfs();
};

#endif
