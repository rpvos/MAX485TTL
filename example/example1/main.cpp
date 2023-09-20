#include <Arduino.h>
#include "max485ttl.h"

RS485 *rs;
int correct = 0;
int wrong = 0;

void setup()
{
    rs = new RS485(2, 3, &Serial1);

    Serial1.begin(115200);
}

void loop()
{
    String input = "AAAABBBBCCCC\n";
    rs->print(input);
    rs->SetMode(INPUT);
    delay(1000);

    if (rs->available())
    {
        String output = rs->readString();
        if (input == output)
        {
            correct++;
        }
        else
        {
            wrong++;
        }
    }

    if (correct + wrong % 50 == 0)
    {
        Serial.print("Attempts:");
        Serial.print(correct + wrong);
        Serial.print("\nCorrect:");
        Serial.print(correct);
        Serial.print("\tWrong:");
        Serial.println(wrong);
    }
}