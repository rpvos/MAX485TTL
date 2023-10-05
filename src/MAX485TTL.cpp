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
#include <max485ttl.h>

RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, uint8_t buffer_size, bool use_end_marker, char end_marker)
{
    this->de_pin_ = de_pin;
    this->re_pin_ = re_pin;
    this->serial_ = serial;
    this->use_end_marker_ = use_end_marker;
    this->end_marker_ = end_marker;
    this->buffer_size_ = buffer_size;
    this->buffer_ = new char[buffer_size];
    this->buffer_cursor_ = 0;
    this->data_in_buffer_ = false;

    pinMode(de_pin, OUTPUT);
    pinMode(re_pin, OUTPUT);

    mode_ = INPUT;
    SetMode(INPUT);
};

RS485::RS485(const RS485 &rs485)
{
    this->de_pin_ = rs485.de_pin_;
    this->re_pin_ = rs485.re_pin_;
    this->serial_ = rs485.serial_;
    this->end_marker_ = rs485.end_marker_;
    this->buffer_size_ = rs485.buffer_size_;
    this->buffer_ = new char[this->buffer_size_];
    this->buffer_cursor_ = 0;
    this->data_in_buffer_ = false;

    pinMode(de_pin_, OUTPUT);
    pinMode(re_pin_, OUTPUT);

    mode_ = INPUT;
    SetMode(INPUT);
}

RS485::~RS485()
{
    delete[] buffer_;
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
        digitalWrite(re_pin_, LOW);
        mode_ = new_mode;
        return;
    }
    else if (new_mode == OUTPUT)
    {
        digitalWrite(de_pin_, HIGH);
        digitalWrite(re_pin_, HIGH);
        mode_ = new_mode;
        return;
    }

    return;
}

int RS485::available(void)
{
    SetMode(INPUT);
    if (serial_)
    {
        return serial_->available();
    }

    return -1;
}

int RS485::peek(void)
{
    SetMode(INPUT);
    if (serial_)
    {
        return serial_->peek();
    }

    return -1;
}

int RS485::read(void)
{
    SetMode(INPUT);
    if (serial_)
    {
        return serial_->read();
    }

    return -1;
}

size_t RS485::write(uint8_t data)
{
    SetMode(OUTPUT);
    if (serial_)
    {
        return serial_->write(data);
    }

    return -1;
}

void RS485::flush(void)
{
    if (serial_)
    {
        serial_->flush();
        SetMode(INPUT);
    }
}

bool RS485::IsDataInBuffer(void)
{
    return data_in_buffer_;
}

const char *RS485::ReadBuffer(void)
{
    if (data_in_buffer_)
    {
        data_in_buffer_ = false;
        return buffer_;
    }

    return "";
}

const char *RS485::GetBuffer(void)
{
    return buffer_;
}

int RS485::ReadIntoBuffer(void)
{
    SetMode(INPUT);
    int amount_of_chars = 0;

    while (available() > 0 && data_in_buffer_ == false)
    {
        char read_character = read();
        amount_of_chars++;

        if (read_character != end_marker_ || !use_end_marker_)
        {
            buffer_[buffer_cursor_] = read_character;
            buffer_cursor_++;
            if (buffer_cursor_ >= buffer_size_)
            {
                buffer_cursor_ = buffer_size_ - 1;
            }
        }
        else
        {
            // Check for edge case of \r\n as two characters
            if (buffer_cursor_ > 0)
            {
                if (buffer_[buffer_cursor_ - 1] == '\r')
                {
                    buffer_[buffer_cursor_ - 1] = '\0'; // terminate the string on \r
                }
            }

            buffer_[buffer_cursor_] = '\0'; // terminate the string
            buffer_cursor_ = 0;
            data_in_buffer_ = true;
        }
    }

    return amount_of_chars;
};

void RS485::WaitForInput(unsigned long TimeOutInMillisecond)
{
    SetMode(INPUT);

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
    this->end_marker_ = otherRS485.end_marker_;
    this->buffer_size_ = otherRS485.buffer_size_;
    this->buffer_ = otherRS485.buffer_;
    this->buffer_cursor_ = otherRS485.buffer_cursor_;
    this->data_in_buffer_ = otherRS485.data_in_buffer_;

    pinMode(de_pin_, OUTPUT);
    pinMode(re_pin_, OUTPUT);

    mode_ = otherRS485.mode_;
    SetMode(mode_);

    return *this;
}