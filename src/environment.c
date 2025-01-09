#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environment.h"

char* get_envvar(char **env, char *target) {
    char **current = env;
    char *location = NULL;

    // Environ should be a null terminal array
    while (*current != NULL && (current == env || location != *(current - 1))) {
        location = strstr(*current, target);
        current++;
    }

    if (location == NULL) {
        fprintf(stderr, "Failed to find an env entry for '%s'\n", target);
        return NULL;
    }

    return location + strlen(target) + 1; // Return only the value of the env var
}

char* search_path(char *path, char *query) {
    return NULL;
}
