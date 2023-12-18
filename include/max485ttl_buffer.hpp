#ifndef MAX485TTL_BUFFER_HPP_
#define MAX485TTL_BUFFER_HPP_
#include "max485ttl.hpp"

// class RS485_Buffer : public RS485
// {
//
//     /**
//      * @brief Construct a new rs485 buffer object
//      *
//      * @param buffer_size size of buffer used to store input, any input string exceeding this will be terminated on buffersize.
//      * @param use_end_marker true if buffer must be \0 terminated on end_marker
//      * @param end_marker Token which is used to determine end of input.
//      * So make sure the buffer is big enough to store even the longest input.
//      */
//     RS485_Buffer(uint8_t buffer_size = 64, bool use_end_marker = true, char end_marker = '\n');

//     RS485_Buffer operator=(const RS485_Buffer &otherRS485);

//     /**
//      * @brief Function to check if full message is stored in buffer. This is toggled when end marker is found.
//      *
//      * @return true when data is stored.
//      * @return false when buffer is empty.
//      */
//     bool IsDataInBuffer(void);

//     /**
//      * @brief Function used to read all available bytes
//      *
//      * @return int amount of bytes in buffer
//      */
//     int BufferAvailable(void);

//     /**
//      * @brief Function used to peek ar first character in buffer
//      *
//      * @return char first character in buffer
//      */
//     char BufferPeek(void);

//     /**
//      * @brief Function used to read first character of buffer
//      *
//      * @return char first character in buffer
//      */
//     char BufferRead(void);

//     /**
//      * @brief Function used to read the data out of the buffer.
//      *
//      * @return const char* data in buffer to end marker.
//      */
//     const char *ReadStringBuffer(void);

//     /**
//      * @brief Function used to read the stream and put the data into the buffer.
//      * This function should be called often when large amount of data is expected so the stream buffer doesn't overflow.
//      *
//      * @return int Amount of bytes received
//      */
//     int ReadIntoBuffer(void);

//     /**
//      * @brief Get the Buffer pointer
//      *
//      * @return const char* pointing to the buffer
//      */
//     const char *GetBuffer(void);

// private:
//     bool use_end_marker_;
//     char end_marker_;
//     bool data_in_buffer_;
//     bool use_buffer_;
//     uint8_t buffer_size_;
//     uint8_t buffer_write_cursor_;
//     uint8_t buffer_read_cursor_;
//     char *buffer_;
// };
#endif // MAX485TTL_BUFFER_HPP_