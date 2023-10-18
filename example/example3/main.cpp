#include <Arduino.h>
#include <MAX485TTL.h>

#define RS485_DE_PIN 2
#define RS485_RE_PIN 3

RS485 *rs;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    rs = new RS485(2, 3, &Serial1);
}

void loop()
{
    rs->SetMode(OUTPUT);
    rs->print("AAAA");
    rs->flush();
    rs->WaitForInput(10);
    if (rs->available())
    {
        Serial.println(rs->readString());
    }
}