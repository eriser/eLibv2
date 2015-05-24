#include "modManagedBuffer.h"

using namespace eLibV2::Util;

ManagedBuffer::ManagedBuffer(int bufferCount, int bufferSize)
    : m_bufferCount(bufferCount),
    m_bufferSize(bufferSize),
    m_readingPosition(NULL),
    m_writingPosition(NULL),
    m_internalBuffer(NULL)
{
    m_internalBuffer = new int*[m_bufferCount];
    for (int bufferIndex = 0; bufferIndex < bufferCount; bufferIndex++)
    {
        m_internalBuffer[bufferIndex] = new int[m_bufferSize];
        memset(m_internalBuffer[bufferIndex], 0, m_bufferSize);
    }

    m_readingPosition = new int[m_bufferCount];
    memset(m_readingPosition, 0, bufferCount);
    m_writingPosition = new int[m_bufferCount];
    memset(m_writingPosition, 0, bufferCount);
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
    if (bufferIndex < m_bufferCount)
    {
        if (output)
        {
            if( (readSize > 0) && (readSize <= m_bufferSize))
            {
                if (m_readingPosition[bufferIndex] + readSize > m_bufferSize)
                {
                    int byteCountFirstRead = (m_readingPosition[bufferIndex] + readSize) % m_bufferSize;
                    int byteCountSecondRead = readSize - byteCountFirstRead;
                    memcpy(output, m_internalBuffer[bufferIndex], sizeof(int) * byteCountFirstRead);
                    memcpy(output + byteCountFirstRead, m_internalBuffer[bufferIndex], sizeof(int) * byteCountSecondRead);

                    m_readingPosition[bufferIndex] = byteCountSecondRead;
                }
                else
                {
                    memcpy(output, m_internalBuffer[bufferIndex], sizeof(int) * readSize);
                    m_readingPosition[bufferIndex] += readSize;
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
    if (bufferIndex < m_bufferCount)
    {
        if (input)
        {
            if ((writeSize > 0) && (writeSize <= m_bufferSize))
            {
                if (m_readingPosition[bufferIndex] + writeSize > m_bufferSize)
                {
                    int byteCountFirstRead = (m_readingPosition[bufferIndex] + writeSize) % m_bufferSize;
                    int byteCountSecondRead = writeSize - byteCountFirstRead;
                    memcpy(m_internalBuffer[bufferIndex], input, sizeof(int) * byteCountFirstRead);
                    memcpy(m_internalBuffer[bufferIndex], input + byteCountFirstRead, sizeof(int) * byteCountSecondRead);

                    m_writingPosition[bufferIndex] = byteCountSecondRead;
                }
                else
                {
                    memcpy(m_internalBuffer[bufferIndex], input, sizeof(int) * writeSize);
                    m_writingPosition[bufferIndex] += writeSize;
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
