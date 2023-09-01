#include "logger.h"
#include <log4cxx/basicconfigurator.h>

#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>


/* SINGLETON for logger → define STATIC MEMBER variable
 *
 * This means that there is only 1 instance of this variable, shared across all
 * Logger class instances. 
 *  → its lifetime therefore must start at program start!
 * 
 * NOTE: may not assign a value (see below)!
 */
log4cxx::LoggerPtr Logger::logger; 

// NOTE: this does not work!
// log4cxx::LoggerPtr LoggerWrapper::logger = log4cxx::Logger::getLogger("PRVH");

log4cxx::LoggerPtr& Logger::get() {

    // check if variable is actually initialized
    if (!logger) {

        /* logging to console */

        // if not, configure basic logger ...
        // log4cxx::BasicConfigurator::configure();

        // and initialize pointer to logger instance in static variable
        // logger = log4cxx::Logger::getLogger("PRVH");


        /* logging to file */

        // initialize pointer to logger instance in static variable
        logger = log4cxx::Logger::getLogger("PRVH");

        // create file appender with SimpleLayout that appends to file
        // log4cxx::AppenderPtr appender(new log4cxx::FileAppender(log4cxx::LayoutPtr(new log4cxx::SimpleLayout()), "logs.log", true));

        // define PatternLayout:
        // %d       timestamp
        // %-5p     log level
        // %c{1}    logger name
        // %m       log message
        // %n       new line
        log4cxx::LayoutPtr layout(new log4cxx::PatternLayout("%d [%t] %-5p %c - %m%n"));
    
        // create file appender with PatternLayout that appends to file
        log4cxx::AppenderPtr appender(new log4cxx::FileAppender(layout, "logs.log", true));

        // attach appender to logger
        logger->addAppender(appender);
    }

    // return static pointer to logger instance
    return logger;
}