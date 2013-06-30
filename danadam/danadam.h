#ifndef DANADAM_H_GUARD
#define DANADAM_H_GUARD

#define DANADAM_QT
//#define DANADAM_STD

#include "hex.h"
#include "itoa.h"
#include "scopeguard.h"
#include "scopeguard_helper.h"
#include "stacktrace.h"

#ifdef DANADAM_QT
#  undef DANADAM_STD
#  include <QtGlobal>
#  include <QString>
#  include <QByteArray>
#endif

#ifdef DANADAM_STD
#  include <sstream>
#  include <string>
#  include <stdint.h>
#  include <vector>
#endif


#define FUNCTOR1(name, param, ret) \
    struct name : public std::unary_function<param, ret>

#define FUNCTOR2(name, param1, param2, ret) \
    struct name : public std::binary_function<param1, param2, ret>


namespace da
{

// --- getStackTrace() ----------------

#ifdef DANADAM_STD

// works best with "-O0" compiler option and "-rdynamic" linker option
inline std::string getStackTrace(const std::string & prefix, int offset);

inline std::string getStackTrace()
{
    return getStackTrace("", 0);
}

inline std::string getStackTrace(int offset)
{
    return getStackTrace("", offset);
}

inline std::string getStackTrace(const std::string & prefix)
{
    return getStackTrace(prefix, 0);
}

#endif

#ifdef DANADAM_QT

// works best with "-O0" compiler option and "-rdynamic" linker option
inline QString getStackTrace(const QString & prefix, int offset)
{
    return QString::fromStdString(getStackTrace(prefix.toStdString(), offset));
}

inline QString getStackTrace()
{
    return getStackTrace(QString(), 0);
}

inline QString getStackTrace(int offset)
{
    return getStackTrace(QString(), offset);
}

inline QString getStackTrace(const QString & prefix)
{
    return getStackTrace(prefix, 0);
}

#endif

// --- QString::number() counterpart for Std ----------------
#ifdef DANADAM_STD
template <typename T>
inline std::string toString(T n, int fieldWidth = 0, int base = 10, char fillChar = ' ')
{
    std::ostringstream ss;
    ss.width(fieldWidth);
    ss.fill(fillChar);
    if (base == 16)
        ss.setf(std::ios::hex);
    else if (base == 8)
        ss.setf(std::ios::oct);
    ss << n;
    return ss.str();
}
#endif

// --- hexdump Qt ----------------

#ifdef DANADAM_QT
inline QString hexdumpLine(const quint8 * data, qint64 dataLen)
{
    QString hex;
    hex.reserve(dataLen * 3 + 8);
    hex.append("[").append(QString::number(dataLen)).append("] ");

    const char * const hexRaw = hexdumpLineRaw(data, dataLen);
    ON_BLOCK_EXIT(delete_array<char>, hexRaw);

    hex.append(hexRaw);

    return hex;
}
inline QString hexdumpLine(const char * data, qint64 dataLen)
{
    return hexdumpLine(reinterpret_cast<const quint8 *>(data), dataLen);
}
inline QString hexdumpLine(const QByteArray & data)
{
    return hexdumpLine(data.constData(), data.size());
}

inline QString hexdump(const quint8 * data, qint64 dataLen)
{
    const char * const hexRaw = hexdumpRaw(data, dataLen);
    ON_BLOCK_EXIT(delete_array<char>, hexRaw);
    return QString(hexRaw);
}
inline QString hexdump(const char * data, qint64 dataLen)
{
    return hexdump(reinterpret_cast<const quint8 *>(data), dataLen);
}
inline QString hexdump(const QByteArray & data)
{
    return hexdump(data.constData(), data.size());
}
#endif

// --- hexdump Std ----------------

#ifdef DANADAM_STD
inline std::string hexdumpLine(const uint8_t * data, int64_t dataLen)
{
    std::string hex;
    hex.reserve(dataLen * 3 + 8);
    hex.append("[").append(toString(dataLen)).append("] ");

    const char * const hexRaw = hexdumpLineRaw(data, dataLen);
    ON_BLOCK_EXIT(delete_array<char>, hexRaw);

    hex.append(hexRaw);

    return hex;
}
inline std::string hexdumpLine(const char * data, int64_t dataLen)
{
    return hexdumpLine(reinterpret_cast<const uint8_t *>(data), dataLen);
}
inline std::string hexdumpLine(const std::vector<uint8_t> & data)
{
    return hexdumpLine(&data[0], data.size());
}
inline std::string hexdumpLine(const std::vector<char> & data)
{
    return hexdumpLine(&data[0], data.size());
}

inline std::string hexdump(const uint8_t * data, int64_t dataLen)
{
    const char * const hexRaw = hexdumpRaw(data, dataLen);
    ON_BLOCK_EXIT(delete_array<char>, hexRaw);
    return std::string(hexRaw);
}
inline std::string hexdump(const char * data, int64_t dataLen)
{
    return hexdump(reinterpret_cast<const uint8_t *>(data), dataLen);
}
inline std::string hexdump(const std::vector<uint8_t> & data)
{
    return hexdump(&data[0], data.size());
}
inline std::string hexdump(const std::vector<char> & data)
{
    return hexdump(&data[0], data.size());
}
#endif

} // namespace

#endif

