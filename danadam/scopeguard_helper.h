#ifndef DANADAM_SCOPEGUARD_HELPER_H_GUARD
#define DANADAM_SCOPEGUARD_HELPER_H_GUARD

#include <cstdlib>

namespace da
{

template <typename T>
void free_ptr(const T * p) { free((void *)p); }

template <typename T>
void delete_ptr(const T * p) { delete p; }

template <typename T>
void delete_array(const T * p) { delete[] p; }

}

#endif

