#ifndef DANADAM_SFINAE_H
#define DANADAM_SFINAE_H

namespace da
{
namespace detail
{

template<typename> struct SFINAE
{
    typedef int type;
};

} // namespace detail
} // namespace da

#endif
