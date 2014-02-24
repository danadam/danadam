#ifndef LOGGERF_H_GUARD
#define LOGGERF_H_GUARD

#define TRACEF(msg, ...) LOGF(da::ELogLevel::trace, msg , ##__VA_ARGS__)
#define INFOF(msg, ...)  LOGF(da::ELogLevel::info,  msg , ##__VA_ARGS__)
#define WARNF(msg, ...)  LOGF(da::ELogLevel::warn,  msg , ##__VA_ARGS__)
#define ERRORF(msg, ...) LOGF(da::ELogLevel::error, msg , ##__VA_ARGS__)

#include "loggercommon.h"

#define LOGF(level, msg, ...) \
    (level < da::g_logOptions.logLevel) \
        ? da::logf_noop() \
        : da::logf("%s %s (%s:%d) - " msg "\n", \
                da::datetimeString().s, \
                da::ELogLevel::c_str(level), \
                __FILE__, \
                __LINE__, \
                ##__VA_ARGS__ \
            )

#endif

