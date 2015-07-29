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

                // buffer end will be reached
                // wrap around
                if (m_readingPosition[bufferIndex] + readSize > m_bufferSize)
                {
                    int byteCountFirstRead = (m_readingPosition[bufferIndex] + readSize) % m_bufferSize;
                    int byteCountSecondRead = readSize - byteCountFirstRead;
                    memcpy(output, m_internalBuffer[bufferIndex], sizeof(int) * byteCountFirstRead);
                    memcpy(output + byteCountFirstRead, m_internalBuffer[bufferIndex], sizeof(int) * byteCountSecondRead);

                    m_readingPosition[bufferIndex] = byteCountSecondRead;

                    // if still ahead after wrap around -> underrun
                    if (
                        (ReadAhead && m_readingPosition[bufferIndex] > m_writingPosition[bufferIndex]) ||
                        (!ReadAhead && m_readingPosition[bufferIndex] <= m_writingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> read-marker after write-marker");
                }
                else
                {
                    memcpy(output, m_internalBuffer[bufferIndex], sizeof(int) * readSize);
                    m_readingPosition[bufferIndex] += readSize;

                    // was ahead, is not anymore after write or vice versa
                    if (
                        (ReadAhead && m_readingPosition[bufferIndex] <= m_writingPosition[bufferIndex]) ||
                        (!ReadAhead && m_readingPosition[bufferIndex] > m_writingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> read-marker after write-marker");
                }
                ModuleLogger::print("read from %i [%li]", m_readingPosition[bufferIndex], bufferIndex);

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

                // buffer end will be reached
                // wrap around
                if (m_writingPosition[bufferIndex] + writeSize > m_bufferSize)
                {
                    int byteCountFirstRead = (m_writingPosition[bufferIndex] + writeSize) % m_bufferSize;
                    int byteCountSecondRead = writeSize - byteCountFirstRead;
                    memcpy(m_internalBuffer[bufferIndex], input, sizeof(int) * byteCountFirstRead);
                    memcpy(m_internalBuffer[bufferIndex], input + byteCountFirstRead, sizeof(int) * byteCountSecondRead);

                    m_writingPosition[bufferIndex] = byteCountSecondRead;

                    // if still ahead after wrap around -> underrun
                    if (
                        (WriteAhead && m_writingPosition[bufferIndex] > m_readingPosition[bufferIndex]) ||
                        (!WriteAhead && m_writingPosition[bufferIndex] <= m_readingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> write-marker after read-marker");
                }
                // no wrap around
                else
                {
                    memcpy(m_internalBuffer[bufferIndex], input, sizeof(int) * writeSize);
                    m_writingPosition[bufferIndex] += writeSize;

                    // was ahead, is not anymore after write or vice versa
                    if (
                        (WriteAhead && m_writingPosition[bufferIndex] <= m_readingPosition[bufferIndex]) ||
                        (!WriteAhead && m_writingPosition[bufferIndex] > m_readingPosition[bufferIndex])
                        )
                        ModuleLogger::print("buffer underrun -> write-marker after read-marker");
                }
                ModuleLogger::print("written to %i [%li]", m_writingPosition[bufferIndex], bufferIndex);

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
