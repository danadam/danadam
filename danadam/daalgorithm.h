#ifndef DANADAM_ALGORITHM
#define DANADAM_ALGORITHM

#include <algorithm>

#include "sfinae.h"

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
    typename da::detail::SFINAE<
            typename std::result_of<GeneratorFn()>::type
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
    typename da::detail::SFINAE<
            typename std::result_of<GeneratorFn(SizeT)>::type
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

namespace detail
{

   struct General { };
   struct Special : General { };

   template<
       typename T,
       typename T::iterator (T::*)(typename T::iterator, typename T::iterator)
   >
   struct Has_Method_Iter_Iter { typedef int type; };

   template<
       typename ContainerT,
       typename PredicateT,
       typename detail::Has_Method_Iter_Iter<
               ContainerT,
               &ContainerT::erase
           >::type = 0
   >
   void prune(ContainerT& container, PredicateT&& predicate, detail::Special)
   {
       container.erase(
               std::remove_if(
                       std::begin(container),
                       std::end(container),
                       std::forward<PredicateT>(predicate)
                   ),
               std::end(container)
           );
   }

   template<typename ContainerT, typename PredicateT>
   void prune(ContainerT& container, PredicateT&& predicate, detail::General)
   {
       auto it = std::begin(container);
       const auto endIt = std::end(container);
       while (it != endIt)
           if (predicate(*it))
               it = container.erase(it);
           else
               ++it;
   }

} // namespace detail

/**
 * Remove elements which match predicate.
 */
template<typename ContainerT, typename PredicateT>
void prune(ContainerT& container, PredicateT&& predicate)
{
   // We have 2 detail::prune() function templates.
   //
   // If ContainerT has an erase() method accepting 2 iterators (e.g. std::vector)
   // then both templates can be instantiated. The one with Special argument
   // is a better match and it gets chosen in overload resolution.
   //
   // If ContainerT doesn't have an erase() method accepting 2 iterators (e.g.
   // std::map) then the template with detail::Special argument cannot be
   // instantiated. Thanks to SFINAE this is not an error and the other
   // template is chosen.
   detail::prune(
           container,
           std::forward<PredicateT>(predicate),
           detail::Special()
       );
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
