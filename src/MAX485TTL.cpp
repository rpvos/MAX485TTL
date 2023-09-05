#include <Arduino.h>
#include <MAX485TTL.h>

/// @brief Constructor using only the necessary
/// @param de_pin Driver output enable pin number
/// @param re_pin Receiver output enable pin number
/// @param serial Stream to which the module is connected
RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial)
{
    RS485(de_pin, re_pin, serial, 64);
};

/// @brief Constructor with buffersize setting
/// @param de_pin Driver output enable pin number
/// @param re_pin Receiver output enable pin number
/// @param serial Stream to which the module is connected
/// @param buffersize Size of wich the buffer must be
RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, uint16_t buffersize)
{
    RS485(de_pin, re_pin, serial, buffersize, 1000);
};

/// @brief Constructor with timeout setting
/// @param de_pin Driver output enable pin number
/// @param re_pin Receiver output enable pin number
/// @param serial Stream to which the module is connected
/// @param buffersize Size of wich the buffer must be
/// @param timeout_millis Timeout of input stream for a message
RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, uint16_t buffersize, uint16_t timeout_millis)
{
    this->de_pin = de_pin;
    this->re_pin = re_pin;
    this->serial = serial;
    this->buffersize = buffersize;
    this->buffer = new char[buffersize];
    this->timeout_millis = timeout_millis;

    // Set pins to output
    pinMode(de_pin, OUTPUT);
    pinMode(re_pin, OUTPUT);

    mode_output = -1;
    set_mode(INPUT);
};

/// @brief Function used to toggle the DE and RE pin to let the module accept incomming data
/// @param mode INPUT(0) or OUTPUT(1)
void RS485::set_mode(uint8_t mode)
{
    if (mode_output == mode)
    {
        return;
    }

    // Set the RS485 driver pin values
    digitalWrite(de_pin, mode);
    digitalWrite(re_pin, mode);
    mode_output = mode;
}

/// @brief Function used to get the number of bytes available in de input buffer which holds 64 bytes
/// @return Number of bytes available
int RS485::available(void)
{
    set_mode(INPUT);
    return serial->available();
}

/// @brief Function used to look at the first byte of the input buffer without taking it out
/// @return first character of the buffer
int RS485::peek(void)
{
    return serial->peek();
}

/// @brief Function used to read the first byte of the incomming data
/// @return first byte or -1 if not available
int RS485::read(void)
{
    set_mode(INPUT);
    return serial->read();
}

/// @brief Function used to get the number of bytes available for writing in the serial buffer before blocking
/// @return Number of bytes (characters)
int RS485::availableForWrite(void)
{
    return serial->availableForWrite();
}

/// @brief Function that is a blocking call to wait for all serial data to be sent
void RS485::flush(void)
{
    set_mode(OUTPUT);
    serial->flush();
}

/// @brief Function to send a single byte or multiple
/// @param data is the byte that will be sent
/// @return Amount of bytes written
size_t RS485::write(uint8_t data)
{
    set_mode(OUTPUT);
    return serial->write(data);
}

/// @brief Function used to read input data
/// @return char* of the input data
char *RS485::receive_rs485(void)
{
    set_mode(INPUT);

    // Wait for a message to be received
    unsigned long start = millis();
    while (!serial->available())
    {
        if (millis() - start > timeout_millis)
            return "";
    }

    if (serial->available())
    {
        int length = serial->readBytes(buffer, buffersize);
        if (length > 0)
        {
            // Coppy the data out of the buffer
            char *data = new char[length + 1];
            memcpy(data, buffer, length);

            // Add null termination to make it a string
            data[length] = '\0';

            return data;
        }
    }

    return "";
}
