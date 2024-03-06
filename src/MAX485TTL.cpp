/**
 * @file max485ttl.cpp
 * @author Rik Vos (rpvos.nl)
 * @brief Library used to send and recveive serial communication using the MAX485TTL modules
 * @version 0.1
 * @date 2023-09-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include "max485ttl.hpp"

RS485::RS485(const uint8_t de_pin, const uint8_t re_pin, Stream *const serial)
{
    this->de_pin_ = de_pin;
    this->re_pin_ = re_pin;
    this->serial_ = serial;

    pinMode(de_pin, OUTPUT);
    pinMode(re_pin, OUTPUT);

    // Initialise as unset
    mode_ = -1;
    SetMode(INPUT);
};

RS485::RS485(const RS485 &rs485)
{
    this->de_pin_ = rs485.de_pin_;
    this->re_pin_ = rs485.re_pin_;
    this->serial_ = rs485.serial_;
    this->mode_ = rs485.mode_;
};

RS485::~RS485()
{
    this->serial_ = nullptr;
};

void RS485::SetMode(uint8_t new_mode)
{
    if (mode_ == new_mode)
    {
        return;
    }

    if (new_mode == INPUT)
    {
        digitalWrite(de_pin_, LOW);
        if (re_pin_ != de_pin_)
        {
            digitalWrite(re_pin_, LOW);
        }

        mode_ = new_mode;
        return;
    }
    else if (new_mode == OUTPUT)
    {
        digitalWrite(de_pin_, HIGH);
        if (re_pin_ != de_pin_)
        {
            digitalWrite(re_pin_, HIGH);
        }

        mode_ = new_mode;
        return;
    }

    return;
}

int32_t RS485::available(void)
{
    if (serial_)
    {
        return serial_->available();
    }

    return -1;
}

int32_t RS485::peek(void)
{
    if (serial_)
    {
        return serial_->peek();
    }

    return -1;
}

int32_t RS485::read(void)
{
    if (serial_)
    {
        return serial_->read();
    }

    return -1;
}

size_t RS485::write(const uint8_t data)
{
    if (serial_)
    {
        return serial_->write(data);
    }

    return -1;
}

size_t RS485::write(const char data)
{
    if (serial_)
    {
        return serial_->write(data);
    }

    return -1;
}

size_t RS485::write(const uint8_t *const buffer, const size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        write(buffer[i]);
    }
    return length;
}

size_t RS485::write(const char *const buffer, const size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        write(buffer[i]);
    }
    return length;
}

void RS485::flush(void)
{
    if (serial_)
    {
        serial_->flush();
    }
}

void RS485::WaitForInput(const unsigned long TimeOutInMillisecond)
{
    unsigned long time = millis();
    while (((millis() - time) < TimeOutInMillisecond) && !available())
    {
        delay(1);
    }
}

RS485 &RS485::operator=(const RS485 &otherRS485)
{
    // Check for self assignment
    if (this == &otherRS485)
    {
        return *this;
    }

    this->de_pin_ = otherRS485.de_pin_;
    this->re_pin_ = otherRS485.re_pin_;
    this->serial_ = otherRS485.serial_;
    mode_ = otherRS485.mode_;

    return *this;
}