#ifndef LOGGER_COMMON_H_GUARD
#define LOGGER_COMMON_H_GUARD

#define INIT_LOGGER() \
    da::LogOptions da::g_logOptions = { da::ELogLevel::trace }

namespace da
{

struct ELogLevel
{
    enum E { trace, info, warn, error };
    static inline const char * c_str(E e);
};

struct LogOptions
{
    ELogLevel::E logLevel;
};

extern LogOptions g_logOptions;

void setLogLevel(da::ELogLevel::E logLevel) { g_logOptions.logLevel = logLevel; }

} // namespace


// --- Visual C++ port of gettimeofday() and localtime_r()
#if defined(_MSC_VER)
#  include <windows.h>
#  define __func__ __FUNCTION__
    struct timezone2
    {
        __int32 tz_minuteswest; /* minutes W of Greenwich */
        bool tz_dsttime;        /* type of dst correction */
    };
    struct timeval2
    {
        __int32 tv_sec;         /* seconds */
        __int32 tv_usec;        /* microseconds */
    };
#  define timezone timezone2
#  define timeval timeval2
#  define snprintf sprintf_s
    inline int gettimeofday(struct timeval * tv, struct timezone * tz);
    inline struct tm * localtime_r(const time_t * timep, struct tm * result);
#endif
// --------------------------------


#if defined(_MSC_VER)
#  define ATTRIBUTE_FORMAT
#else
#  include <sys/time.h> // gettimeofday
#  define ATTRIBUTE_FORMAT __attribute__ ((format (printf, 1, 2)))
#endif

#include <stdio.h>
#include <stdarg.h>
#include <time.h>     // localtime_r

namespace da
{

static const int DATETIME_BUF_LEN = 24;
struct DateTimeString { char s[DATETIME_BUF_LEN]; };

inline DateTimeString datetimeString();

const char * ELogLevel::c_str(E e)
{
    switch (e)
    {
        case trace: return "TRACE";
        case info:  return "INFO";
        case warn:  return "WARN";
        case error: return "ERROR";
    }
    return "???";
}

inline DateTimeString datetimeString()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);

    // Change data to normal format
    struct tm f;
    time_t sec = tv.tv_sec;
    localtime_r(&sec, &f);

    DateTimeString dt;
    snprintf(dt.s, DATETIME_BUF_LEN, "%04d-%02d-%02d %02d:%02d:%02d,%03d",
            (f.tm_year + 1900), (f.tm_mon + 1), f.tm_mday,
            f.tm_hour, f.tm_min, f.tm_sec, (int)(tv.tv_usec / 1000)
        );
    return dt;
}

inline void logf_noop() { }
inline void logf(const char * fmt, ...) ATTRIBUTE_FORMAT;
inline void logf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

} // namespace


// --- Visual C++ port of gettimeofday() and localtime_r()
#if defined(_MSC_VER)
const __int64 DELTA_EPOCH_IN_MICROSECS = 11644473600000000;

int gettimeofday(struct timeval * tv, struct timezone * tz)
{
    FILETIME ft;
    __int64 tmpres = 0;
    TIME_ZONE_INFORMATION tz_winapi;
    int rez = 0;

    ZeroMemory(&ft, sizeof(ft));
    ZeroMemory(&tz_winapi, sizeof(tz_winapi));

    GetSystemTimeAsFileTime(&ft);

    tmpres = ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tv->tv_sec = (__int32)(tmpres*0.000001);
    tv->tv_usec =(tmpres%1000000);

    rez = GetTimeZoneInformation(&tz_winapi);
    tz->tz_dsttime = (rez==2) ? true : false;
    tz->tz_minuteswest = tz_winapi.Bias + ((rez==2) ? tz_winapi.DaylightBias : 0);

    return 0;
}

struct tm *localtime_r(const time_t * timep, struct tm * result)
{
    localtime_s(result, timep);
    return result;
}

#undef timezone
#undef timeval
#undef sprintf

#endif
// --------------------------------


#endif

