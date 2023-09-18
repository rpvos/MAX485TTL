#include <Arduino.h>
#include <unity.h>
#include "max485ttl.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_print(void)
{
  // TODO add test for print
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN(); // IMPORTANT LINE!

  RUN_TEST(test_print);

  UNITY_END(); // stop unit testing
}

void loop()
{
  delay(2000);
}
