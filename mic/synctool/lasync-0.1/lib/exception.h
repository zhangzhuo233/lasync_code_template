#ifndef __EXCEPTION_HEADER__
#define __EXCEPTION_HEADER__
#include <string>
using namespace std;

class LAException
{
    public:
        virtual ~LAException(){};
};

class LAKeyNotFoundException: public LAException
{
    public:
        LAKeyNotFoundException(std::string key);
        virtual ~LAKeyNotFoundException(){};

    public:
        std::string info;
};


class LADuplicateKeyException: public LAException
{
    public:
        LADuplicateKeyException(std::string key);
        virtual ~LADuplicateKeyException(){};

    public:
        std::string info;
};

class LAFileNotFoundException: public LAException
{
    public:
        LAFileNotFoundException(char* path);
        LAFileNotFoundException(std::string path);
        virtual ~LAFileNotFoundException(){};

    public:
        std::string info;
};

class LADirectoryOpenException: public LAException
{
    public:
        LADirectoryOpenException(char* path);
        LADirectoryOpenException(std::string path);
        virtual ~LADirectoryOpenException(){};
    public:
        std::string info;
};

class LADirectoryCanNotReadException: public LAException
{
    public:
        LADirectoryCanNotReadException(char* path);
        LADirectoryCanNotReadException(std::string path);
        virtual ~LADirectoryCanNotReadException(){};
    public:
        std::string info;
};

#endif
