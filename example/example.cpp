#include <Arduino.h>
#include "max485ttl.h"

RS485 *rs;

void setup()
{
    rs = new RS485(2, 3);

    Serial1.begin(115200);
    rs->set_serial(&Serial1);
}

void loop()
{

    rs->print("AAAABBBBCCCC\n");

    if (rs->is_new_data())
    {
        const char *s = rs->get_new_data();
        rs->print(s);
    }

    rs->update();
    delay(1000);
}