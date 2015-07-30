#include <Util/ManagedBuffer.h>

using namespace eLibV2::Util;

ManagedBuffer::ManagedBuffer(int bufferCount, int bufferSize)
    : m_bufferCount(bufferCount),
    m_bufferSize(bufferSize),
    m_readingPosition(NULL),
    m_writingPosition(NULL),
    m_internalBuffer(NULL)
{
    m_internalBuffer = new int*[m_bufferCount];
    for (int bufferIndex = 0; bufferIndex < m_bufferCount; bufferIndex++)
    {
        m_internalBuffer[bufferIndex] = new int[m_bufferSize];
        memset(m_internalBuffer[bufferIndex], 0, m_bufferSize * sizeof(int));
    }

    m_readingPosition = new int[m_bufferCount];
    memset(m_readingPosition, 0, m_bufferCount * sizeof(int));
    m_writingPosition = new int[m_bufferCount];
    memset(m_writingPosition, 0, m_bufferCount * sizeof(int));
}

ManagedBuffer::~ManagedBuffer()
{
    if (m_internalBuffer)
    {
        for (int bufferIndex = 0; bufferIndex < m_bufferCount; bufferIndex++)
            delete[] m_internalBuffer[bufferIndex];
        delete[] m_internalBuffer;
        m_internalBuffer = NULL;
    }
}

int ManagedBuffer::Read(int bufferIndex, int readSize, int* output)
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

//                ModuleLogger::print("read (%li) from %i [%li]\n", readSize, m_readingPosition[bufferIndex], bufferIndex);
                if (m_readingPosition[bufferIndex] + readSize > m_bufferSize)
                {
                    int byteCountSecondRead = (m_readingPosition[bufferIndex] + readSize) % m_bufferSize;
                    int byteCountFirstRead = readSize - byteCountSecondRead;

                    memcpy(output, m_internalBuffer[bufferIndex] + m_readingPosition[bufferIndex], byteCountFirstRead * sizeof(int));
                    memcpy(output + byteCountFirstRead, m_internalBuffer[bufferIndex], byteCountSecondRead * sizeof(int));

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
                    memcpy(output, m_internalBuffer[bufferIndex] + m_readingPosition[bufferIndex], readSize * sizeof(int));
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

int ManagedBuffer::Write(int bufferIndex, int writeSize, int* input)
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

//                ModuleLogger::print("write (%li) to %i [%li]\n", writeSize, m_writingPosition[bufferIndex], bufferIndex);
                if (m_writingPosition[bufferIndex] + writeSize > m_bufferSize)
                {
                    int byteCountSecondRead = (m_writingPosition[bufferIndex] + writeSize) % m_bufferSize;
                    int byteCountFirstRead = writeSize - byteCountSecondRead;

                    memcpy(m_internalBuffer[bufferIndex] + m_writingPosition[bufferIndex], input, byteCountFirstRead * sizeof(int));
                    memcpy(m_internalBuffer[bufferIndex], input + byteCountFirstRead, byteCountSecondRead * sizeof(int));

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
                    memcpy(m_internalBuffer[bufferIndex] + m_writingPosition[bufferIndex], input, writeSize * sizeof(int));
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
