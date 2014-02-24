#ifndef LOGGER_QT_H_GUARD
#define LOGGER_QT_H_GUARD

#include <QString>
#include <QMutex>
#include <QMutexLocker>

#include "loggercommon.h"

#define TRACE(msg) da::LoggerHelper(da::datetimeString().s, da::ELogLevel::trace, __FILE__, __LINE__) << QString(msg)
#define INFO(msg)  da::LoggerHelper(da::datetimeString().s, da::ELogLevel::info,  __FILE__, __LINE__) << QString(msg)
#define WARN(msg)  da::LoggerHelper(da::datetimeString().s, da::ELogLevel::warn,  __FILE__, __LINE__) << QString(msg)
#define ERROR(msg) da::LoggerHelper(da::datetimeString().s, da::ELogLevel::error, __FILE__, __LINE__) << QString(msg)


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
            logf("%s %s (%s:%d) - %s\n",
                    m_dt,
                    ELogLevel::c_str(m_level),
                    m_file,
                    m_line,
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

