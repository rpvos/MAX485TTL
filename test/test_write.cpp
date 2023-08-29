#include "unity.h"
#include "MAX485TTL.h"

RS485 *rs485;

void setUp(void)
{
    rs485 = new RS485(11, 12, &Serial2);
}

void tearDown(void)
{
    rs485 = 0;
}

void test_write_string(void)
{
    char *data = "TEST";
    rs485->write(data);
    String returned_data = rs485->receive_rs485();

    TEST_ASSERT_EQUAL(data, returned_data.c_str());
}

void test_print(void)
{
    rs485->print(0xFF, HEX);
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
