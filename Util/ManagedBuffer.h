#ifndef __MODMANAGEDBUFFER_H__
#define __MODMANAGEDBUFFER_H__

#include <Util/Logger.h>
#include <memory>

namespace eLibV2
{
    namespace Util
    {
        class ManagedBuffer
        {
        public:
            enum
            {
                DEFAULT_BUFFER_COUNT = 2,
                DEFAULT_BUFFER_SIZE = 32768
            };

        public:
            ManagedBuffer(int bufferCount = DEFAULT_BUFFER_COUNT, int bufferSize = DEFAULT_BUFFER_SIZE);
            ~ManagedBuffer();

            int GetBufferCount() { return m_bufferCount; }
            int GetBufferSize() { return m_bufferSize; }

            int Read(int bufferIndex, int readSize, int* output);
            int Write(int bufferIndex, int writeSize, int* input);

        private:
            int     m_bufferCount;
            int     m_bufferSize;
            int*    m_readingPosition;
            int*    m_writingPosition;
            int**   m_internalBuffer;
        };
    }
}

#endif
