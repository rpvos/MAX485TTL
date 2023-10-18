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

RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial, bool use_buffer, uint8_t buffer_size, bool use_end_marker, char end_marker)
{
    this->de_pin_ = de_pin;
    this->re_pin_ = re_pin;
    this->serial_ = serial;
    this->use_buffer_ = use_buffer;
    this->use_end_marker_ = use_end_marker;
    this->end_marker_ = end_marker;
    this->buffer_size_ = buffer_size;
    if (use_buffer)
    {
        this->buffer_ = new char[buffer_size];
    }
    else
    {
        this->buffer_ = nullptr;
    }
    uint8_t buffer_write_cursor = 0;
    uint8_t buffer_read_cursor = 0;
    this->buffer_write_cursor_ = buffer_write_cursor;
    this->buffer_read_cursor_ = buffer_read_cursor;
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
    if (rs485.use_buffer_)
    {
        this->buffer_ = new char[this->buffer_size_];
    }
    else
    {
        this->buffer_ = nullptr;
    }
    uint8_t buffer_write_cursor = 0;
    uint8_t buffer_read_cursor = 0;
    this->buffer_write_cursor_ = buffer_write_cursor;
    this->buffer_read_cursor_ = buffer_read_cursor;
    this->data_in_buffer_ = false;
    this->mode_ = rs485.mode_;
};

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

const char *RS485::ReadStringBuffer(void)
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

char RS485::BufferPeek(void)
{
    if (BufferAvailable())
    {
        return buffer_[buffer_read_cursor_];
    }
    return 0;
}

char RS485::BufferRead(void)
{
    if (BufferAvailable())
    {
        char c = buffer_[buffer_read_cursor_];
        if (buffer_read_cursor_ <= buffer_write_cursor_ - 1U)
        {
            buffer_read_cursor_ = 0;
            buffer_write_cursor_ = 0;
        }

        return c;
    }
    return 0;
}

int RS485::BufferAvailable(void)
{
    return buffer_write_cursor_ - buffer_read_cursor_;
}

int RS485::ReadIntoBuffer(void)
{
    if (!use_buffer_)
    {
        return -1;
    }

    SetMode(INPUT);
    int amount_of_chars = 0;

    while (available() > 0 && data_in_buffer_ == false)
    {
        char read_character = read();
        amount_of_chars++;

        if (read_character != end_marker_ || !use_end_marker_)
        {
            buffer_[buffer_write_cursor_] = read_character;
            buffer_write_cursor_++;
            if (buffer_write_cursor_ >= buffer_size_)
            {
                buffer_write_cursor_ = buffer_size_ - 1;
            }
        }
        else
        {
            // Check for edge case of \r\n as two characters
            if (buffer_write_cursor_ > 0)
            {
                if (buffer_[buffer_write_cursor_ - 1] == '\r')
                {
                    buffer_[buffer_write_cursor_ - 1] = '\0'; // terminate the string on \r
                }
            }

            buffer_[buffer_write_cursor_] = '\0'; // terminate the string
            buffer_write_cursor_ = 0;
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
    if (this->buffer_)
    {
        delete[] this->buffer_;
    }

    this->buffer_ = new char[this->buffer_size_];
    memcpy(this->buffer_, otherRS485.buffer_, buffer_size_);
    this->buffer_write_cursor_ = otherRS485.buffer_write_cursor_;
    this->buffer_read_cursor_ = otherRS485.buffer_read_cursor_;
    this->data_in_buffer_ = otherRS485.data_in_buffer_;

    mode_ = otherRS485.mode_;

    return *this;
}