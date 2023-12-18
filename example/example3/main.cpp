#include <Arduino.h>
#include "MAX485TTL.hpp"

#define RS485_DE_PIN 2
#define RS485_RE_PIN 3

RS485 *rs;
uint8_t *buffer = (uint8_t *)"AAAA";

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    rs = new RS485(2, 3, &Serial1);

    rs->SetMode(OUTPUT);
    rs->write(buffer, 5);
    rs->flush();
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
            Serial.print((char)rs->read());
        }

        rs->SetMode(OUTPUT);
        rs->write(buffer, 5);
        rs->flush();
        rs->SetMode(INPUT);
    }

    delay(1000);
}