#ifndef __SAMPLECONVERSION_H__
#define __SAMPLECONVERSION_H__

// global platform-dependent initialization
#include <ASIO/ginclude.h>

#if MAC
#define TRUNCATE 0

#elif ASIO_CPU_X86 || ASIO_CPU_SPARC || ASIO_CPU_MIPS
#define TRUNCATE 1
#undef MAXFLOAT
#define MAXFLOAT 0x7fffff00L
#endif

namespace eLibV2
{
    namespace ASIO
    {
        class SampleConversion
        {
        public:
            // format converters, input 32 bit integer
            // mono
            static void convertMono8(long *source, char *dest, long frames);
            static void convertMono8Unsigned(long *source, char *dest, long frames);
            static void convertMono16LSB(long *source, short *dest, long frames);
            static void convertMono16MSB(long *source, short *dest, long frames);
            static void convertMono24LSB(long *source, char *dest, long frames);
            static void convertMono24MSB(long *source, char *dest, long frames);
            static void convertMono64LSB(long *source, char *dest, long frames);
            static void convertMono64MSB(long *source, char *dest, long frames);

            // stereo interleaved
            static void convertStereo8Interleaved(long *left, long *right, char *dest, long frames);
            static void convertStereo8InterleavedUnsigned(long *left, long *right, char *dest, long frames);
            static void convertStereo16Interleaved(long *left, long *right, short *dest, long frames);
            static void convertStereo16InterleavedSmallEndian(long *left, long *right, short *dest, long frames);
            static void convertStereo24Interleaved(long *left, long *right, char *dest, long frames);
            static void convertStereo24InterleavedSmallEndian(long *left, long *right, char *dest, long frames);

            // stereo split
            static void convertStereo8(long *left, long *right, char *dLeft, char *dRight, long frames);
            static void convertStereo8Unsigned(long *left, long *right, char *dLeft, char *dRight, long frames);
            static void convertStereo16(long *left, long *right, short *dLeft, short *dRight, long frames);
            static void convertStereo16SmallEndian(long *left, long *right, short *dLeft, short *dRight, long frames);
            static void convertStereo24(long *left, long *right, char *dLeft, char *dRight, long frames);
            static void convertStereo24SmallEndian(long *left, long *right, char *dLeft, char *dRight, long frames);

            // integer in place conversions
            static void int32msb16to16inPlace(long *in, long frames);
            static void int32lsb16to16inPlace(long *in, long frames);
            static void int32msb16shiftedTo16inPlace(long *in1, long frames, long shift);
            static void int24msbto16inPlace(unsigned char *in, long frames);

            // integer to integer

            static void shift32(void* buffer, long shiftAmount, long targetByteWidth, bool reverseEndian, long frames);
            static void reverseEndian(void* buffer, long byteWidth, long frames);

            static void int32to16inPlace(void* buffer, long frames);
            static void int24to16inPlace(void* buffer, long frames);
            static void int32to24inPlace(void* buffer, long frames);
            static void int16to24inPlace(void* buffer, long frames);
            static void int24to32inPlace(void* buffer, long frames);
            static void int16to32inPlace(void* buffer, long frames);

            // float to integer
            static void float32toInt16inPlace(float* buffer, long frames);
            static void float32toInt24inPlace(float* buffer, long frames);
            static void float32toInt32inPlace(float* buffer, long frames);
        };
    }
}

#endif // __SAMPLECONVERSION_H__
