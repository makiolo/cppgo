#ifndef _CPPGO_API_H_
#define _CPPGO_API_H_

#define cppgo_VERSION_MAJOR 1
#define cppgo_VERSION_MINOR 0
#define cppgo_VERSION ((cppgo_VERSION_MAJOR << 16) | cppgo_VERSION_MINOR)

#ifdef _MSC_VER
    #ifdef cppgo_EXPORTS
        #define cppgo_API __declspec(dllexport)
    #else
        #define cppgo_API __declspec(dllimport)
    #endif
#else
    #ifdef cppgo_EXPORTS
        #define cppgo_API __attribute__((visibility("default")))
    #else
        #define cppgo_API
    #endif
#endif

using int64 = long long;
using int32 = signed int;
using int16 = signed short;
using int8 = signed char;

using uint64 = unsigned long long;
using uint32 = unsigned int;
using uint16 = unsigned short;
using uint8 = unsigned char;

using real64 = double;
using real32 = float;

using uint = uint32;
using real = real32;

#endif

