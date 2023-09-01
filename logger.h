#include <log4cxx/logger.h>

/* SINGLETON for logger */

class Logger {
private:

    // make constructor private due to singleton: prevent outside entities from 
    // creation of new instances and ensure there is only 1 instance!!
    // C++11: "default" constructor: tells compiler to generate it automatically
    Logger() = default; 

    // static pointer to logger instance
    static log4cxx::LoggerPtr logger;

public:

    // always return the same logger instance 
    static log4cxx::LoggerPtr& get();

    // singleton: prevent copy and assignment
    Logger(const Logger&) = delete; 
    void operator=(const Logger&) = delete;

};