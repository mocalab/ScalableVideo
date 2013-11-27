#ifndef LOG_H
#define LOG_H
#include "QsLog.h"

///Define logging macros for custom formatting
#define FORMAT() __FILE__ << ":" << __LINE__ << "--"

//Define different logging levels
///Trace logging level
#define TRACE() QLOG_TRACE() << FORMAT()
///Debug logging level
#define DEBUG() QLOG_DEBUG() << FORMAT()
///Info logging level
#define INFO() QLOG_INFO() << FORMAT()
///Warning logging level
#define WARN() QLOG_WARN() << FORMAT()
///Error logging level
#define L_ERROR() QLOG_ERROR() << FORMAT()
///Define the log outfile
#define LOG_PATH "../logs/videostreamer.log"

#endif // LOG_H
