#pragma once
#include "mutex.h"
#include "processor.h"
#include "scheduler.h"
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define NETCO_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        netco::EventWraper(netco::LogEvent::Ptr(new netco::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, threadIdx,\
                netco::getThisCoId(), time(0)))).getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define NETCO_LOG_DEBUG(logger) NETCO_LOG_LEVEL(logger, netco::LogLevel::Level::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define NETCO_LOG_INFO(logger) NETCO_LOG_LEVEL(logger, netco::LogLevel::Level::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define NETCO_LOG_WARN(logger) NETCO_LOG_LEVEL(logger, netco::LogLevel::Level::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define NETCO_LOG_ERROR(logger) NETCO_LOG_LEVEL(logger, netco::LogLevel::Level::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define NETCO_LOG_FATAL(logger) NETCO_LOG_LEVEL(logger, netco::LogLevel::Level::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define NETCO_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        netco::EventWraper(netco::LogEvent::Ptr(new netco::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, threadIdx,\
                netco::getThisCoId(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define NETCO_LOG_FMT_DEBUG(logger, fmt, ...) NETCO_LOG_FMT_LEVEL(logger, netco::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define NETCO_LOG_FMT_INFO(logger, fmt, ...)  NETCO_LOG_FMT_LEVEL(logger, netco::LogLevel::Level::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define NETCO_LOG_FMT_WARN(logger, fmt, ...)  NETCO_LOG_FMT_LEVEL(logger, netco::LogLevel::Level::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define NETCO_LOG_FMT_ERROR(logger, fmt, ...) NETCO_LOG_FMT_LEVEL(logger, netco::LogLevel::Level::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define NETCO_LOG_FMT_FATAL(logger, fmt, ...) NETCO_LOG_FMT_LEVEL(logger, netco::LogLevel::Level::FATAL, fmt, __VA_ARGS__)

/**
 * @brief 获取主日志器
 */
#define NETCO_LOG_ROOT() netco::LoggerManager::getInstance()->getRoot()

/**
 * @brief 获取name的日志器
 */
#define NETCO_LOG_NAME(name) netco::LoggerManager::getInstance()->getLogger(name)

#define NETCO_LOG() NETCO_LOG_DEBUG(NETCO_LOG_ROOT())

#define NETCO_LOG_FMT(fmt, ...) NETCO_LOG_FMT_DEBUG(NETCO_LOG_ROOT(), fmt, __VA_ARGS__)


namespace netco
{   
    class Logger;
    //日志级别
    class LogLevel
    {
    public:
        enum Level{
            UNKNOWN = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    };
    //日志事件
    class LogEvent
    {
    public:
        using Ptr = std::shared_ptr<LogEvent>;
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
        ,const char* file, int32_t line, uint32_t elapse
        ,uint32_t thread_id, uint32_t co_id, uint64_t time
        );
        const char* getFile() { return file_; }
        const uint32_t getLine() { return line_; }
        const uint32_t getElapse() { return elapse_; }
        const time_t getTime() { return time_; }
        const int getThreadId() { return threadId_; }
        const int getCoId() { return coId_; }
        const LogLevel::Level getLevel() { return level_; }
        const std::string getMsg() { return message_; } 
        std::stringstream& getSS() { return ss_; }
        const std::shared_ptr<Logger> getLogger() { return logger_; }
        void format(const char* fmt, ...);
        void format(const char* fmt, va_list al);
    private:
        const char* file_; //文件名
        uint32_t line_;//行号
        uint32_t elapse_;//程序启动到现在运行的毫秒数
        time_t time_ = 0;//时间戳
        int threadId_; //线程ID
        int coId_; //协程ID
        LogLevel::Level level_;//日志级别
        std::string message_;//日志消息       
        std::stringstream ss_;//日志消息内容流
        std::shared_ptr<Logger> logger_;
    };
    
    //日志事件包装器，在析构时写入日志到appenders
    class EventWraper{
    public:
        EventWraper(LogEvent::Ptr);
        ~EventWraper();
        std::stringstream& getSS() { return event_->getSS(); }
        const LogEvent::Ptr getEvent() { return event_;}
    private:
        LogEvent::Ptr event_;
    };
    
    //日志事件解析器
    class EventParse
    {
    public:
        static void DateTimeFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void TabFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void ThreadIdFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void ThreadNameFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void FiberIdFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void LevelFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void NameFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void FilenameFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void LineFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void MessageFormatItem(std::ostream& os, LogEvent::Ptr event);
        static void NewLineFormatItem(std::ostream& os, LogEvent::Ptr event);
    };
    //日志格式器
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
    class LoggerFormatter
    {
    public:
        using Ptr = std::shared_ptr<LoggerFormatter>;
        LoggerFormatter(const std::string& format = "%d%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");            
        std::string format(LogEvent::Ptr);
        void format(std::ostream&, LogEvent::Ptr);
        void init();
    private:
        //format序列的元素，要么是纯文本，要么是%x对应的解析器
        struct Item{
            union 
            {
                char * str;
                void (*Foo)(std::ostream& os, LogEvent::Ptr event);
            };
            bool isString;

            Item(const std::string& s) : isString(true) { str = new char[s.length()+1]; strcpy(str, s.c_str()); }
            Item(void (*f)(std::ostream& os, LogEvent::Ptr event)) : Foo(f), isString(false) {}
            
            ~Item() {
                if (isString) {
                    delete[] str;
                }
            }
        };
        //日志格式
        std::string format_;
        //从format_解析的日志格式序列
        std::vector<std::shared_ptr<Item>> formatSequence_;
    };
    //日志输出器
    class LoggerAppender{
    public:
        using Ptr = std::shared_ptr<LoggerAppender>;
        virtual ~LoggerAppender() {}
        virtual void log(LogLevel::Level, LogEvent::Ptr) = 0;
        void setFormatter(LoggerFormatter::Ptr formatter) { formatter_ = formatter; }
        LoggerFormatter::Ptr getFormatter() { return formatter_; }
        inline bool hasFormatter() { return !!formatter_;}
    
    protected:
        LogLevel::Level level_;
        LoggerFormatter::Ptr formatter_;
    };    

    //日志器
    class Logger
    {
    public:
        using Ptr = std::shared_ptr<Logger>;
        Logger(const std::string & name = "root");
        ~Logger() {}
        void log(LogLevel::Level, LogEvent::Ptr);
        void addAppender(LoggerAppender::Ptr);
        void delAppender(LoggerAppender::Ptr);
        void debug(LogEvent::Ptr);
        void info(LogEvent::Ptr);
        void warn(LogEvent::Ptr);
        void error(LogEvent::Ptr);
        void fatal(LogEvent::Ptr);
        void setLevel(LogLevel::Level);
        LogLevel::Level getLevel()const { return level_; }
        std::string getName()const { return logName_; }
        void setFormatter(LoggerFormatter::Ptr val);
        void setFormatter(const std::string& val);
        LoggerFormatter::Ptr getFormatter() { return formatter_; }
    private:
        std::vector<LoggerAppender::Ptr> appenders_;
        std::string logName_;
        LoggerFormatter::Ptr formatter_;
        netco::MutexLock locker_;
        LogLevel::Level level_;


    };
    //输出到STDOUT
    class StdoutLoggerAppender : public LoggerAppender{
    public:
        using Ptr = std::shared_ptr<StdoutLoggerAppender>;
        void log(LogLevel::Level, LogEvent::Ptr) override;
    private:

    };    
    //输出到文件
    class FileLoggerAppender : public LoggerAppender{
    public:
        using Ptr = std::shared_ptr<FileLoggerAppender>;
        FileLoggerAppender(const std::string & name);
        void log(LogLevel::Level, LogEvent::Ptr) override;
        //重新打开文件，文件打开成功返回true
        bool reopen();
    private:
        std::string fileName_;
        std::ofstream fileStream_;
    };

    /**
     * @brief 日志器管理类
     */
    class LoggerManager {
    public:
        static LoggerManager* getInstance();
        /**
         * @brief 获取日志器
         * @param[in] name 日志器名称
         */
        Logger::Ptr getLogger(const std::string& name);

        /**
         * @brief 返回主日志器
         */
        Logger::Ptr getRoot() const { return root_;}

    private:
        /**
         * @brief 构造函数
         */
        LoggerManager();
    private:
        /// Mutex
        netco::MutexLock loggerMapLocker;
        /// 日志器容器
        std::map<std::string, Logger::Ptr> loggerMap_;
        /// 主日志器
        Logger::Ptr root_;
    };
    
}