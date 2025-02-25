#include "unity.h"
#include "command.h"
#include <stdlib.h>

void test_parse_command() {
    struct command comm;
    char *src = "ls -la build";
    int result = parse_command(&comm, &src, 0, 12);
    char *expected_args[4] = {"ls", "-la", "build", NULL};

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(COMMAND_EXEC, comm.type);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expected_args, comm.args, 4);
}

void test_parse_builtin_command() {
    struct command comm;
    char *src = "cd build";
    int result = parse_command(&comm, &src, 0, 8);
    char *expected_args[3] = {"cd", "build", NULL};

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(COMMAND_BUILTIN, comm.type);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expected_args, comm.args, 3);
}

void test_command_arg_counting() {
    TEST_ASSERT_EQUAL_INT(3, count_command_args("ls -la build", 0, 12));
    TEST_ASSERT_EQUAL_INT(2, count_command_args("ls -la", 0, 6));
    TEST_ASSERT_EQUAL_INT(1, count_command_args("ls", 0, 2));
}

void test_get_command_args() {
    char **result = get_command_args("ls -la build", 0, 12);
    char *expected[3] = {"ls", "-la", "build"};

    TEST_ASSERT_NOT_EQUAL(NULL, result);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expected, result, 3);

    free(result);
}
