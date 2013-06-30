#ifndef DANADAM_ITOA_H_GUARD
#define DANADAM_ITOA_H_GUARD

#include <assert.h>

namespace da
{

inline void reverse(char * str, int len)
{
    if (len < 0)
        return;

    int i = 0;
    int j = len - 1;
    while (i < j)
    {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++; j--;
    }
}

/*
 * Function to convert signed integer number to string. Result will be '\0' ended.
 *
 * n    - Number to convert.
 * buf  - Destination buffer.
 * bufsz    - Destination buffer size.
 * rc   - Will indicate if whole number fitted in destination buffer.
 * return   - Number of bytes written in buf (not counting '\0').
 */
inline int itoa(int64_t n, char * buf, const int bufsz, bool * rc = 0)
{
    const int MAX_DIGITS = 20;    // int64_t may have 19 digits plus '-' sign
    char internal[MAX_DIGITS];
    int actsz = 0;
    int sign = n < 0 ? 1 : 0;

    if (n == 0)
        internal[actsz++] = '0';

    if (n < 0)
    {
        internal[actsz++] = '-';
        n = -n;
    }

    while (n > 0)
    {
        assert(actsz < MAX_DIGITS);
        internal[actsz++] = '0' + n % 10;
        n /= 10;
    }

    reverse(internal + sign, actsz - sign);

    int left = bufsz-1;    // leave place for NUL
    if (rc)
        *rc = actsz <= left;

    int i = 0;
    while (left > 0 && i < actsz)
    {
        buf[i] = internal[i];
        i++; left--;
    }

    if (bufsz > 0)
        buf[i] = '\0';

    return bufsz-1 - left;
}
};

#endif

