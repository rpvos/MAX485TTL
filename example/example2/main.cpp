#include <Arduino.h>
#include <MAX485TTL.h>

#define RS485_DE_PIN 2
#define RS485_RE_PIN 3

RS485 *rs;
String output;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200);
    rs = new RS485(2, 3, &Serial1);
}

void loop()
{
    rs->WaitForInput();
    if (rs->available())
    {
        String s = rs->readString();
        rs->print(s);
    }
}