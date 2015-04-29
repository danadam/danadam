#ifndef DANADAM_STRING_ENUM
#define DANADAM_STRING_ENUM

#include "stringutils.h"

#include <vector>
#include <string>

#define DEFINE_STRING_ENUM(EnumType, ...) \
class EnumType \
{ \
private: \
    typedef std::vector<std::string> StringList; \
    static const StringList & names() \
    { \
        static StringList s_names = da::split<StringList>(#__VA_ARGS__, ", ", true); \
        return s_names; \
    } \
public: \
    enum E { __VA_ARGS__ }; \
 \
    EnumType() : m_value() { } \
    EnumType(EnumType::E value) : m_value(value) { } \
    EnumType & operator=(E value) { m_value = value; return *this; } \
 \
    operator EnumType::E() const { return m_value; } \
 \
    std::string str() const { return names()[m_value]; } \
 \
    bool fromString(const std::string & input) \
    { \
        for (int i = 0; i < names().size(); i++) \
        { \
            if (input == names()[i]) \
            { \
                m_value = static_cast<EnumType::E>(i); \
                return true; \
            } \
        } \
        return false; \
    } \
private: \
    E m_value; \
}

#endif
