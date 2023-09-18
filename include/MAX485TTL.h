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
class RS485 : Stream
{
public:
    /**
     * @brief Constructor using only the necessary
     * @param de_pin Driver output enable pin number
     * @param re_pin Receiver output enable pin number
     * @param end_marker Token which is used to determine end of input
     * @param buffersize size of buffer used to store input, any input string exceeding this will be terminated on buffersize
     */
    RS485(uint8_t de_pin, uint8_t re_pin, char end_marker = '\n', uint8_t buffersize = 64);

    /**
     * @brief Destroy the RS485 object
     * Delete buffer and remove pointers
     */
    ~RS485(void);

    /**
     * @brief Set the Stream to the wanted serial stream.
     * Stream must be opened before passing to this object (Serial.begin(Baudrate)).
     * Note that the SofwareSerial maximum baudrate is less then the HardwareSerial.
     *
     * @param serial memory address of serial stream
     */
    void set_serial(Stream *serial);

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
    int available(void);

    /**
     * @brief Function used to read the first byte of the incomming data
     *
     * @return first byte or -1 if not available
     */
    int read(void);

    /**
     * @brief Function used to look at the first byte of the input buffer without taking it out
     *
     * @return First character of the buffer, if stream not available -1
     */
    int peek(void);

    /**
     * @brief Function to send a single byte
     *
     * @param data the byte that will be sent
     * @return true if succesfull, if stream not available -1
     */
    size_t write(uint8_t data);

    /**
     * @brief Function to send a buffer of bytes
     *
     * @param buffer is the byte array that will be sent
     * @param size is the size of the sent bytes
     * @return Amount of bytes written, if stream not available -1
     */
    size_t write(const uint8_t *buffer, size_t size);

    /**
     * @brief Function to send a buffer of bytes
     *
     * @param buffer is the byte array that will be sent
     * @param size is the size of the sent bytes
     * @return Amount of bytes written, if stream not available -1
     */
    size_t write(const char *buffer, size_t size);

    /**
     * @brief Function to send a String
     *
     * @param string is the chars being sent
     * @return Amount of bytes written, if stream not available -1
     */
    size_t print(const String &string);

    /**
     * @brief Function used to know if there is new data available
     *
     * @return true, function get_new_data() can be used to get the new data
     * @return false, no new data has been received
     */
    bool is_new_data(void);

    /**
     * @brief Get the new data
     * Data must be processed immediately because pointer will be recycled
     *
     * @return const char* carrying the data
     */
    const char *get_new_data(void);

    /**
     * @brief Function must be called to retrieve input
     * Must be called often enough so buffer will not overflow
     *
     */
    void update(void);

private:
    Stream *serial;

    uint8_t de_pin;
    uint8_t re_pin;

    uint8_t mode;

    bool newData;
    byte index;
    char endMarker;
    uint8_t buffersize;
    uint16_t timeout_millis;
    char *buffer;
};

#endif