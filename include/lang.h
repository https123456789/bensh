#ifndef _BENSH_LANG_H_
#define _BENSH_LANG_H_

#include "command.h"

enum lang_node_type {
    NODE_NONE = 0, // So that zeroed out memory counts as NODE_NONE
    NODE_COMMAND,
};

struct lang_node {
    enum lang_node_type type;
    int arg_count;
    char *executable;
    char **args;
};

int lang_parse_line(struct lang_node** node_buf, char* src);
int lang_parse_command(struct lang_node *node, struct command *comm);

#endif
