#ifndef DANADAM_STRING_UTILS_H_GUARD
#define DANADAM_STRING_UTILS_H_GUARD

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

    std::string::size_type pos, lastPos = 0;
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
};

}

#endif
