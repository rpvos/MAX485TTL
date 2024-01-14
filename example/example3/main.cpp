#include <Arduino.h>
#include "MAX485TTL.hpp"

#define RS485_DE_PIN 2
#define RS485_RE_PIN 3

RS485 *rs;
const int string_length = 12;
char buffer[string_length + 1] = "Hello World!";

void send_message()
{
    rs->SetMode(OUTPUT);
    rs->write(buffer, string_length);
    rs->flush();
    rs->SetMode(INPUT);
}

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600, SERIAL_8N1);
    rs = new RS485(2, 3, &Serial1);
    rs->SetMode(INPUT);

    send_message();
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
            Serial.print(' ');
        }
        Serial.println();

        send_message();
    }
    delay(100);
}