#ifndef __EXCEPTION_HEADER__
#define __EXCEPTION_HEADER__
#include <string>
using namespace std;

class LAException
{
    public:
        virtual ~LAException(){};
    public:
        std::string info;
};

class LAKeyNotFoundException: public LAException
{
    public:
        LAKeyNotFoundException(std::string key);
        virtual ~LAKeyNotFoundException(){};

};


class LADuplicateKeyException: public LAException
{
    public:
        LADuplicateKeyException(std::string key);
        virtual ~LADuplicateKeyException(){};

};

class LAFileNotFoundException: public LAException
{
    public:
        LAFileNotFoundException(char* path);
        LAFileNotFoundException(std::string path);
        virtual ~LAFileNotFoundException(){};

};

class LADirectoryOpenException: public LAException
{
    public:
        LADirectoryOpenException(char* path);
        LADirectoryOpenException(std::string path);
        virtual ~LADirectoryOpenException(){};
};

class LADirectoryCanNotReadException: public LAException
{
    public:
        LADirectoryCanNotReadException(char* path);
        LADirectoryCanNotReadException(std::string path);
        virtual ~LADirectoryCanNotReadException(){};
};

class LAHardwareException: public LAException
{
    public:
        LAHardwareException(char* hwtype);
        LAHardwareException(std::string hwtype);
        virtual ~LAHardwareException() {};
};

class LAWorkloadException: public LAException
{
    public:
        LAWorkloadException(char* msg);
        LAWorkloadException(std::string msg);
        virtual ~LAWorkloadException() {};
};

class LAFileCanNotAccessException: public LAException
{
    public:
        LAFileCanNotAccessException(char* path);
        LAFileCanNotAccessException(std::string path);
        virtual ~LAFileCanNotAccessException(){};
};

#endif
