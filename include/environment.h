#ifndef _BENSH_ENVIRONMENT_H_
#define _BENSH_ENVIRONMENT_H_

/**
 * Searches through a list of strings for the environment variable indicated by target
 *
 * Note: that env should be a null terminated array of char pointers
 * Note: this function returns a pointer inside env
 */
char* get_envvar(char **env, char *target);

/**
 * Searches through the provided path string in an attempt to resolve the specified query
 */
char* search_path(char *path, char *query);

#endif
