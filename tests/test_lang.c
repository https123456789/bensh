#include "command.h"
#include "unity.h"
#include "lang.h"

void test_parse_line_works() {
    struct lang_node *nodes;
    int result = lang_parse_line(&nodes, "ls -la build");

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(2, nodes[0].arg_count);
    TEST_ASSERT_EQUAL_STRING("ls", nodes[0].executable);
    TEST_ASSERT_EQUAL_STRING("ls", nodes[0].args[0]);
    TEST_ASSERT_EQUAL_STRING("-la", nodes[0].args[1]);
    TEST_ASSERT_EQUAL_STRING("build", nodes[0].args[2]);
    TEST_ASSERT_EQUAL_STRING(NULL, nodes[0].args[3]);
}

void test_node_to_command() {
    struct lang_node *nodes;
    int result = lang_parse_line(&nodes, "ls -la build");

    TEST_ASSERT_EQUAL_INT(1, result);

    struct command comm;
    char* expected_args[4] = {"ls", "-la", "build", NULL};
    result = lang_parse_command(&nodes[0], &comm);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(COMMAND_EXEC, comm.type);
    TEST_ASSERT_EQUAL_STRING("ls", comm.exec);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expected_args, comm.args, 4);
}
