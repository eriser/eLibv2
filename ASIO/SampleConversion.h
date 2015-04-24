#ifndef __SAMPLECONVERSION_H__
#define __SAMPLECONVERSION_H__

#if SGI 
#undef BEOS 
#undef MAC 
#undef WINDOWS
//
#define ASIO_BIG_ENDIAN 1
#define ASIO_CPU_MIPS 1
#elif defined(_WIN32) || defined(_WIN64)
#undef BEOS 
#undef MAC 
#undef SGI
#define WINDOWS 1
#define ASIO_LITTLE_ENDIAN 1
#define ASIO_CPU_X86 1
#elif BEOS
#undef MAC 
#undef SGI
#undef WINDOWS
#define ASIO_LITTLE_ENDIAN 1
#define ASIO_CPU_X86 1
//
#else
#define MAC 1
#undef BEOS 
#undef WINDOWS
#undef SGI
#define ASIO_BIG_ENDIAN 1
#define ASIO_CPU_PPC 1
#endif

// always
#define NATIVE_INT64 0
#define IEEE754_64FLOAT 1

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
            void convertStereo8(long *left, long *right, char *dLeft, char *dRight, long frames);
            void convertStereo8Unsigned(long *left, long *right, char *dLeft, char *dRight, long frames);
            void convertStereo16(long *left, long *right, short *dLeft, short *dRight, long frames);
            void convertStereo16SmallEndian(long *left, long *right, short *dLeft, short *dRight, long frames);
            void convertStereo24(long *left, long *right, char *dLeft, char *dRight, long frames);
            void convertStereo24SmallEndian(long *left, long *right, char *dLeft, char *dRight, long frames);

            // integer in place conversions

            void int32msb16to16inPlace(long *in, long frames);
            void int32lsb16to16inPlace(long *in, long frames);
            void int32msb16shiftedTo16inPlace(long *in1, long frames, long shift);
            void int24msbto16inPlace(unsigned char *in, long frames);

            // integer to integer

            void shift32(void* buffer, long shiftAmount, long targetByteWidth,
                bool reverseEndian, long frames);
            void reverseEndian(void* buffer, long byteWidth, long frames);

            void int32to16inPlace(void* buffer, long frames);
            void int24to16inPlace(void* buffer, long frames);
            void int32to24inPlace(void* buffer, long frames);
            void int16to24inPlace(void* buffer, long frames);
            void int24to32inPlace(void* buffer, long frames);
            void int16to32inPlace(void* buffer, long frames);

            // float to integer

            void float32toInt16inPlace(float* buffer, long frames);
            void float32toInt24inPlace(float* buffer, long frames);
            void float32toInt32inPlace(float* buffer, long frames);
        };
    }
}

#endif // __SAMPLECONVERSION_H__
