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

/// @brief
class RS485 : public Stream
{
public:
    /**
     * @brief Constructor using only the necessary
     * @param de_pin Driver output enable pin number
     * @param re_pin Receiver output enable pin number
     * @param serial Stream to which the data needs to be send
     * Stream must be opened before passing to this object (Serial.begin(Baudrate)).
     * @param end_marker Token which is used to determine end of input
     * @param buffersize size of buffer used to store input, any input string exceeding this will be terminated on buffersize
     */
    RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial);

    /**
     * @brief Destroy the RS485 object
     * Delete buffer and remove pointers
     */
    ~RS485(void);

    /**
     * @brief Function used to toggle the DE and RE pin to let the module accept incomming data
     *
     * @param mode INPUT(0) or OUTPUT(1)
     */
    void set_mode(uint8_t);

    /**
     * @brief Function used to get the number of bytes available in de input buffer which holds 64 bytes
     *
     * @return Number of bytes available, if stream not available -1
     */
    virtual int available(void);

    /**
     * @brief Function used to read the first byte of the incomming data
     *
     * @return first byte or -1 if not available
     */
    virtual int read(void);

    /**
     * @brief Function used to look at the first byte of the input buffer without taking it out
     *
     * @return First character of the buffer, if stream not available -1
     */
    virtual int peek(void);

    /**
     * @brief Function to send a single byte
     *
     * @param data the byte that will be sent
     * @return true if succesfull, if stream not available -1
     */
    virtual size_t write(uint8_t data);

    /**
     * @brief Flushes the write buffer and set mode to input
     *
     */
    virtual void flush(void);

private:
    Stream *serial;

    uint8_t de_pin;
    uint8_t re_pin;

    uint8_t mode;
};

#endif