#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "command.h"

int parse_command(struct command* comm, char **src, size_t slice_start, size_t slice_end) {
    // At first, we must assume it's an exec command
    comm->type = COMMAND_EXEC;
    comm->exec = malloc(sizeof(char) * (slice_end - slice_start + 1));

    if (comm->exec < 0) {
        perror("malloc");
        return -1;
    }

    // Look for a space so we can find the program name
    size_t exec_end = slice_end - slice_start;
    char *space = strstr(*src + slice_start, " ");
    if (space != NULL) {
        exec_end = space - (*src + slice_start);
    }

    bzero(comm->exec, slice_end - slice_start + 1);
    memcpy(comm->exec, (*src + slice_start), exec_end - slice_start);

    // Now that we know the program name, we can check if it is built in
    if (strcmp(comm->exec, "cd") == 0) {
        comm->type = COMMAND_BUILTIN;
    }

    // Fill in EXEC command args
    if (comm->type == COMMAND_EXEC) {
        char **command_args = get_command_args(*src, slice_start, slice_end);

        if (command_args == NULL) {
            fprintf(stderr, "Failed to parse command arguments!\n");
            return -1;
        }

        comm->args = command_args;
    }

    return 0;
}

int count_command_args(char *str, size_t slice_start, size_t slice_end) {
    int count = 0;
    char *copy = malloc(sizeof(char) * slice_end - slice_start + 1);

    if (copy == NULL) {
        perror("malloc");
        return -1;
    }

    bzero(copy, slice_end - slice_start + 1);
    memcpy(copy, str + slice_start, slice_end - slice_start + 1);

    char *token = strtok(copy, " ");

    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }

    free(copy);
    return count;
}

char** get_command_args(char *src, size_t slice_start, size_t slice_end) {
    int arg_count = count_command_args(src, slice_start, slice_end);
    char **buf = malloc(sizeof(char*) * arg_count + 1);
    size_t size = slice_end - slice_start + 1;
    char *copy = malloc(sizeof(char) * size);

    if (copy == NULL) {
        perror("malloc");
        return NULL;
    }

    bzero(copy, size);
    memcpy(copy, src + slice_start, size);

    char *token = strtok(copy, " ");

    int i = 0;
    while (token != NULL) {
        buf[i++] = token;
        token = strtok(NULL, " ");
    }
    buf[arg_count] = NULL;

    return buf;
}
