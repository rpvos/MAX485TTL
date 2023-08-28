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

    String receive_rs485();
};

#pragma region Constructor

RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial)
{
    RS485(de_pin, re_pin, serial, 256);
};

RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, uint16_t buffersize)
{
    RS485(de_pin, re_pin, serial, buffersize, 1000);
};

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

    // Set module to listening the RS485 driver
    digitalWrite(de_pin, LOW);
    digitalWrite(re_pin, LOW);
};

#pragma endregion

int RS485::available(void)
{
    return serial->available();
}

int RS485::peek(void)
{
    return serial->peek();
}

int RS485::read(void)
{
    return serial->read();
}

int RS485::availableForWrite(void)
{
    return serial->availableForWrite();
}

void RS485::flush(void)
{
    serial->flush();
}

/// @brief Function to send a single byte or multiple
/// @param data is the byte that will be sent
/// @return Amount of bytes written
size_t RS485::write(uint8_t data)
{
    // Set the RS485 driver as output
    digitalWrite(de_pin, HIGH);
    digitalWrite(re_pin, HIGH);

    return serial->write(data);
}

void RS485::set_mode(bool input)
{
    if (input)
    {
        // Set the RS485 driver as input
        digitalWrite(de_pin, LOW);
        digitalWrite(re_pin, LOW);
    }
    else
    {
        // Set the RS485 driver as output
        digitalWrite(de_pin, HIGH);
        digitalWrite(re_pin, HIGH);
    }
}

// TODO add to read bytes
String RS485::receive_rs485()
{
    // Enable listen
    digitalWrite(de_pin, LOW);
    digitalWrite(re_pin, LOW);

    unsigned long start = millis();
    while (!serial->available())
    {
        if (millis() - start > timeout_millis)
            return "";
    }

    if (serial->available())
    {
        int length = serial->readBytesUntil('\n', buffer, buffersize);
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

#endif