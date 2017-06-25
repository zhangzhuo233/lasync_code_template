#ifndef __LOG_UTILS_HEADER__
#define __LOG_UTILS_HEADER__

#include "singletontemplate.h"
#include <string>
using namespace std;

class LALogUtils: public LASingletonTemplateBase<LALogUtils>
{
    public:
        LALogUtils();
        virtual ~LALogUtils(){};
        void log(const int& loglevel, const char* format, ...);
        void info_log(const char* format, ...);
        void debug_log(const char* format, ...);
        void error_log(const char* format, ...);
   private:
        std::string logfile;
};

#endif
