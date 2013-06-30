#ifndef DANADAM_HEX_H_GUARD
#define DANADAM_HEX_H_GUARD

#include <stdint.h>
#include <cstring>

namespace da
{

static inline bool ishex(char c)
{
    return (c - '0' < 10)
        || (c - 'a' < 'g' - 'a')
        || (c - 'A' < 'G' - 'A');
}
inline char enhex(uint8_t b)
{
    b = b & 0x0f;
    return b + (b < 10 ? '0' : -10 + 'a');
}
inline uint8_t dehex(char c)
{
    return (c - '0' < 10) ? (c - '0') :                // if digit
        (c - 'a' < 'g' - 'a') ? (c - 'a' + 10) :       // if lowercase letter
            (c - 'A' < 'G' - 'A') ? (c - 'A' + 10) :   // if uppercase letter
                -1;                                    // wrong hex
}
inline char * hexdumpLineRaw(const uint8_t * data, int64_t dataLen)
{
    char * const hex = new char[3*dataLen];
    char * cur = hex;
    for (int i = 0; i < dataLen; i++)
    {
        *(cur++) = enhex(data[i] >> 4);
        *(cur++) = enhex(data[i]);
        *(cur++) = (i+1 < dataLen) ? ' ' : '\0';
    }
    return hex;
}
inline char * hexdumpLineRaw(const char * data, int64_t dataLen)
{
    return hexdumpLineRaw(reinterpret_cast<const uint8_t *>(data), dataLen);
}
inline char * hexdumpRaw(const uint8_t * data, int64_t dataLen)
{
    const char header[] = "-- hexdump begin -------------------------------------------------------------\n";
    const char footer[] = "-- hexdump end ---------------------------------------------------------------";
    const int lineLength = sizeof(header)/sizeof(header[0]) - 1;    // without '\0'
    const int hexdumpSize =
        lineLength +   // header
        lineLength +   // footer
        (dataLen / 16 + (dataLen % 16 != 0 ? 1 : 0)) * lineLength;   // content
    char * const hex = new char[hexdumpSize];
    char * cur = hex;
    memcpy(cur, header, lineLength);    // copied without '\0'
    cur += lineLength;
    int last_LF_idx = (cur - hex) - 1;
    for (int i = 0; i < dataLen; i++)
    {
        const int bytePosInLine = i % 16;

        // offset part
        if (bytePosInLine == 0)          // if this is the beginning of the line
        {
            *(cur++) = enhex(i >> 12);
            *(cur++) = enhex(i >>  8);
            *(cur++) = enhex(i >>  4);
            *(cur++) = enhex(i);
            *(cur++) = ' '; *(cur++) = '-'; *(cur++) = ' ';
        }

        // hex part
        *(cur++) = enhex(data[i] >> 4);
        *(cur++) = enhex(data[i]);

        if (i+1 < dataLen)               // if there is more data
        {
            *(cur++) = ' ';
            if (bytePosInLine == 7)      // if this is the middle of the line
            {
                *(cur++) = ' ';
                *(cur++) = ' ';
            }
        }

        // ascii part
        if (bytePosInLine == 15 || i + 1 == dataLen)   // if this is end of the line or end of data
        {
            int lineLen = (cur - hex) - last_LF_idx - 1;
            for (int j = lineLen; j < 57; j++)
                *(cur++) = ' ';
            *(cur++) = ' '; *(cur++) = ' '; *(cur++) = '-'; *(cur++) = ' ';

            const int start = i - bytePosInLine;
            const int end = i + 16 - bytePosInLine;
            for (int j = start; j < end; j++)
            {
                if (j < dataLen)
                    if (data[j] >= 32 && data[j] <= 126)
                        *(cur++) = data[j];
                    else
                        *(cur++) = '.';
                else
                    *(cur++) = ' ';

                if (j % 16 == 7)   // if this is the middle of the ascii line
                    *(cur++) = ' ';
            }
            *(cur++) = '\n';
            last_LF_idx = (cur - hex) - 1;
        }
    }
    memcpy(cur, footer, lineLength);    // copied with '\0'
    cur += lineLength;
    return hex;
}
inline char * hexdumpRaw(const char * data, int64_t dataLen)
{
    return hexdumpRaw(reinterpret_cast<const uint8_t *>(data), dataLen);
}

} // namespace

#endif

