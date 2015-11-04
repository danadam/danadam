#ifndef DANADAM_ALGORITHM
#define DANADAM_ALGORITHM

#include <algorithm>

#include "TrueValue.h"

namespace da
{

/**
 * Algorithm similar to std::generate_n().
 *
 * The type of the output container has to be specified as a template parameter
 * when the function is called.
 *
 * The type of the elements in the output container is deduced from the return
 * value of the GeneratorFn.
 *
 * Example:
 *      auto result = qn:generate_n<std::vector>(10, []() { return 42; });
 */
template<
    template <typename T, typename...> class OutputContainerT,
    typename SizeT,
    typename GeneratorFn,
    typename OutputElementT = typename std::decay<      // std::decay in case the function returns (const) ref
            typename std::result_of<GeneratorFn()>::type
        >::type,
    typename std::enable_if<
            ::detail::TrueValue<
                    typename std::result_of<GeneratorFn()>::type
                >::value,
            int
        >::type = 0
>
OutputContainerT<OutputElementT> generate_n(SizeT count, GeneratorFn&& generatorFn)
{
    OutputContainerT<OutputElementT> result;
    result.reserve(count);
    auto it = std::back_inserter(result);
    for (SizeT i = 0; i < count; i++)
    {
        *it++ = generatorFn();
    }
    return result;
}

/**
 * Algorithm similar to std::generate_n().
 *
 * GeneratorFn accepts a SizeT argument which takes values from 0 to count-1.
 *
 * The type of the output container has to be specified as a template parameter
 * when the function is called.
 *
 * The type of the elements in the output container is deduced from the return
 * value of the GeneratorFn.
 *
 * Example:
 *      auto output = qn:generate_n<std::vector>(10, [](int i) { return i*10; });
 */
template<
    template <typename T, typename...> class OutputContainerT,
    typename SizeT,
    typename GeneratorFn,
    typename OutputElementT = typename std::decay<      // std::decay in case the function returns (const) ref
            typename std::result_of<GeneratorFn(SizeT)>::type
        >::type,
    typename std::enable_if<
            ::detail::TrueValue<
                    typename std::result_of<GeneratorFn(SizeT)>::type
                >::value,
            int
        >::type = 0
>
OutputContainerT<OutputElementT> generate_n(SizeT count, GeneratorFn&& generatorFn)
{
    OutputContainerT<OutputElementT> result;
    result.reserve(count);
    auto it = std::back_inserter(result);
    for (SizeT i = 0; i < count; i++)
    {
        *it++ = generatorFn(i);
    }
    return result;
}


/**
 * A wrapper around std::transform().
 *
 * It takes the input container as an argument. It produces the output
 * container as a return value.
 *
 * The type of the output container is the same as the type of the input
 * container (e.g, QVector<T> on input will produce QVector<U> on output).
 *
 * The type of the elements in the output container is deduced from the return
 * value of the ProcessFn.
 *
 * Example:
 *
 *      QVector<int> input;
 *      auto output = qn::transform(input, [](int elem) { return 0.5 * elem; });
 *      // output is QVector<double>
 */
template<
    template <typename T, typename...> class ContainerT,
    typename InputElementT,
    typename ProcessFn,
    typename OutputElementT = typename std::decay<
            typename std::result_of<ProcessFn(InputElementT)>::type
        >::type
>
ContainerT<OutputElementT> transform(const ContainerT<InputElementT> & input, ProcessFn&& processFn)
{
    ContainerT<OutputElementT> result;
    result.reserve(input.size());
    std::transform(
            std::begin(input),
            std::end(input),
            std::back_inserter(result),
            std::forward<ProcessFn>(processFn)
        );
    return result;
}

/**
 * A wrapper around std::transform().
 *
 * It takes the input container as an argument. It produces the output
 * container as a return value.
 *
 * The type of the output container has to be specified as a template parameter
 * when the function is called.
 *
 * The type of the elements in the output container is deduced from the return
 * value of the ProcessFn.
 *
 * Example:
 *
 *      QVector<int> input;
 *      auto output = qn::transform<QList>(input, [](int elem) { return 0.5 * elem; });
 *      // output is QList<double>
 */
template<
    template <typename T, typename...> class OutputContainerT,
    typename InputContainerT,
    typename ProcessFn,
    typename OutputElementT = typename std::decay<
            typename std::result_of<ProcessFn(typename InputContainerT::value_type)>::type
        >::type
>
OutputContainerT<OutputElementT> transform(const InputContainerT& input, ProcessFn&& processFn)
{
    OutputContainerT<OutputElementT> result;
    result.reserve(input.size());
    std::transform(
            std::begin(input),
            std::end(input),
            std::back_inserter(result),
            std::forward<ProcessFn>(processFn)
        );
    return result;
}

} // namespace da

#endif
