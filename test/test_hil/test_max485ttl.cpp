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

const long kPerformaceTestAmount = 1000000;
const uint8_t de_port = 2;
const uint8_t re_port = 3;
HardwareSerial *serial = &Serial1;

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
    serial->begin(115200);
    rs = new RS485(de_port, re_port, serial);
};

/**
 * @brief Clean up the test by deleting the used memory and giving it back
 *
 */
void tearDown(void)
{
    rs->~RS485();
    Serial1.end();
};

/**
 * @brief Test for a single byte input and output
 *
 */
void test_Write(void)
{
    char input = 'S';
    rs->write(input);
    rs->flush();
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
void test_Print(void)
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
void test_Buffer(void)
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
        String output = rs->ReadStringBuffer();
        // \n is removed because it is the end marker
        output += "\n";
        TEST_ASSERT_EQUAL_STRING_MESSAGE(input.c_str(), output.c_str(), "String was not put correctly into buffer or retrieved from buffer");
    }
};

/**
 * @brief Test to determine if 2000000 messages could be send and received without error
 *
 */
void test_SendReceive(void)
{
    int number = rand();
    rs->println(number);
    rs->flush();

    rs->WaitForInput();
    if (rs->available())
    {
        int read_count = strtol(rs->readString().c_str(), nullptr, 10);
        if (read_count == 0)
        {
            TEST_FAIL_MESSAGE("No number was received");
        }

        TEST_ASSERT_EQUAL_MESSAGE(number, read_count, "Number was not the same");
    }
    else
    {
        TEST_FAIL_MESSAGE("No message was received");
    }
}

void test_PerformanceTest(void)
{
#ifndef PERFORMANCE_TEST
    TEST_IGNORE_MESSAGE("Ignored performance, to turn on define PERFORMANCE_TEST");
#endif
    unsigned long start_time = millis();

    for (long i = 0; i < kPerformaceTestAmount; i++)
    {
        test_SendReceive();
    }

    unsigned long end_time = millis();
    String output = String("Performance test completed in: ");
    output += (end_time - start_time);
    TEST_MESSAGE(output.c_str());
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

    RUN_TEST(test_Write);
    RUN_TEST(test_Print);
    RUN_TEST(test_Buffer);
    RUN_TEST(test_SendReceive);
    RUN_TEST(test_PerformanceTest);

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