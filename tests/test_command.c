#include "unity.h"
#include "command.h"

void test_command_arg_counting() {
    TEST_ASSERT_EQUAL_INT(3, count_command_args("ls -la build", 0, 12));
    TEST_ASSERT_EQUAL_INT(2, count_command_args("ls -la", 0, 6));
    TEST_ASSERT_EQUAL_INT(1, count_command_args("ls", 0, 2));
}
