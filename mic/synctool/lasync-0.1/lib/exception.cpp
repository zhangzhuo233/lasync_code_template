#include "exception.h"

LAKeyNotFoundException::LAKeyNotFoundException(std::string key)
{
    this->info = key + " not found";
}

LADuplicateKeyException::LADuplicateKeyException(std::string key)
{
    this->info = key + " already exists";
}

LAFileNotFoundException::LAFileNotFoundException(char* path)
{
    this->info = std::string(path) + " can not be found";
}

LAFileNotFoundException::LAFileNotFoundException(std::string path)
{
    this->info = path + " can not be found";
}

LADirectoryOpenException::LADirectoryOpenException(char* path)
{
    this->info = std::string(path) + " can not be opened";
}

LADirectoryOpenException::LADirectoryOpenException(std::string path)
{
    this->info = path + " can not be opened";
}

LADirectoryCanNotReadException::LADirectoryCanNotReadException(char* path)
{
    this->info = std::string(path) + " can not be accessed";
}

LADirectoryCanNotReadException::LADirectoryCanNotReadException(std::string path)
{
    this->info = path + " can not be accessed";
}
