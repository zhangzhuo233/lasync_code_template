//c++ headers
#ifndef __DIR_READER_HEADER__
#define __DIR_READER_HEADER__
#include <vector>
#include <string>
#include "exception.h"

using namespace std;

class LADirReader
{
public:
    LADirReader(char* path);
    LADirReader(std::string path_str);
    ~LADirReader();

public:
    static inline LADirReader* create_instance(char* path)
    {
        return new LADirReader(path);
    };

    static inline LADirReader* create_instance(std::string path)
    {
        return new LADirReader(path);
    };

    vector<std::string>* read_dir()
        throw(LADirectoryOpenException, LADirectoryCanNotReadException);

private:
    std::string path;
    std::vector<std::string>* dir_entries;
};
#endif
