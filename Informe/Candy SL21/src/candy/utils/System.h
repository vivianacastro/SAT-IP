/****************************************************************************************[System.h]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Glucose_System_h
#define Glucose_System_h

#if defined(__linux__)
#include <fpu_control.h>
#endif

#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <chrono>

#ifndef PRIu64
#define PRIu64 "lu"
#define PRIi64 "ld"
#endif

//-------------------------------------------------------------------------------------------------

namespace Glucose {

extern double memUsed();            // Memory in mega bytes (returns 0 for unsupported architectures).
extern double memUsedPeak();        // Peak-memory in mega bytes (returns 0 for unsupported architectures).

}
//-------------------------------------------------------------------------------------------------
// Implementation of inline functions:

#if defined(_WIN32)

namespace Glucose {
// Getting the CPU time on Windows involves including Windows.h,
// for now let's restrict that to System.cc
std::chrono::milliseconds cpuTime(void);
}

#else
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

namespace Glucose {
static inline std::chrono::milliseconds cpuTime(void) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return std::chrono::milliseconds {ru.ru_utime.tv_sec * 1000 + ru.ru_utime.tv_usec / 1000 };
}
}

#endif
    


#endif
