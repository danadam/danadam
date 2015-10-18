#ifndef DANADAM_STRING_UTILS_H_GUARD
#define DANADAM_STRING_UTILS_H_GUARD

#include <string>
#include <type_traits>

namespace da
{

    namespace detail
    {

    template<typename T, bool isNumber = std::is_arithmetic<T>::value || std::is_enum<T>::value>
    struct ToStringConverter;

    // Partial specialization for integers and floats, which uses std::to_string() for conversion
    template<typename T>
    struct ToStringConverter<T, true>
    {
        static std::string str(T value) { return std::to_string(value); }
    };

    // Partial specialization for rest of the types, which uses their convert operator if they have one
    template<typename T>
    struct ToStringConverter<T, false>
    {
        static std::string str(const T& value) { return std::string(value); }
    };

    /**
     * Templated to_string() function which uses above specializations. It is
     * intended as a fallback function for join() in case there is no better
     * overload found by ADL.
     */
    template<typename T>
    std::string to_string(const T& value)
    {
        return detail::ToStringConverter<T>::str(value);
    }

    } // namespace detail

/**
 * join() function for container of elements. It produces a std::string by
 * concatenating string representation of each element in the container,
 * separated with "separator" element. By default the string representation of
 * each element is obtained using "detail::to_string()" function, which in turn
 * uses "std::to_string()" for integral, float and enum types or std::string
 * convert operator for anything else. If the default action is not what you
 * want or if the class doesn't have a std::string convert operator, you can
 * add overload "to_string()" function in the namespace of the class, e.g.:
 *
 *     namespace customlib
 *     {
 *         std::string to_string(const MyType& obj)
 *         {
 *             // ...
 *         }
 *     }
 *
 *     Note: In case the class is in global namespace this becomes a global function.
 */
template<typename ListT, typename SeparatorT>
std::string join(const ListT& list, const SeparatorT& separator)
{
    using detail::to_string;    // in case no custom overload is found, use this one
    const std::string separatorString(to_string(separator));
    std::string joined;
    joined.reserve(list.size() * 16);
    for (auto&& elem: list)
    {
        if (!joined.empty())
            joined.append(separatorString);
        joined.append(to_string(elem));
    }
    return joined;
}

// http://stackoverflow.com/a/1493195/85696
// modified
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

    StringPos lastPos = 0;
    ContainerT tokens;

    while (true)
    {
        StringPos pos = str.find_first_of(delimiters, lastPos);
        const bool foundDelimiter = (pos != std::string::npos);

        if (!foundDelimiter)
            pos = str.length();

        if (pos != lastPos || !trimEmpty)
        {
            tokens.push_back(
                    ValueType(str.data() + lastPos, (SizeType)pos - lastPos)
                );
        }

        if (foundDelimiter)
            lastPos = pos + 1;
        else
            break;
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
