#include <Arduino.h>
#include "MAX485TTL.hpp"

#define RS485_DE_PIN 2
#define RS485_RE_PIN 3

RS485 *rs;
uint8_t buffer[8] = {0x1, 0x3, 0x0, 0x16, 0x0, 0x7, 0xE5, 0xCC};

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600, SERIAL_8N1);
    rs = new RS485(2, 3, &Serial1);
    rs->SetMode(INPUT);
}

void loop()
{
    rs->WaitForInput();
    while (rs->available())
    {
        int length = rs->available();
        for (int i = 0; i < length; i++)
        {
            Serial.print((char)rs->read(), HEX);
            Serial.print(' ');
        }
        Serial.println();

        rs->SetMode(OUTPUT);
        rs->write(buffer, 5);
        rs->flush();
        rs->SetMode(INPUT);
    }

    delay(1000);
}