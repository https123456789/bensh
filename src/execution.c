#include "execution.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        printf("Line: '%s'\n", buf);

        free(buf);

        line += line_len + 1;
    }

    free(contents);

    return -1;
}
