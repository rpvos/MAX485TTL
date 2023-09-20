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

    output = "";
}

void loop()
{
    while (Serial1.available())
    {
        output += Serial1.readString();
        delay(10);
    }

    if (output.length())
    {
        rs->SetMode(OUTPUT);
        rs->print(output);
        Serial.print(output);
        rs->flush();
        rs->SetMode(INPUT);
        output = "";
    }
}