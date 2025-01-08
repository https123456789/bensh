#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "command.h"

int parse_command(struct command* comm, char **src, size_t slice_start, size_t slice_end) {
    // At first, we must assume it's an exec command
    comm->type = COMMAND_EXEC;

    if ((comm->exec = malloc(sizeof(char) * slice_end - slice_start)) < 0) {
        perror("malloc");
        return -1;
    }

    // Look for a space so we can find the program name
    size_t exec_end = slice_end - slice_start;
    char *space = strstr(*src + slice_start, " ");
    if (space != NULL) {
        exec_end = space - (*src + slice_start);
    }

    memcpy(comm->exec, (*src + slice_start), exec_end - slice_start);

    // Now that we know the program name, we can check if it is built in
    if (strcmp(comm->exec, "cd") == 0) {
        comm->type = COMMAND_BUILTIN;
    }

    return 0;
}
