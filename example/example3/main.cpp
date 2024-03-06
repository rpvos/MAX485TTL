#include <Arduino.h>
#include "MAX485TTL.hpp"

#define RS485_DE_PIN 2
#define RS485_RE_PIN 2

RS485 *rs;
char s[] = "Hello World!";
const int string_length = sizeof(s) / sizeof(s[0]);

void send_message()
{
    rs->SetMode(OUTPUT);
    rs->write(s, string_length);
    rs->flush();
    rs->SetMode(INPUT);
}

void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200, SERIAL_8N1);
    rs = new RS485(2, 2, &Serial1);
    rs->SetMode(INPUT);

    send_message();
}

const size_t buffer_size = 128;
uint8_t buffer[buffer_size];
size_t cursor = 0;

void loop()
{
    rs->WaitForInput();
    while (rs->available())
    {
        delay(20);
        int length = rs->available();
        for (int i = 0; i < length; i++)
        {
            uint32_t c = rs->read();
            if (c)
            {
                buffer[i] = (uint8_t)c;
            }
        }
        cursor += length;
    }

    if (cursor)
    {
        Serial.print('{');
        for (size_t i = 0; i < cursor; i++)
        {
            Serial.print("0x");
            Serial.print(buffer[i], HEX);
            if (i < cursor - 1)
            {
                Serial.print(',');
            }
        }
        Serial.println('}');
        cursor = 0;

        // rs->SetMode(OUTPUT);
        // delay(100);
        // rs->write(buffer, length);
        // rs->flush();
        // rs->SetMode(INPUT);
    }

    delay(10);
}