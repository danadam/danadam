#ifndef LOGGER_QT_H_GUARD
#define LOGGER_QT_H_GUARD

#include <QString>
#include <QMutex>
#include <QMutexLocker>

#include "loggercommon.h"

#define TRACE(msg) LOG(da::ELogLevel::trace) << QString(msg)
#define INFO(msg)  LOG(da::ELogLevel::info)  << QString(msg)
#define WARN(msg)  LOG(da::ELogLevel::warn)  << QString(msg)
#define ERROR(msg) LOG(da::ELogLevel::error) << QString(msg)

#define LOG(level) \
    da::LoggerHelper( \
            level >= da::g_logOptions.logLevel && da::g_logOptions.format.datetime ? da::datetimeString().s : 0, \
            level, \
            __FILE__, \
            __LINE__ \
        )

namespace da
{

class LoggerHelper
{
public:
    LoggerHelper(const char * dt, ELogLevel::E level, const char * file, int line)
        : m_dt(dt)
        , m_level(level)
        , m_file(file)
        , m_line(line)
    { }
    ~LoggerHelper()
    {
        static QMutex s_mutex;
        if (m_level >= g_logOptions.logLevel)
        {
            QMutexLocker locker(&s_mutex);
            logf(
                    m_dt,
                    da::g_logOptions.format.logLevel ? da::ELogLevel::c_str(m_level) : 0,
                    da::g_logOptions.format.place ? m_file : 0,
                    m_line,
                    "%s\n",
                    m_msg.toUtf8().data()
                );
        }
    }
    void operator<<(const QString & msg) { m_msg = msg; }

private:
    const char * m_dt;
    const ELogLevel::E m_level;
    const char * m_file;
    const int m_line;
    QString m_msg;
};

} // namespace

#endif

