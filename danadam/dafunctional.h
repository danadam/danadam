#ifndef DANADAM_FUNCTIONAL
#define DANADAM_FUNCTIONAL

#include <type_traits>

#include "TrueValue.h"

template<
    typename FunctionT,
    typename std::enable_if<
            detail::TrueValue<
                    typename std::result_of<FunctionT()>::type
                >::value,
            int
        >::type = 0
>
void operator*(int count, FunctionT&& func)
{
    for (int i = 0; i < count; i++)
    {
        func();
    }
}

template<
    typename FunctionT,
    typename std::enable_if<
            detail::TrueValue<
                    typename std::result_of<FunctionT(int)>::type
                >::value,
            int
        >::type = 0
>
void operator*(int count, FunctionT&& func)
{
    for (int i = 0; i < count; i++)
    {
        func(i);
    }
}

#endif
