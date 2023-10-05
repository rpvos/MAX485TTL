/**
 * @file max485ttl.h
 * @author Rik Vos (rpvos.nl)
 * @brief Library used to send and recveive serial communication using the MAX485TTL modules
 * @version 0.1
 * @date 2023-09-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RS485_H
#define RS485_H

#include <Arduino.h>

class RS485 : public Stream
{
public:
    /**
     * @brief Constructor using only the necessary.
     * @param de_pin Driver output enable pin number.
     * @param re_pin Receiver output enable pin number.
     * @param serial Stream to which the data needs to be send.
     * Stream must be opened before passing to this object (Serial.begin(Baudrate))..
     * @param use_buffer to determine if string buffer is used
     * @param buffer_size size of buffer used to store input, any input string exceeding this will be terminated on buffersize.
     * @param use_end_marker true if buffer must be \0 terminated on end_marker
     * @param end_marker Token which is used to determine end of input.
     * So make sure the buffer is big enough to store even the longest input.
     */
    RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, bool use_buffer = true, uint8_t buffer_size = 64, bool use_end_marker = true, char end_marker = '\n');

    /**
     * @brief Copy constructor
     *
     * @param rs485 which settings will be used for new value
     */
    RS485(const RS485 &rs485);

    /**
     * @brief Destroy the RS485 object.
     * Delete buffer and remove pointers.
     */
    ~RS485(void);

    /**
     * @brief Function used to toggle the DE and RE pin to let the module accept incomming data.
     *
     * @param mode INPUT(0) or OUTPUT(1).
     */
    void SetMode(uint8_t mode);

    /**
     * @brief Function used to get the number of bytes available in de input buffer which holds 64 bytes.
     *
     * @return Number of bytes available, if stream not available -1.
     */
    virtual int available(void) override;

    /**
     * @brief Function used to read the first byte of the incomming data.
     *
     * @return first byte or -1 if not available.
     */
    virtual int read(void) override;

    /**
     * @brief Function used to look at the first byte of the input buffer without taking it out.
     *
     * @return First character of the buffer, if stream not available -1.
     */
    virtual int peek(void) override;

    /**
     * @brief Function to send a single byte.
     *
     * @param data the byte that will be sent.
     * @return true if succesfull, if stream not available -1.
     */
    virtual size_t write(uint8_t data) override;

    using Print::write;

    /**
     * @brief Flushes the write buffer and set mode to input.
     *
     */
    virtual void flush(void) override;

    /**
     * @brief Function to check if data is stored in buffer.
     *
     * @return true when data is stored.
     * @return false when buffer is empty.
     */
    bool IsDataInBuffer(void);

    /**
     * @brief Function used to read the data out of the buffer.
     *
     * @return const char* data in buffer to end marker.
     */
    const char *ReadBuffer(void);

    /**
     * @brief Function used to read the stream and put the data into the buffer.
     * This function should be called often when large amount of data is expected so the stream buffer doesn't overflow.
     *
     * @return int Amount of bytes received
     */
    int ReadIntoBuffer(void);

    /**
     * @brief Get the Buffer pointer
     *
     * @return const char* pointing to the buffer
     */
    const char *GetBuffer(void);

    /**
     * @brief Function used to wait for a input signal
     *
     * @param TimeOutInMillis duration of the maximum wait in Millisecond
     */
    void WaitForInput(unsigned long TimeOutInMillisecond = 2000);

    /**
     * @brief Overload = operator because use of dynamic memory
     *
     * @param otherRS485 to which it is compared
     * @return RS485 copy of object
     */
    RS485 &operator=(const RS485 &otherRS485);

private:
    Stream *serial_;

    uint8_t de_pin_;
    uint8_t re_pin_;

    uint8_t mode_;

    bool use_end_marker_;
    char end_marker_;
    bool data_in_buffer_;
    bool use_buffer_;
    uint8_t buffer_size_;
    uint8_t buffer_cursor_;
    char *buffer_;
};

#endif