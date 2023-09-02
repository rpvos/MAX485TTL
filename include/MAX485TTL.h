#ifndef RS485_H
#define RS485_H

#include <Arduino.h>

/// @brief Stream must be opened before passing to this object (Serial.begin(Baudrate)). Note that the SofwareSerial maximum baudrate is less then the HardwareSerial.
class RS485 : public Stream
{
private:
    uint8_t de_pin;
    uint8_t re_pin;
    uint16_t buffersize;
    uint16_t timeout_millis;
    Stream *serial;
    char *buffer;

public:
    RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial);
    RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, uint16_t buffersize);
    RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, uint16_t buffersize, uint16_t timeout_millis);

    void set_mode(bool input);
    int available(void);
    int peek(void);
    int read(void);
    int availableForWrite(void);
    void flush(void);
    size_t write(uint8_t);
    using Print::print;
    using Print::write;

    char *receive_rs485();
};

#endif