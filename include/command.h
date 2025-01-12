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
 * An array of strings which lists the available builtin commands
 */
extern char *BUILTIN_COMMANDS[];

/**
 * An array of function pointers which map one-to-one with the BUILTIN_COMMANDS array
 */
extern int (*BUILTIN_COMMAND_FNS[])(struct command*); // This syntax looks so bad...

/**
 * Attempts to parse the command contained in the slice of *src between slice_start and slice_end.
 * The result is stored in comm which must be allocated before any call to this function.
 */
int parse_command(struct command* comm, char **src, size_t slice_start, size_t slice_end);

/**
 * Counts the number of args that the command string str contains
 */
int count_command_args(char *str, size_t slice_start, size_t slice_end);

/**
 * Returns an NULL terminated array of strings representing the command's arguments
 */
char** get_command_args(char *src, size_t slice_start, size_t slice_end);

#endif
