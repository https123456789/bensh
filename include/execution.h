#ifndef _BENSH_EXECUTION_H_
#define _BENSH_EXECUTION_H_

#include "command.h"

int execute_script(char *script_path);
int execute_line(char *line);
int execute(struct command *comm);

#endif
