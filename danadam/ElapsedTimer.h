#ifndef DANADAM_ELAPSED_TIMER_GUARD
#define DANADAM_ELAPSED_TIMER_GUARD

// requires: MyQtDebug.h

#include <stdint.h>
#include <time.h>

#include <string>
#include <sstream>
#include <iomanip>

#include "flavour.h"
#ifdef DANADAM_QT
#  include <QString>
#endif

class ElapsedTimer
{
public:
    static std::string toString(uint64_t duration);
#ifdef DANADAM_QT
    static QString toQString(quint64 duration) { return toString(duration).c_str(); }
#endif

    ElapsedTimer() : m_ts(0) { start(); }

    bool start()
    {
        struct timespec ts = { 0, 0 };
        const bool isOk = clock_gettime(CLOCK_MONOTONIC, &ts) == 0;
        if (isOk)
            m_ts = 1000000000L * ts.tv_sec + ts.tv_nsec;
        else
            TRACE("Failed to get clock value");
        return isOk;
    }

    uint64_t elapsed()
    {
        struct timespec ts = { 0, 0 };
        const bool isOk = clock_gettime(CLOCK_MONOTONIC, &ts) == 0;
        if (!isOk)
            TRACE("Failed to get clock value");
        return isOk ? (1000000000L * ts.tv_sec + ts.tv_nsec) - m_ts : 0;
    }

    std::string elapsedString() { return toString(elapsed()); }
#ifdef DANADAM_QT
    QString elapsedQString() { return toQString(elapsed()); }
#endif

private:
    uint64_t m_ts;
};

// static
std::string ElapsedTimer::toString(uint64_t duration)
{
    using namespace std;

    uint64_t nsecs = duration % 1000000000L;
    duration /= 1000000000L;
    uint64_t secs = duration % 60;
    duration /= 60;
    uint64_t minutes = duration % 60;
    duration /= 60;
    uint64_t hours = duration;

    ostringstream oss;
    oss << setfill('0');

    bool fullFieldWidth = false;

    if (hours > 0)
    {
        oss << hours << ":";
        fullFieldWidth = true;
    }

    if (minutes > 0)
    {
        if (fullFieldWidth)
            oss << setw(2);
        oss << minutes << ":";
        fullFieldWidth = true;
    }

    if (fullFieldWidth)
        oss << setw(2);
    oss << secs << ".";

    oss << setw(9) << nsecs;

    return oss.str();
}

#endif

