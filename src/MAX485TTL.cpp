#include <Arduino.h>
#include <max485ttl.h>

RS485::RS485(uint8_t de_pin, uint8_t re_pin, char end_marker = '\n', uint8_t buffersize = 64)
{
    this->de_pin = de_pin;
    this->re_pin = re_pin;
    this->serial = nullptr;
    this->buffersize = buffersize;
    this->buffer = new char[this->buffersize];
    this->newData = false;
    this->endMarker = end_marker;
    this->index = 0;

    pinMode(de_pin, OUTPUT);
    pinMode(re_pin, OUTPUT);

    mode = INPUT;
    set_mode(INPUT);
};

RS485::~RS485()
{
    delete[] this->buffer;
    this->serial = nullptr;
};

void RS485::set_serial(Stream *serial)
{

    this->serial = serial;
}

void RS485::set_mode(uint8_t new_mode)
{
    if (mode == new_mode)
    {
        return;
    }

    if (new_mode == INPUT)
    {
        digitalWrite(de_pin, LOW);
        digitalWrite(re_pin, LOW);
        mode = new_mode;
        return;
    }
    else if (new_mode == OUTPUT)
    {
        digitalWrite(de_pin, HIGH);
        digitalWrite(re_pin, HIGH);
        mode = new_mode;
        return;
    }

    return;
}

int RS485::available(void)
{
    set_mode(INPUT);
    if (serial)
    {
        return serial->available();
    }

    return -1;
}

int RS485::peek(void)
{
    set_mode(INPUT);
    if (serial)
    {
        return serial->peek();
    }

    return -1;
}

int RS485::read(void)
{
    set_mode(INPUT);
    if (serial)
    {
        return serial->read();
    }

    return -1;
}

size_t RS485::write(uint8_t data)
{
    set_mode(OUTPUT);
    if (serial)
    {
        return serial->write(data);
    }

    return -1;
}

size_t RS485::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while (size--)
    {
        int response = write(*buffer++);
        if (response)
        {
            n++;
        }
        else if (response == -1)
        {
            return -1;
        }
        else
        {
            break;
        }
    }
    return n;
}

size_t RS485::write(const char *buffer, size_t size)
{
    return write((const uint8_t *)buffer, size);
}

size_t RS485::print(const String &string)
{
    return write(string.c_str(), string.length());
}

bool RS485::is_new_data(void)
{
    return newData;
}

const char *RS485::get_new_data(void)
{
    if (newData)
    {
        newData = false;
        return buffer;
    }

    return "";
}

void RS485::update(void)
{
    set_mode(INPUT);
    char rc;

    while (available() > 0 && newData == false)
    {
        rc = read();

        if (rc != endMarker)
        {
            buffer[index] = rc;
            index++;
            if (index >= buffersize)
            {
                index = buffersize - 1;
            }
        }
        else
        {
            buffer[index] = '\0'; // terminate the string
            index = 0;
            newData = true;
        }
    }
};