#include "../include/log.h"
#include <cstring>
#include <stdarg.h>

using namespace netco;

Logger::Logger(const std::string & name)
    : level_(LogLevel::DEBUG)
{
    logName_ = name;
    formatter_.reset(new LoggerFormatter("%d%T[%p]%T[%c]%T%f:%l%T%m%n"));    
}

void Logger::setLevel(LogLevel::Level level)
{
    SpinlockGuard lock(locker_);
    level_ = level;
}

void Logger::setFormatter(LoggerFormatter::Ptr val) {
    {
        SpinlockGuard lock(locker_);
        formatter_ = val;
    }
}

void Logger::setFormatter(const std::string& val) {
    std::cout << "---" << val << std::endl;
    LoggerFormatter::Ptr new_val(new LoggerFormatter(val));
    setFormatter(new_val);
}

void Logger::addAppender(LoggerAppender::Ptr appender){
    if(!appender->hasFormatter()){
        appender->setFormatter(formatter_);
    }
    SpinlockGuard lock(locker_);
    appenders_.push_back(appender);
}

void Logger::delAppender(LoggerAppender::Ptr appender){

    SpinlockGuard lock(locker_);
    for(auto it = appenders_.begin();
        it<appenders_.end();it++)
    {
        if(*it==appender)
        {
            appenders_.erase(it);
            return;
        }
    }
}

void Logger::log(LogLevel::Level level, LogEvent::Ptr event)
{
    SpinlockGuard lock(locker_);
    if(level >= level_){
        for(auto &i : appenders_){
            i->log(level, event);
        }
    }
}

void Logger::debug(LogEvent::Ptr event)
{
    log(LogLevel::Level::DEBUG, event);
}
void Logger::info(LogEvent::Ptr event)
{
    log(LogLevel::Level::INFO, event);
    
}
void Logger::warn(LogEvent::Ptr event)
{
    log(LogLevel::Level::WARN, event);
    
}
void Logger::error(LogEvent::Ptr event)
{
    log(LogLevel::Level::ERROR, event);
    
}
void Logger::fatal(LogEvent::Ptr event)
{
    log(LogLevel::Level::FATAL, event);
    
}

FileLoggerAppender::FileLoggerAppender(const std::string& name)
    :fileName_(name)
{
    reopen();
}
void FileLoggerAppender::log(LogLevel::Level level, LogEvent::Ptr event)
{
    if( level > level_){
        fileStream_ << formatter_->format(event);
    }
}
bool FileLoggerAppender::reopen(){
    if(fileStream_){
        fileStream_.close();
    }
    fileStream_.open(fileName_);
    return !!fileStream_;
}
void StdoutLoggerAppender::log(LogLevel::Level level, LogEvent::Ptr event)
{
    if( level > level_){
        std::cout << formatter_->format(event); 
    }
}

void EventParse::DateTimeFormatItem(std::ostream& os, LogEvent::Ptr event)
{
    struct tm tm;
    time_t time = event->getTime();
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    os << buf;
}
void EventParse::TabFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << '\t';
}
void EventParse::ThreadIdFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << event->getThreadId();
}
// void EventParse::ThreadNameFormatItem(std::ostream& os, LogEvent::Ptr event){

// }
void EventParse::FiberIdFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << event->getCoId();
}
void EventParse::LevelFormatItem(std::ostream& os, LogEvent::Ptr event){
    LogLevel::Level level = event->getLevel();
    std::string levelStr;
    switch (level)
    {
    case LogLevel::Level::DEBUG:
        levelStr = "DEBUG";
        break;
    case LogLevel::Level::INFO:
        levelStr = "INFO";
        break;
    case LogLevel::Level::WARN:
        levelStr = "WARN";
        break;
    case LogLevel::Level::ERROR:
        levelStr = "ERROR";
        break;
    case LogLevel::Level::FATAL:
        levelStr = "FATAL";
        break;    
    default:
        levelStr = "UNKNOWN";
        break;
    }
    os << levelStr;
}
void EventParse::NameFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << event->getLogger()->getName();
}
void EventParse::FilenameFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << event->getFile();
}
void EventParse::LineFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << event->getLine();
}
void EventParse::MessageFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << event->getSS().str();
}
void EventParse::NewLineFormatItem(std::ostream& os, LogEvent::Ptr event){
    os << std::endl;
}

LoggerFormatter::LoggerFormatter(const std::string& format)
    : format_(format)
{
    init();
}
void LoggerFormatter::init()
{
    int fmt_status = 0;
    int substr_start = 0;
    int format_length = format_.length();
    formatSequence_.clear();
    for(int i = 0; i < format_length; i++){
        if( format_[i] == '%' ){
            fmt_status = 1;
        }
        /*
            %d : %d 标识输出的是时间 {%Y-%m-%d %H:%M:%S}为时间格式, DateTimeFormatItem
            %T : Tab[\t]            TabFormatItem
            %t : 线程id             ThreadIdFormatItem
            %N : 线程名称           ThreadNameFormatItem
            %F : 协程id             FiberIdFormatItem
            %p : 日志级别           LevelFormatItem       
            %c : 日志名称           NameFormatItem
            %f : 文件名             FilenameFormatItem
            %l : 行号               LineFormatItem
            %m : 日志内容           MessageFormatItem
            %n : 换行符[\r\n]       NewLineFormatItem
        */
        else if( fmt_status ){
            switch(format_[i]){
                case 'd':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start+1))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::DateTimeFormatItem)));
                    break;
                case 'T':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::TabFormatItem)));                
                    break;
                case 't':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::ThreadIdFormatItem)));
                    break;
                case 'N':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    // formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::ThreadNameFormatItem)));//does not work
                    break;
                case 'F':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::FiberIdFormatItem)));
                    break;
                case 'p':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::LevelFormatItem)));                
                    break;
                case 'c':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    // formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::NameFormatItem)));//does not work
                    break;
                case 'f':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::FilenameFormatItem)));                
                    break;
                case 'l':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::LineFormatItem)));
                    break;
                case 'm':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::MessageFormatItem)));                
                    break;
                case 'n':
                    if(substr_start <= i-2)
                    {
                        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, i-1-substr_start))));
                    }
                    substr_start = i+1;
                    fmt_status = 0;
                    formatSequence_.push_back(std::move(std::make_shared<Item>(&EventParse::NewLineFormatItem)));                
                    break;
                default:
                    break;                    
            }
        }
    }
    if(substr_start < format_length){
        formatSequence_.push_back(std::move(std::make_shared<Item>(format_.substr(substr_start, format_length))));
    }
}
void LoggerFormatter::format(std::ostream& os, LogEvent::Ptr event)
{
    for(auto& i : formatSequence_)
    {
        if(i->isString){
            os << i->str;
        }
        else{
            i->Foo(os, event);
        }
    }
}
std::string LoggerFormatter::format(LogEvent::Ptr event)
{
    std::stringstream ss;
    for(auto& i : formatSequence_)
    {
        if(i->isString){
            ss << i->str;
        }
        else{
            i->Foo(ss, event);
        }
    }
    return ss.str();
}

LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
            ,const char* file, int32_t line, uint32_t elapse
            ,uint32_t thread_id, uint32_t co_id, uint64_t time)
    :file_(file)
    ,line_(line)
    ,elapse_(elapse)
    ,threadId_(thread_id)
    ,coId_(co_id)
    ,time_(time)
    ,logger_(logger)
    ,level_(level) {
}

void LogEvent::format(const char* fmt, ...) {
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char* fmt, va_list al) {
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if(len != -1) {
        ss_ << std::string(buf, len);
        free(buf);
    }
}

EventWraper::EventWraper(LogEvent::Ptr event)
    :event_(event)
{

}

EventWraper::~EventWraper()
{
    event_->getLogger()->log(event_->getLevel(), event_);
}

LoggerManager::LoggerManager()
{
    root_ = std::make_shared<Logger>("root");
    LoggerAppender::Ptr stdApp = std::make_shared<StdoutLoggerAppender>();
    root_->addAppender(stdApp);
    loggerMap_.insert({"root", root_});
}

LoggerManager* LoggerManager::getInstance()
{
    static LoggerManager myMng;
    return &myMng;
}

Logger::Ptr LoggerManager::getLogger(const std::string& name)
{
    {
        SpinlockGuard lock(loggerMapLocker);
        if(loggerMap_.find(name) != loggerMap_.end()){
            return loggerMap_[name];
        }
    }
    Logger::Ptr newLogger = std::make_shared<Logger>(name);
    {
        SpinlockGuard lock(loggerMapLocker);
        loggerMap_.insert({name, newLogger});
    }
    return newLogger;
}
