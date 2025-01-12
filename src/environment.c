#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environment.h"

char* search_dir(char *dir_path, char *query);

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

// TODO: deal with memory leaks
char* search_path(char *path, char *query) {
    char *result, *token;

    // strtok clobbers the input string
    char *path_copy = malloc(sizeof(char) * (strlen(path) + 1));
    if (path_copy == NULL) {
        fprintf(stderr, "Encountered an error while search path!\n");
        perror("malloc");
        return NULL;
    }
    strcpy(path_copy, path);

    token = strtok(path_copy, ":");
    result = NULL;

    while (token != NULL && result == NULL) {
        result = search_dir(token, query);
        token = strtok(NULL, ":");
    }

    return result;
}

char* search_dir(char *dir_path, char *query) {
    DIR *dir = opendir(dir_path);
    char *result = NULL;

    if (dir == NULL) {
        // TODO: add optional verbosity
        // fprintf(stderr, "Failed to search PATH directory '%s'!\n", dir_path);
        // perror("opendir");
        return NULL;
    }

    struct dirent *entry = readdir(dir);
    
    while (entry != NULL) {
        // Ignore anything that isn't a file or symbolic link
        if (entry->d_type != DT_REG && entry->d_type != DT_LNK) {
            entry = readdir(dir);
            continue;
        }

        if (strcmp(entry->d_name, query) == 0) {
            int size = sizeof(char) * (strlen(dir_path) + strlen(entry->d_name) + 2);
            if ((result = malloc(size)) < 0) {
                // Uh oh
                perror("malloc");
                break;
            }

            bzero(result, size);
            strcpy(result, dir_path);
            *(result + strlen(dir_path)) = '/';
            strcpy(result + strlen(dir_path) + 1, entry->d_name);

            break;
        }

        entry = readdir(dir);
    }

    if (closedir(dir) < 0) {
        fprintf(stderr, "Warning, failed to close dir while searching PATH: '%s'!\n", dir_path);
    }

    return result;
}
