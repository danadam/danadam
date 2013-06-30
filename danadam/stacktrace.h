#ifndef DANADAM_STACKTRACE_H_GUARD
#define DANADAM_STACKTRACE_H_GUARD

#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <iomanip>
#include <sstream>

#include <cxxabi.h>

#include "scopeguard.h"
#include "scopeguard_helper.h"

namespace da
{

inline std::string demangleName(char* frame)
{
    const char * fileBeg = frame;
    int fileLen = 0;
    const char * funcBeg = 0;
    int funcLen = 0;
    const char * offsetBeg = 0;
    int offsetLen = 0;
    const char * returnBeg = 0;
    int returnLen = 0;

    // --- parse ----------------

    // format is: <file> "(" [[<function>] "+" <offset>] ") [" <return> "]"
    // format is: <file>(<function>+<offset>) [<return>]
    // e.g.:
    // some_file(some_function+0x123) [0x1234]
    // some_file(+0x123) [0x1234]
    // some_file() [0x1234]
    for (const char * c = frame; *c; ++c)
    {
        if (*c == '(' && !funcBeg)
        {
            funcBeg = c + 1;
            if (fileBeg && !fileLen)
                fileLen = c - fileBeg;
        }
        else if (*c == '+' && !offsetBeg)
        {
            offsetBeg = c + 1;
            if (funcBeg && !funcLen)
                funcLen = c - funcBeg;
        }
        else if (*c == ')')
        {
            if (offsetBeg && !offsetLen)
                offsetLen = c - offsetBeg;
            if (funcBeg && !funcLen)
                funcLen = c - funcBeg;
        }
        else if (*c == '[' && !returnBeg)
        {
            returnBeg = c + 1;
        }
        else if (*c == ']' && returnBeg && !returnLen)
        {
            returnLen = c - returnBeg;
        }
    }

    // --- demangle ----------------
    const char * demangledBeg = 0;
    int demangledLen = 0;

    if (funcLen)
    {
        frame[funcBeg + funcLen - frame] = '\0';
        int status;
        demangledBeg = abi::__cxa_demangle(funcBeg, NULL, 0, &status);
        if (demangledBeg)
            demangledLen = strlen(demangledBeg);
    }
    ON_BLOCK_EXIT(free_ptr<char>, demangledBeg);

    // --- build string ----------------

    std::ostringstream oss;
    char spacer[] = { '\0', '\0' };
    if (fileLen)
    {
        frame[fileBeg + fileLen - frame] = '\0';
        oss << fileBeg << ":";
        spacer[0] = ' ';
    }
    if (funcLen)
    {
        if (demangledLen)
            oss << spacer << demangledBeg;
        else
            oss << spacer << funcBeg << "()";
        spacer[0] = ' ';
    }

    if (offsetLen)
    {
        frame[offsetBeg + offsetLen - frame] = '\0';
        oss << spacer << "+" << offsetBeg;
        spacer[0] = ' ';
    }
    if (returnLen)
    {
        frame[returnBeg + returnLen - frame] = '\0';
        oss << spacer << "[" << returnBeg << "]";
        spacer[0] = ' ';
    }

    return oss.str();
}

inline std::string formatFrame(int frameNr, char * frame)
{
    std::ostringstream oss;
    oss << "Frame " << std::setw(2) << frameNr << ": " << demangleName(frame);
    return oss.str();
}

// works best with "-O0" compiler option and "-rdynamic" linker option
inline std::string getStackTrace(const std::string & prefix, int offset)
{
    static const int STACK_DEPTH = 32;

    void *stack[STACK_DEPTH];
    int count = backtrace(stack, STACK_DEPTH);

    char **functions = backtrace_symbols(stack, count);
    ON_BLOCK_EXIT(free_ptr<char*>, functions);

    std::ostringstream oss;
    for (int i = 1 + offset; i < count; i++) {
        oss << prefix << formatFrame(i, functions[i]) << std::endl;
    }

    return oss.str();
}

}

#endif
