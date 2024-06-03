#include "../include/log.h"

using namespace netco;

static netco::Logger::Ptr mylog = LoggerManager::getInstance()->getLogger("myLog");
int main()
{
    StdoutLoggerAppender::Ptr stdApp = std::make_shared<StdoutLoggerAppender>();
    mylog->addAppender(stdApp);
    FileLoggerAppender::Ptr fileApp = std::make_shared<FileLoggerAppender>("./log");
    mylog->addAppender(fileApp);
    NETCO_LOG_DEBUG(mylog)<<"hello";
    NETCO_LOG_FMT_DEBUG(mylog, "%s", "hello myargs");
}