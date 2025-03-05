#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "execution.h"
#include "environment.h"
#include "lang.h"

extern char **environ;

int execute_line(char *line) {
    if (strlen(line) == 0) {
        return 0;
    }

    // Parse the line
    struct lang_node* nodes;
    int node_count;
    if ((node_count = lang_parse_line(&nodes, line)) < 0) {
        fprintf(stderr, "Failed to parse line!\n");
        return 0;
    }

    for (int i = 0; i < node_count; i++) {
        struct command comm;

        if (lang_parse_command(&nodes[i], &comm) < 0) {
            fprintf(stderr, "Failed to parse command!\n");
            return 0;
        }

        if (comm.type == COMMAND_EXEC) {
            if (execute(&comm) < 0) {
                return -1;
            }
        }

        if (comm.type == COMMAND_BUILTIN) {
            // Attempt to resolve a function pointer for the builtin function
            int i = 0;
            char **current = BUILTIN_COMMANDS;
            while (*current != NULL && strcmp(comm.exec, *current) != 0) {
                i++;
                current++;
            }

            int status = BUILTIN_COMMAND_FNS[i](&comm);
            if (status < 0) {
                fprintf(stderr, "Command failed with status code %d\n", status);
            }
        }

        for (int j = 0; j < nodes[i].arg_count; j++) {
            free(nodes[i].args[j]);
        }
    }

    free(nodes);

    return 0;
}

int execute_script(char *script_path) {
    FILE *file = fopen(script_path, "r");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *contents = malloc(sizeof(char) * (file_size + 1));

    if (contents == NULL) {
        fprintf(stderr, "Failed to allocate memory for script contents: %s\n", strerror(errno));
        return -1;
    }

    int count_read;
    if ((count_read = fread(contents, file_size, 1, file)) != 1) {
        fprintf(stderr, "Failed to read script: %s\n", strerror(errno));
        printf("%d\n", count_read);
        free(contents);
        return -1;
    }

    fclose(file);

    char *line = contents;
    size_t line_len = 0;

    // Attempt to execute the script line by line
    while(line < contents + file_size - 1) {
        char *ptr = strstr(line, "\n");
        line_len = ptr - line;

        char *buf = malloc(sizeof(char) * (line_len + 1));

        if (buf == NULL) {
            fprintf(stderr, "Failed to allocate %ld bytes: %s\n", line_len + 1, strerror(errno));
            free(contents);
            return -1;
        }

        bzero(buf, line_len + 1);
        memcpy(buf, line, line_len);

        if (execute_line(buf) < 0) {
            fprintf(stderr, "Failed to execute script!\n");
            free(buf);
            free(contents);
            return -1;
        }

        free(buf);

        line += line_len + 1;
    }

    free(contents);

    return 0;
}

/**
 * Execute the command specified by comm and wait for it to complete
 */
int execute(struct command *comm) {
    // Search the path for the command
    char *path_var = get_envvar(environ, "PATH");
    char *search_result = search_path(path_var, comm->exec);

    char *exec = search_result;
    if (exec == NULL) {
        exec = comm->exec;
    }

    // Actually begin executing a process
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Failed to execute process: %s\n", comm->exec);
        perror("fork");
        return -1;
    }

    // Child process
    if (pid == 0) {
        execve(exec, comm->args, environ);
        fprintf(stderr, "Failed to execute process: %s\n", comm->exec);
        perror("execve");
        exit(EXIT_FAILURE);
    }

    int child_status;
    waitpid(pid, &child_status, 0);

    return 0;
}
