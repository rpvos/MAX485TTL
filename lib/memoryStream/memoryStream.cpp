#include "memoryStream.h"

MemoryStream::MemoryStream(uint8_t buffersize)
{
    this->buffersize = buffersize;
    this->buffer = new uint8_t[buffersize];
    this->writeCursor = 0;
    this->readCursor = 0;
    this->inputLength = 0;
}

MemoryStream::~MemoryStream()
{
    delete[] this->buffer;
}

int MemoryStream::read()
{
    int c = peek();
    if (c >= 0)
    {
        // Make read cursor loop
        readCursor = (readCursor + 1) % buffersize;
        // Subtract 1 from length of chars that need to be read
        inputLength--;
    }
    return c;
}

int MemoryStream::peek()
{
    if (available() == 0)
    {
        // Input buffer empty
        return -1;
    }
    else
    {
        return buffer[readCursor];
    }
}

size_t MemoryStream::write(uint8_t c)
{
    if (inputLength >= buffersize)
        return 0; // buffer is full
    else
    {
        // Put char in buffer
        buffer[writeCursor] = c;
        // Make write cursor loop
        writeCursor = (writeCursor + 1) % buffersize;
        // Add 1 to length of chars that need to be read
        inputLength++;
        // Return 1 because 1 char is written
        return 1;
    }
}