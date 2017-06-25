#include <iostream>
#include <iterator>

#include "dirreader.h"


int main()
{
    LADirReader* dirreader = LADirReader::create_instance(".");
    vector<std::string>* dir_entries;
    try
    {
         dir_entries = dirreader->read_dir();
    }
    catch(LADirectoryOpenException &e)
    {
        cerr << e.info << endl;
    }
    catch(LADirectoryCanNotReadException &e)
    {
        cerr << e.info <<endl;
    }
    copy(dir_entries->begin(), dir_entries->end(), ostream_iterator<std::string>(cout, "\n"));
    cout<<endl;
    delete dirreader;
    dirreader = NULL;
    return 0;
}
