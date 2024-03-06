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

class RS485
{
public:
    /**
     * @brief Constructor using only the necessary.
     *
     * @param de_pin Driver output enable pin number.
     * @param re_pin Receiver output enable pin number.
     * @param serial Stream to which the data needs to be send.
     * Stream must be opened before passing to this object (Serial.begin(Baudrate))..
     */
    RS485(const uint8_t de_pin, const uint8_t re_pin, Stream *const serial);

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
    int32_t available(void);

    /**
     * @brief Function used to read the first byte of the incomming data.
     *
     * @return first byte or -1 if not available.
     */
    int32_t read(void);

    /**
     * @brief Function used to look at the first byte of the input buffer without taking it out.
     *
     * @return First character of the buffer, if stream not available -1.
     */
    int32_t peek(void);

    /**
     * @brief Function to send a single byte.
     *
     * @param data the byte that will be sent.
     * @return true if succesfull, if stream not available -1.
     */
    size_t write(const uint8_t data);
    size_t write(const char data);

    size_t write(const uint8_t *const buffer, const size_t length);
    size_t write(const char *const buffer, const size_t length);

    /**
     * @brief Flushes the write buffer.
     *
     */
    void flush(void);

    /**
     * @brief Function used to wait for a input signal
     *
     * @param TimeOutInMillis duration of the maximum wait in Millisecond
     */
    void WaitForInput(const unsigned long TimeOutInMillisecond = 2000);

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
};

#endif