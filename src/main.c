#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"

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

int execute(struct command *comm) {
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Failed to execute process: %s\n", comm->exec);
        perror("fork");
        return -1;
    }

    // Child process
    if (pid == 0) {
        execve(comm->exec, comm->args, environ);
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

        printf("Line read (%zd): %s\n", nread, line);

        // Parse the command
        struct command comm;
        if (parse_command(&comm, &line, 0, nread - 1) < 0) {
            fprintf(stderr, "Failed to parse command!\n");
            exit_status = EXIT_FAILURE;
            break;
        }

        printf("'%s'\n", comm.exec);

        if (comm.type == COMMAND_EXEC) {
            execute(&comm);
        }
    }

    return exit_status;
}
