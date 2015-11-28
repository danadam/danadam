#ifndef DANADAM_FUNCTIONAL
#define DANADAM_FUNCTIONAL

#include <type_traits>

#include "sfinae.h"

template<
    typename FunctionT,
    typename da::detail::SFINAE<
            typename std::result_of<FunctionT()>::type
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
    typename da::detail::SFINAE<
            typename std::result_of<FunctionT(int)>::type
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
