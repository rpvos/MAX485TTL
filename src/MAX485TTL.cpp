#include <Arduino.h>
#include <max485ttl.h>

RS485::RS485(uint8_t de_pin, uint8_t re_pin)
{
    RS485(de_pin, re_pin, 64);
};

RS485::RS485(uint8_t de_pin, uint8_t re_pin, uint16_t buffersize)
{
    RS485(de_pin, re_pin, buffersize, 1000);
};

RS485::RS485(uint8_t de_pin, uint8_t re_pin, uint16_t buffersize, uint16_t timeout_millis)
{
    this->de_pin = de_pin;
    this->re_pin = re_pin;
    this->serial = 0;
    this->buffersize = buffersize;
    this->buffer = new char[buffersize];
    this->timeout_millis = timeout_millis;
    this->newData = false;

    // Set pins to output
    pinMode(de_pin, OUTPUT);
    pinMode(re_pin, OUTPUT);

    mode_output = -1;
    set_mode(INPUT);
};

RS485::~RS485()
{
    delete this->buffer;
    this->serial = nullptr;
};

void RS485::set_serial(Stream *serial)
{
    this->serial = serial;
}

void RS485::set_mode(uint8_t mode)
{
    if (mode_output == mode)
    {
        return;
    }

    if (mode == INPUT)
    {
        digitalWrite(de_pin, LOW);
        digitalWrite(re_pin, LOW);
        mode_output = mode;
    }
    else if (mode == OUTPUT)
    {
        digitalWrite(de_pin, HIGH);
        digitalWrite(re_pin, HIGH);
        mode_output = mode;
    }
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

int RS485::availableForWrite(void)
{
    if (serial)
    {
        return serial->availableForWrite();
    }

    return -1;
}

void RS485::flush(void)
{
    set_mode(OUTPUT);
    if (serial)
    {
        return serial->flush();
    }
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

size_t RS485::print(String data)
{
    set_mode(OUTPUT);
    if (serial)
    {
        return serial->print(data);
    }

    return -1;
}

bool RS485::isNewData(void)
{
    return newData;
}

String RS485::getNewData(void)
{
    if (newData)
    {
        newData = false;
        return String(buffer);
    }

    return "";
}

void RS485::receive(void)
{
    static byte index = 0;
    char endMarker = '\n';
    char newChar;

    while (Serial.available() > 0 && newData == false)
    {
        newChar = Serial.read();

        if (newChar != endMarker)
        {
            buffer[index] = newChar;
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
}

// String RS485::receive_rs485(void)
// {
//     set_mode(INPUT);

//     if (!serial)
//     {
//         return "";
//     }

//     // Wait for a message to be received
//     unsigned long start = millis();
//     while (!serial->available())
//     {
//         if (millis() - start > timeout_millis)
//             return new char[0];
//     }

//     if (serial->available())
//     {
//         int length = serial->readBytes(buffer, buffersize);
//         if (length > 0)
//         {
//             // Coppy the data out of the buffer
//             char *data = new char[length + 1];
//             memcpy(data, buffer, length);

//             // Add null termination to make it a string
//             data[length] = '\0';
//             String stringData(data);
//             delete[] data;
//             return stringData;
//         }
//     }

//     return "";
// }
