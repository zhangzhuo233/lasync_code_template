//system headers
#include <errno.h> //errno
#include <dirent.h> //DIR, opendir and readdir
#include <unistd.h> //access 

//c++ headers
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>

#include "dirreader.h"
#include "exception.h"
#include "limits.h"

LADirReader::LADirReader(char* path)
{
    this->path = std::string(path);
    this->dir_entries = new vector<std::string>();
}

LADirReader::LADirReader(std::string path_str):path(path_str)
{
    this->dir_entries = new vector<std::string>();
}

LADirReader::~LADirReader()
{
    delete dir_entries;
    dir_entries = NULL;
}

vector<std::string>* LADirReader::read_dir() 
    throw(LADirectoryOpenException, LADirectoryCanNotReadException)
{

    char buffer[1024] ;
    char* path = NULL;
    path = realpath(this->path.c_str(), buffer);

    if(access(path, F_OK | R_OK))
    {
        throw (LADirectoryCanNotReadException(path));
    }

    DIR* dir;
    struct dirent* ent;
    if(!(dir = opendir(this->path.c_str())))
    {
        throw (LADirectoryOpenException(path));
    }

    errno = 0;
    while((ent = readdir(dir)))
    {
        this->dir_entries->push_back(std::string(ent->d_name));
        errno = 0;
    }
    
    closedir(dir);    
    return this->dir_entries;
}
