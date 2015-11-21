#include <Util/ManagedBuffer.h>
#include <Util/Defines.h>
#include <memory.h>

using namespace eLibV2::Util;

ManagedBuffer::ManagedBuffer(SInt16 bufferCount, SInt16 bufferSize)
    : m_bufferCount(bufferCount),
    m_bufferSize(bufferSize),
    m_readingPosition(NULL),
    m_writingPosition(NULL),
    m_internalBuffer(NULL)
{
    try
    {
        m_internalBuffer = new SInt16*[m_bufferCount];
        for (SInt16 bufferIndex = 0; bufferIndex < m_bufferCount; bufferIndex++)
        {
            m_internalBuffer[bufferIndex] = new SInt16[m_bufferSize];
            memset(m_internalBuffer[bufferIndex], 0, m_bufferSize * sizeof(SInt16));
        }

        m_readingPosition = new SInt16[m_bufferCount];
        memset(m_readingPosition, 0, m_bufferCount * sizeof(SInt16));
        m_writingPosition = new SInt16[m_bufferCount];
        memset(m_writingPosition, 0, m_bufferCount * sizeof(SInt16));
    }
    catch (std::bad_alloc e)
    {
        ModuleLogger::print(LOG_CLASS_UTIL, "new failed");
    }
}

ManagedBuffer::~ManagedBuffer()
{
    if (m_internalBuffer)
    {
        for (SInt16 bufferIndex = 0; bufferIndex < m_bufferCount; bufferIndex++)
            delete[] m_internalBuffer[bufferIndex];
        delete[] m_internalBuffer;
        m_internalBuffer = NULL;
    }
}

SInt16 ManagedBuffer::Read(SInt16 bufferIndex, SInt16 readSize, SInt16* output)
{
    bool ReadAhead = false;

    if (bufferIndex < m_bufferCount)
    {
        if (output)
        {
            if( (readSize > 0) && (readSize <= m_bufferSize))
            {
                if (m_readingPosition[bufferIndex] > m_writingPosition[bufferIndex])
                    ReadAhead = true;

//                ModuleLogger::print("read (%li) from %i [%li]", readSize, m_readingPosition[bufferIndex], bufferIndex);
                if (m_readingPosition[bufferIndex] + readSize > m_bufferSize)
                {
                    SInt16 byteCountSecondRead = (m_readingPosition[bufferIndex] + readSize) % m_bufferSize;
                    SInt16 byteCountFirstRead = readSize - byteCountSecondRead;

                    memcpy(output, m_internalBuffer[bufferIndex] + m_readingPosition[bufferIndex], byteCountFirstRead * sizeof(SInt16));
                    memcpy(output + byteCountFirstRead, m_internalBuffer[bufferIndex], byteCountSecondRead * sizeof(SInt16));

                    m_readingPosition[bufferIndex] = byteCountSecondRead;
#if 0
                    // if still ahead after wrap around -> underrun
                    if (
                        (ReadAhead && m_readingPosition[bufferIndex] > m_writingPosition[bufferIndex]) ||
                        (!ReadAhead && m_readingPosition[bufferIndex] <= m_writingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> read-marker after write-marker");
#endif
                }
                else
                {
                    memcpy(output, m_internalBuffer[bufferIndex] + m_readingPosition[bufferIndex], readSize * sizeof(SInt16));
                    m_readingPosition[bufferIndex] += readSize;

#if 0
                    // was ahead, is not anymore after write or vice versa
                    if (
                        (ReadAhead && m_readingPosition[bufferIndex] <= m_writingPosition[bufferIndex]) ||
                        (!ReadAhead && m_readingPosition[bufferIndex] > m_writingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> read-marker after write-marker");
#endif
                }

                return readSize;
            }
            else
                return -3; // number of bytes to read <= 0
        }
        else
            return -2; // output buffer not set
    }
    else
        return -1; // requested buffer index out of bounds
}

SInt16 ManagedBuffer::Write(SInt16 bufferIndex, SInt16 writeSize, SInt16* input)
{
    bool WriteAhead = false;

    if (bufferIndex < m_bufferCount)
    {
        if (input)
        {
            if ((writeSize > 0) && (writeSize <= m_bufferSize))
            {
                if (m_writingPosition[bufferIndex] > m_readingPosition[bufferIndex])
                    WriteAhead = true;

//                ModuleLogger::print("write (%li) to %i [%li]", writeSize, m_writingPosition[bufferIndex], bufferIndex);
                if (m_writingPosition[bufferIndex] + writeSize > m_bufferSize)
                {
                    SInt16 byteCountSecondRead = (m_writingPosition[bufferIndex] + writeSize) % m_bufferSize;
                    SInt16 byteCountFirstRead = writeSize - byteCountSecondRead;

                    memcpy(m_internalBuffer[bufferIndex] + m_writingPosition[bufferIndex], input, byteCountFirstRead * sizeof(SInt16));
                    memcpy(m_internalBuffer[bufferIndex], input + byteCountFirstRead, byteCountSecondRead * sizeof(SInt16));

                    m_writingPosition[bufferIndex] = byteCountSecondRead;

#if 0
                    // if still ahead after wrap around -> underrun
                    if (
                        (WriteAhead && m_writingPosition[bufferIndex] > m_readingPosition[bufferIndex]) ||
                        (!WriteAhead && m_writingPosition[bufferIndex] <= m_readingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> write-marker after read-marker");
#endif
                }
                else
                {
                    memcpy(m_internalBuffer[bufferIndex] + m_writingPosition[bufferIndex], input, writeSize * sizeof(SInt16));
                    m_writingPosition[bufferIndex] += writeSize;

#if 0
                    // was ahead, is not anymore after write or vice versa
                    if (
                        (WriteAhead && m_writingPosition[bufferIndex] <= m_readingPosition[bufferIndex]) ||
                        (!WriteAhead && m_writingPosition[bufferIndex] > m_readingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> write-marker after read-marker");
#endif
                }

                return writeSize;
            }
            else
                return -3; // number of bytes to write <= 0
        }
        else
            return -2; // input buffer not set
    }
    else
        return -1; // requested buffer index out of bounds
}
