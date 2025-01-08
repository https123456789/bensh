#ifndef _BENSH_COMMAND_H_
#define _BENSH_COMMAND_H_

#include <stdlib.h>

enum command_type {
    COMMAND_NONE,
    COMMAND_EXEC,
    COMMAND_BUILTIN
};

struct command {
    enum command_type type;
    char *exec;
    char **args; // All args except the first should be freed
};

/**
 * Attempts to parse the command contained in the slice of *src between slice_start and slice_end.
 * The result is stored in comm which must be allocated before any call to this function.
 */
int parse_command(struct command* comm, char **src, size_t slice_start, size_t slice_end);

#endif
