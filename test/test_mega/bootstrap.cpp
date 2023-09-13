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
  int RE_PIN = 2;
  int DE_PIN = 3;
  RS485 *rs = new RS485(DE_PIN, RE_PIN);

  rs->set_serial(&Serial);

  rs->set_mode(OUTPUT);

  delay(1);
  TEST_ASSERT_EQUAL(HIGH, digitalRead(DE_PIN));
  TEST_ASSERT_EQUAL(HIGH, digitalRead(RE_PIN));

  String send = "Hello world!\r\n";
  rs->print(send);

  rs->set_mode(INPUT);
  delay(1);
  TEST_ASSERT_EQUAL(LOW, digitalRead(DE_PIN));
  TEST_ASSERT_EQUAL(LOW, digitalRead(RE_PIN));
}

// void test_print(void)
// {
//   TEST_ASSERT_EQUAL_CHAR('A', 'A');

//   Serial1.begin(9600);
//   Serial2.begin(9600);
//   RS485 *receiver = new RS485(2, 3);
//   RS485 *sender = new RS485(4, 5);

//   receiver->set_serial(&Serial1);
//   sender->set_serial(&Serial2);

//   receiver->set_mode(INPUT);
//   sender->set_mode(OUTPUT);

//   String send = "Hello world!\r\n";

//   Serial.println("Before write");
//   sender->print(send);
//   Serial.println("After write");
//   receiver->receive();
//   Serial.println("After receive");

//   String receive = "";
//   if (receiver->isNewData())
//   {
//     String receive = receiver->getNewData();
//   }
//   Serial.println("After get");

//   TEST_ASSERT_EQUAL_STRING(send.c_str(), receive.c_str());
// }

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
