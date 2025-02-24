#include <errno.h>
#include <pcq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lang.h"

/**
 * Tokenize the provided line and store the resulting ast into `result`
 */
static int lex_line(char *src, pcq_result_t *result) {
    pcq_parser_t *Start = pcq_new("start");
    pcq_parser_t *End = pcq_new("end");
    pcq_parser_t *Next = pcq_new("next");
    pcq_parser_t *Executable = pcq_new("executable");
    pcq_parser_t *Arg = pcq_new("arg");
    pcq_parser_t *Command = pcq_new("command");
    pcq_parser_t *Line = pcq_new("line");

    pcqa_lang(PCQA_LANG_DEFAULT,
        " start: /^/; "
        " end: /$/; "
        " next: ';'; "
        " executable: /[a-zA-Z0-9\\/]+/; "
        " arg: /[\\.\\-a-zA-Z0-9\\/]+/; "
        " command: <executable> <arg>*; "
        " line: <start> (<command> <next>?)+ <end>; ",
        Start, End, Next, Executable, Arg, Command, Line, NULL);

    if (!pcq_parse("input", src, Line, result)) {
        pcq_err_print(result->error);
        pcq_err_delete(result->error);
        pcq_cleanup(6, Start, End, Next, Arg, Executable, Command, Line);
        return -1;
    }

    pcq_cleanup(6, Start, End, Next, Arg, Executable, Command, Line);

    return 0;
}

/**
 * Convert the command specified by `ast` into a lang node stored in `node`
 *
 * There are two possible variants of the root command node. For commands that have arguments,
 * the structure looks something like:
 * command|>
 *  executable|regex
 *  arg|regex
 * 
 * However, for commands with no arguments, the ast node is the entire command. I.e.
 * command|executable|regex
 */
static int ast_to_lang_node(struct lang_node *node, struct pcq_ast_t *ast) {
    node->type = NODE_COMMAND;
    
    char has_args = strstr(ast->tag, "executable") == NULL;
    char *exec_src;

    if (has_args) {
        if (ast->children_num < 1) {
            fprintf(stderr, "Invalid ast node!\n");
            return -1;
        }

        node->arg_count = ast->children_num - 1;

        if (strstr(ast->children[0]->tag, "executable") == NULL) {
            fprintf(stderr, "Failed to find executable for command with args!\n");
            return -1;
        }
        exec_src = ast->children[0]->contents;
    } else {
        node->arg_count = 0;

        // Since there are no args, the executable is contained in the command ast contents
        exec_src = ast->contents;
    }

    int size = strlen(exec_src) + 1;
    if ((node->executable = malloc(sizeof(char) * size)) < 0) {
        fprintf(
            stderr,
            "Failed to allocate memory for node executable field: %s\n",
            strerror(errno)
        );
        return -1;
    }
    memset(node->executable, 0, size);
    strcpy(node->executable, exec_src);

    if ((node->args = malloc(sizeof(char*) * (node->arg_count + 2))) < 0) {
        fprintf(
            stderr,
            "Failed to allocate memory for node args list: %s\n",
            strerror(errno)
        );
        free(node->executable);
        return -1;
    }

    // First and last args are special for execve
    node->args[0] = node->executable;
    node->args[node->arg_count + 1] = NULL;

    for (int i = 0; i < node->arg_count; i++) {
        int size = strlen(ast->children[i + 1]->contents) + 1;
        if ((node->args[i + 1] = malloc(sizeof(char) * size)) < 0) {
            fprintf(
                stderr,
                "Failed to allocate memory for node args field: %s\n",
                strerror(errno)
            );

            // TODO: implement memory cleanup
            return -1;
        }

        memset(node->args[i + 1], 0, size);
        strcpy(node->args[i + 1], ast->children[i + 1]->contents);
    }

    return 0;
}

/**
 * Parses the provided line into the given node buffer. The caller is responsible for freeing the
 * node buffer when they are done
 */
int lang_parse_line(struct lang_node** node_buf, char* src) {
    pcq_result_t r;

    if (lex_line(src, &r) < 0) {
        fprintf(stderr, "Failed to lex line!\n");
        return -1;
    }

    pcq_ast_t *ast = (pcq_ast_t*) r.output;

    int command_count = 0;
    for (int i = 0; i < ast->children_num; i++) {
        command_count += strstr(ast->children[i]->tag, "command") != NULL;
    }

    *node_buf = malloc(sizeof(struct lang_node) * command_count);
    if (*node_buf == NULL) {
        fprintf(stderr, "Failed to allocate lang node buffer!: %s\n", strerror(errno));
        pcq_ast_delete(r.output);
        return -1;
    }
    
    for (int i = 0; i < command_count; i++) {
        if (ast_to_lang_node(*node_buf + i, ast->children[i * 2 + 1]) < 0) {
            fprintf(stderr, "Failed to convert pcq ast to a lang node!\n");
            free(*node_buf);
            pcq_ast_delete(r.output);
            return -1;
        }
    }

    return command_count;
}

/**
 * Parse the provided lang node into the provided command. Note that this function does not
 * perform an allocations so `node` must not be freed before `comm`.
 */
int lang_parse_command(struct lang_node *node, struct command *comm) {
    comm->exec = node->executable;
    comm->args = node->args;

    comm->type = COMMAND_EXEC;

    char **builtin_name = BUILTIN_COMMANDS;
    while (*builtin_name != NULL && strcmp(comm->exec, *builtin_name) != 0) {
        builtin_name++;
    }
    if (*builtin_name != NULL) {
        comm->type = COMMAND_BUILTIN;
    }

    return 0;
}
