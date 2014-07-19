#ifndef DANADAM_STRING_UTILS_H_GUARD
#define DANADAM_STRING_UTILS_H_GUARD

#include <string>

namespace da
{

// http://stackoverflow.com/a/1493195/85696
template < class ContainerT>
ContainerT split(
        const std::string & str,
        const std::string & delimiters = " ",
        bool trimEmpty = false
    )
{
    typedef ContainerT Base;
    typedef typename Base::value_type ValueType;
    typedef typename ValueType::size_type SizeType;
    typedef std::string::size_type StringPos;

    StringPos pos, lastPos = 0;
    ContainerT tokens;

    while (true)
    {
        pos = str.find_first_of(delimiters, lastPos);
        if (pos == std::string::npos)
        {
            pos = str.length();

            if (pos != lastPos || !trimEmpty)
            {
                tokens.push_back(
                        ValueType(str.data() + lastPos, (SizeType)pos - lastPos)
                    );
            }

            break;
        }
        else
        {
            if (pos != lastPos || !trimEmpty)
            {
                tokens.push_back(
                        ValueType(str.data() + lastPos, (SizeType)pos - lastPos)
                    );
            }
        }

        lastPos = pos + 1;
    }

    return tokens;
}

inline std::string escapeString(
        const std::string & str,
        const std::string & specialCharacters,
        char escapeCharacter
    )
{
    const std::string toEscape =
        specialCharacters.find_first_of(escapeCharacter) == std::string::npos
            ? specialCharacters + escapeCharacter
            : specialCharacters;

    std::string result;

    std::string::size_type pos = 0;
    std::string::size_type lastPos = 0;
    while ((pos = str.find_first_of(toEscape, lastPos)) != std::string::npos)
    {
        result += str.substr(lastPos, pos - lastPos);
        result += escapeCharacter;
        result += str[pos];

        lastPos = pos + 1;
    }
    result += str.substr(lastPos, pos - lastPos);
    return result;
}

inline std::string unescapeString(
        const std::string & str,
        char escapeCharacter
    )
{
    const std::string::size_type length = str.size();
    std::string result;
    std::string::size_type pos = 0;
    std::string::size_type lastPos = 0;
    while ((pos = str.find_first_of(escapeCharacter, lastPos)) != std::string::npos)
    {
        result += str.substr(lastPos, pos - lastPos);
        lastPos = pos + 1;
        if (lastPos < length)
        {
            result += str[lastPos];
            lastPos++;
        }
    }
    result += str.substr(lastPos, pos - lastPos);
    return result;
}

}

#endif
