#ifndef MODTYPES_H_
#define MODTYPES_H_

#ifdef WIN32
typedef signed char SInt8;          ///< 8 bit integer type
typedef unsigned char UInt8;        ///< 8 bit integer type
typedef signed short SInt16;        ///< 16 bit integer type
typedef unsigned short UInt16;      ///< 16 bit integer type
typedef signed int SInt32;          ///< 32 bit integer type
typedef unsigned int UInt32;        ///< 32 bit integer type
typedef signed __int64 SInt64;      ///< 64 bit integer type
typedef unsigned __int64 UInt64;    ///< 64 bit integer type
#elif defined __APPLE__
// mac osx
#include <CoreServices/CoreServices.h>
#else
// all other platforms
#include <stdint.h>
typedef int8_t SInt8;       ///< 8 bit integer type
typedef uint8_t UInt8;      ///< 8 bit integer type
typedef int16_t SInt16;     ///< 16 bit integer type
typedef uint16_t UInt16;    ///< 16 bit integer type
typedef int32_t SInt32;     ///< 32 bit integer type
typedef uint32_t UInt32;    ///< 32 bit integer type
typedef int64_t SInt64;     ///< 64 bit integer type
typedef uint64_t UInt64;    ///< 64 bit integer type
#endif

union flint
{
    float f;
    SInt16 i;
};

#endif

