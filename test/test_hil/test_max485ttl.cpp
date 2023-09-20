/**
 * @file test_max485ttl.cpp
 * @author rpvos (mr.rv.asd@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-09-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include <unity.h>
#include "max485ttl.h"

#define DE_PORT 2
#define RE_PORT 3

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
    Serial1.begin(115200);
    rs = new RS485(DE_PORT, RE_PORT, &Serial1);
};

/**
 * @brief Clean up the test by deleting the used memory and giving it back
 *
 */
void tearDown(void)
{
    delete rs;
    Serial1.end();
};

/**
 * @brief Test for a single byte input and output
 *
 */
void test_single_byte(void)
{
    char input = 'S';
    rs->write(input);
    rs->flush();
    rs->SetMode(INPUT);
    // Wait for the byte to be sent back
    rs->WaitForInput();

    if (rs->available())
    {
        char output = rs->read();
        TEST_ASSERT_EQUAL_CHAR_MESSAGE(input, output, "Byte not received correctly");
    }
    else
    {
        TEST_FAIL_MESSAGE("No byte in response");
    }
};

/**
 * @brief Test for a string input and output
 *
 */
void test_string(void)
{
    String input = "AAAABBBBCCCCDDDD";
    rs->print(input);
    rs->flush();
    rs->SetMode(INPUT);
    // Wait for the byte to be sent back
    rs->WaitForInput();

    if (rs->available())
    {
        String output = rs->readString();
        TEST_ASSERT_EQUAL_STRING_MESSAGE(input.c_str(), output.c_str(), "Byte not received correctly");
    }
    else
    {
        TEST_FAIL_MESSAGE("No string in response");
    }
};

/**
 * @brief Test for using the read buffer
 *
 */
void test_reading_buffer(void)
{
    String input = "AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJ\n";
    rs->print(input);
    rs->flush();

    int time = millis();
    while (millis() - time < 5000)
    {
        rs->ReadIntoBuffer();
    }

    TEST_ASSERT_TRUE_MESSAGE(rs->IsDataInBuffer(), "Data was not put into buffer");

    if (rs->IsDataInBuffer())
    {
        String output = rs->ReadBuffer();
        // \n is removed because it is the end marker
        output += "\n";
        TEST_ASSERT_EQUAL_STRING_MESSAGE(input.c_str(), output.c_str(), "String was not put correctly into buffer or retrieved from buffer");
    }
};

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

    RUN_TEST(test_single_byte);
    RUN_TEST(test_string);
    RUN_TEST(test_reading_buffer);

    UNITY_END(); // Stop unit testing
};

/**
 * @brief Do nothing after all tests have succeeded
 *
 */
void loop()
{
    delay(2000);
};