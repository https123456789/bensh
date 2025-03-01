#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "execution.h"
#include "sigdefs.h"

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

int main(int argc, char **argv) {
    char *line = NULL;
    char done = 0;
    int exit_status = EXIT_SUCCESS;

    if (init_signal_handlers() < 0) {
        fprintf(stderr, "Failed to initialize signal handlers: %s\n", strerror(errno));
        exit_status = EXIT_FAILURE;
        return exit_status;
    }

    if (argc > 1) {
        if (execute_script(argv[1]) < 0) {
            exit_status = EXIT_FAILURE;
        }

        return exit_status;
    }

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

        int status;
        if ((status = execute_line(line)) < 0) {
            exit_status = EXIT_FAILURE;
            break;
        }
    }

    return exit_status;
}
