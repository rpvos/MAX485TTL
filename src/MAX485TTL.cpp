#include <Arduino.h>
#include <max485ttl.h>

RS485::RS485(uint8_t de_pin, uint8_t re_pin, Stream *serial)
{
    this->de_pin = de_pin;
    this->re_pin = re_pin;
    this->serial = serial;

    pinMode(de_pin, OUTPUT);
    pinMode(re_pin, OUTPUT);

    mode = INPUT;
    set_mode(INPUT);
};

RS485::~RS485()
{
    this->serial = nullptr;
};

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

void RS485::flush(void)
{
    if (serial)
    {
        serial->flush();
        set_mode(INPUT);
    }
}
