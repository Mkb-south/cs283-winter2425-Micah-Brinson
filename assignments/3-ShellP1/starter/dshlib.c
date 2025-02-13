#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    command_t *user_input = malloc(sizeof(command_t));

    if (cmd_line == NULL || *cmd_line == '\0' ||*cmd_line == '\n') {
        return WARN_NO_CMDS;
    }
        char exe[EXE_MAX] = {0};
        char args[ARG_MAX] = {0};
        int i = 0, j = 0;


        while (*cmd_line == ' ') {
            cmd_line++;
        }

        char *end = cmd_line + strlen(cmd_line) - 1;
        while (end > cmd_line && *end == ' ') {
            end--;
        }
        *(end + 1) = '\0';

    char *command = strtok(cmd_line, PIPE_STRING);  
\

    while (command != NULL) {
        while (*command == ' ') command++;

        memset(exe, 0, sizeof(exe));
        memset(args, 0, sizeof(args));
        i = 0;
        while (command[i] != SPACE_CHAR && command[i] != '\0') {
            if (i >= EXE_MAX - 1) {  
                printf("Command too long\n");
                free(user_input);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            exe[i] = command[i];
            i++;
        }
        exe[i] = '\0'; 
        strcpy(user_input->exe, exe);


        while (command[i] == SPACE_CHAR) {
            i++;
        }

        j=0;
        while (command[i] != '\0') {
            if (j >= ARG_MAX - 1) {  
                free(user_input);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            args[j++] = command[i++];
        }
        
        args[j] = '\0'; 
        
        strcpy(user_input->args, args);
        if (clist->num < CMD_MAX) {
            clist->commands[clist->num] = *user_input;  
            clist->num++;
        } else {
            free(user_input);
            return ERR_TOO_MANY_COMMANDS;
        }
        command = strtok(NULL, PIPE_STRING);
    }
    printf(CMD_OK_HEADER, clist->num);

    for (int k = 0; k < clist->num; k++) {
        if (strcmp(clist->commands[k].args, "") == 0) {
            printf("<%d> %s\n", k + 1, clist->commands[k].exe);
        } else {
            printf("<%d> %s [%s]\n", k + 1, clist->commands[k].exe, clist->commands[k].args);
        }
    }
    free(user_input);
    return OK;
}