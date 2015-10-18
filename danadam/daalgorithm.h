#ifndef DANADAM_ALGORITHM
#define DANADAM_ALGORITHM

#include <algorithm>

namespace da
{
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
