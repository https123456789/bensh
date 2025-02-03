#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "environment.h"
#include "lang.h"

extern char **environ;

/**
 * Read a single line from standard input
 */
int read_line(char **line) {
    ssize_t nread;
    size_t len = 0;

    if ((nread = getline(line, &len, stdin)) < 0) {
        return -1;
    }

    // Strip the newline off the end that getline always reads
    *(*line + nread - 1) = 0;

    return nread;
}

/**
 * Print the shell prompt
 */
void print_prompt() {
    printf("-> ");
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

int main() {
    char *line = NULL;
    char done = 0;
    int exit_status = EXIT_SUCCESS;

    while (!done) {
        print_prompt();

        ssize_t nread = read_line(&line);

        // Handle reading errors
        if (nread < 0 && errno != 0) {
           exit_status = EXIT_FAILURE;
            break;
        }

        // Handle EOF on stdin by simply leaving the loop
        if (nread < 0) {
            break;
        }

        // Ignore blank lines
        if (strlen(line) == 0) {
            continue;
        }

        // Parse the line
        struct lang_token* tokens;
        if (lang_parse_line(&tokens, line) < 0) {
            fprintf(stderr, "Failed to parse line!\n");
            exit_status = EXIT_FAILURE;
            break;
        }

        // Parse the command
        struct command comm;
        if (parse_command(&comm, &line, 0, nread - 1) < 0) {
            fprintf(stderr, "Failed to parse command!\n");
            exit_status = EXIT_FAILURE;
            break;
        }

        if (comm.type == COMMAND_EXEC) {
            execute(&comm);
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
    }

    return exit_status;
}
