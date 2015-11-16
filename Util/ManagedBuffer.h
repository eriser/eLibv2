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
            ManagedBuffer(SInt16 bufferCount = DEFAULT_BUFFER_COUNT, SInt16 bufferSize = DEFAULT_BUFFER_SIZE);
            ~ManagedBuffer();

            SInt16 GetBufferCount() { return m_bufferCount; }
            SInt16 GetBufferSize() { return m_bufferSize; }

            SInt16 Read(SInt16 bufferIndex, SInt16 readSize, SInt16* output);
            SInt16 Write(SInt16 bufferIndex, SInt16 writeSize, SInt16* input);

        private:
            SInt16     m_bufferCount;
            SInt16     m_bufferSize;
            SInt16*    m_readingPosition;
            SInt16*    m_writingPosition;
            SInt16**   m_internalBuffer;
        };
    }
}

#endif
