#ifndef RS485_H
#define RS485_H

#include <Arduino.h>

/// @brief
class RS485 : private Stream
{
public:
    // using Print::print;
    // using Print::println;
    // using Stream::readString;

    /// @brief Constructor using only the necessary
    /// @param de_pin Driver output enable pin number
    /// @param re_pin Receiver output enable pin number
    RS485(uint8_t de_pin, uint8_t re_pin);

    /// @brief Constructor with buffersize setting
    /// @param de_pin Driver output enable pin number
    /// @param re_pin Receiver output enable pin number
    /// @param buffersize Size of wich the buffer must be
    RS485(uint8_t de_pin, uint8_t re_pin, uint16_t buffersize);

    /// @brief Constructor with timeout setting
    /// @param de_pin Driver output enable pin number
    /// @param re_pin Receiver output enable pin number
    /// @param buffersize Size of wich the buffer must be
    /// @param timeout_millis Timeout of input stream for a message
    RS485(uint8_t de_pin, uint8_t re_pin, uint16_t buffersize, uint16_t timeout_millis);

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

    /// @brief Function used to toggle the DE and RE pin to let the module accept incomming data
    /// @param mode INPUT(0) or OUTPUT(1)
    void set_mode(uint8_t);

    /// @brief Function used to get the number of bytes available in de input buffer which holds 64 bytes
    /// @return Number of bytes available, if stream not available -1
    int available(void);

    /// @brief Function used to look at the first byte of the input buffer without taking it out
    /// @return First character of the buffer, if stream not available -1
    int peek(void);

    /// @brief Function used to read the first byte of the incomming data
    /// @return first byte or -1 if not available
    int read(void);

    /// @brief Function used to get the number of bytes available for writing in the serial buffer before blocking
    /// @return Number of bytes (characters), if stream not available -1
    int availableForWrite(void);

    /// @brief Function that is a blocking call to wait for all serial data to be sent
    void flush(void);

    /// @brief Function to send a single byte or multiple
    /// @param data is the byte that will be sent
    /// @return Amount of bytes written, if stream not available -1
    size_t write(uint8_t);

    size_t print(String);

    /// @brief Function used to read input data
    void receive(void);

    bool isNewData(void);

    String getNewData(void);

private:
    bool newData;

    uint8_t mode_output;
    uint8_t de_pin;
    uint8_t re_pin;
    uint16_t buffersize;
    uint16_t timeout_millis;
    Stream *serial;
    char *buffer;
};

#endif