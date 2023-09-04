
#include <Arduino.h>
#include "MAX485TTL.h"
#include <unity.h>

// TODO test for diffrent baudrates
#define BAUDRATE 9600

void setUp(void)
{
    Serial1.begin(BAUDRATE);
    Serial2.begin(BAUDRATE);
}

void tearDown(void)
{
    Serial1.end();
    Serial2.end();
}

void test_write_string(void)
{
    // Construct the RS485 modules
    RS485 *rs485_serial1 = new RS485(8, 9, &Serial1);
    RS485 *rs485_serial2 = new RS485(10, 11, &Serial2);

    // Set serial 1 as output and 2 as input
    rs485_serial1->set_mode(false);
    rs485_serial2->set_mode(true);

    // Data that will be send
    char const *data = "TEST";

    // Send via serial 1
    rs485_serial1->write(data);
    // Receive via serial 2
    char *received_data = rs485_serial2->receive_rs485();

    // Assert the received data
    TEST_ASSERT_EQUAL(data, received_data);
}

void test_print(void)
{
    TEST_IGNORE_MESSAGE("This test needs to be implemented");
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_write_string);
    RUN_TEST(test_print);
    return UNITY_END();
}

void setup()
{
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop()
{
    delay(1000);
}
