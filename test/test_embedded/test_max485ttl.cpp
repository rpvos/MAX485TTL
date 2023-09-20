/**
 * @file test_max485ttl.cpp
 * @author rpvos (mr.rv.asd@gmail.com)
 * @brief Unit tests for the max485ttl.cpp
 * @version 0.1
 * @date 2023-09-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include <unity.h>
#include "max485ttl.h"
#include "memory_stream.h"

#define DE_PORT 2
#define RE_PORT 3
#define DE_MEASURING_PORT 4
#define RE_MEASURING_PORT 5

/**
 * @brief Stream used as mock for serial stream
 *
 */
MemoryStream *stream;

/**
 * @brief The object that is being tested
 *
 */
RS485 *rs;

/**
 * @brief Set the test up by creating a new instance of both the mock stream and the MAX485TTL module
 *
 */
void setUp(void)
{
  stream = new MemoryStream();
  rs = new RS485(DE_PORT, RE_PORT, stream);

  pinMode(DE_MEASURING_PORT, INPUT);
  pinMode(RE_MEASURING_PORT, INPUT);
}

/**
 * @brief Clean up the test by deleting the used memory and giving it back
 *
 */
void tearDown(void)
{
  delete rs;
  delete stream;
}

/**
 * @brief Helper function used to determine if the pins are set high
 *
 * @param value LOW or HIGH
 * @return true if both are set to value
 * @return false if on or both is not set to value
 */
bool isSet(int value)
{
  int re_port = digitalRead(RE_MEASURING_PORT);
  int de_port = digitalRead(RE_MEASURING_PORT);

  return (re_port == value) && (de_port == value);
}

/**
 * @brief Testing the port values of the MAX485TTL module using the set_mode() function
 *
 */
void test_ports()
{
  // Check for initialisation as input
  TEST_ASSERT_TRUE_MESSAGE(isSet(LOW), "Pins are not set to LOW on initialisation");

  // Check for set to output
  rs->SetMode(OUTPUT);
  TEST_ASSERT_TRUE_MESSAGE(isSet(HIGH), "Pins are not set to HIGH");

  // Check for set to input
  rs->SetMode(INPUT);
  TEST_ASSERT_TRUE_MESSAGE(isSet(LOW), "Pins are not set to LOW");

  // Check for repeated set
  rs->SetMode(INPUT);
  TEST_ASSERT_TRUE_MESSAGE(isSet(LOW), "Pins are not set to LOW after repeated set command");

  rs->SetMode(2);
  TEST_ASSERT_TRUE_MESSAGE(isSet(LOW), "Pins are changed after given wrong command");
}

/**
 * @brief Testing the write() function
 *
 */
void test_write(void)
{
  char input = 'A';
  TEST_ASSERT_TRUE_MESSAGE(isSet(LOW), "Initial state is not receiving");
  rs->write(input);
  TEST_ASSERT_TRUE_MESSAGE(isSet(HIGH), "State is not changed for sending data");
  rs->SetMode(INPUT);

  char output;
  if (rs->available())
  {
    TEST_ASSERT_TRUE_MESSAGE(isSet(LOW), "State is not changed to receiving data");
    output = rs->read();
  }
  TEST_ASSERT_EQUAL_CHAR_MESSAGE(input, output, "Byte is not written correctly");
}

/**
 * @brief Testing the print function
 *
 */
void test_print(void)
{
  String input = "Hello world!";
  rs->print(input);
  TEST_ASSERT_TRUE_MESSAGE(isSet(HIGH), "State is not changed for sending data");
  rs->SetMode(INPUT);

  String output;
  if (rs->available())
  {
    TEST_ASSERT_TRUE_MESSAGE(isSet(LOW), "State is not changed to receiving data");
    output = rs->readString();
  }
  TEST_ASSERT_EQUAL_STRING_MESSAGE(input.c_str(), output.c_str(), "String is not written correctly");
}

/**
 * @brief Testing the peek method
 *
 */
void test_peek(void)
{
  String input = "Hello world!";
  rs->print(input);

  char c = (char)rs->peek();

  TEST_ASSERT_EQUAL_CHAR_MESSAGE(input.c_str()[0], c, "Peek did not retrieve correct character");

  char cAgain = (char)rs->peek();

  TEST_ASSERT_EQUAL_CHAR_MESSAGE(c, cAgain, "Peek did not retrieve same character twice");
}

void test_read(void)
{
  String input = "Hello world!";
  rs->print(input);

  char c = (char)rs->read();

  TEST_ASSERT_EQUAL_CHAR_MESSAGE(input.c_str()[0], c, "Read did not retrieve first character correctly");

  char c2 = (char)rs->read();

  TEST_ASSERT_EQUAL_CHAR_MESSAGE(input.c_str()[1], c2, "Read did not retrieve seccond character correctly");
}

/**
 * @brief Function used to test available
 *
 */
void test_available(void)
{
  String input = "Hello world!";

  // Available should return 0 because nothing is in the read buffer
  TEST_ASSERT_FALSE(rs->available());

  rs->print(input);

  // Available should return amount of bytes in the read buffer
  TEST_ASSERT_EQUAL(input.length(), rs->available());

  char c = rs->read();

  // Available should return length -1 because one char is read
  TEST_ASSERT_EQUAL(input.length() - 1, rs->available());

  String s = rs->readString();

  // Available should return 0 because all chars are read
  TEST_ASSERT_EQUAL(0, rs->available());
}

/**
 * @brief Entry point to start all tests
 *
 */
void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN(); // Start unit testing

  RUN_TEST(test_ports);
  RUN_TEST(test_write);
  RUN_TEST(test_print);
  RUN_TEST(test_peek);
  RUN_TEST(test_read);
  RUN_TEST(test_available);

  UNITY_END(); // Stop unit testing
}

/**
 * @brief Do nothing after all tests have succeeded
 *
 */
void loop()
{
  delay(2000);
}
