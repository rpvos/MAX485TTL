#ifndef MEMORY_STREAM_H_
#define MEMORY_STREAM_H_

#include <Stream.h>

class MemoryStream : public Stream
{
public:
    MemoryStream(uint8_t buffersize = 64);
    ~MemoryStream();

    virtual int available() { return inputLength; };

    virtual int read();

    virtual int peek();

    virtual size_t write(uint8_t c);

private:
    uint8_t *buffer;

    uint8_t buffersize;

    uint8_t writeCursor;

    uint8_t readCursor;

    uint8_t inputLength;
};

#endif // MEMORY_STREAM_H_