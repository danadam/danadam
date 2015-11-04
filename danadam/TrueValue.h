#ifndef DANADAM_TRUE_VALUE_H
#define DANADAM_TRUE_VALUE_H

namespace detail
{

/**
 * Helper template for using with enable_if and result_of, to limit
 * instantiation of other templates depending on function signature, e.g.:
 *
 *      template<
 *          typename FunctionT,
 *          typename std::enable_if<
 *                  detail::TrueValue<
 *                          typename std::result_of<FunctionT(int)>::type
 *                      >::value,
 *                  int
 *              >::type = 0
 *      >
 *      ... (FunctionT f) {  }
 */
template<typename>
struct TrueValue
{
    static const bool value = true;
};

} // namespace detail

#endif

