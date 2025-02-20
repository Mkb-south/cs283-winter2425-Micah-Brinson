#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX);
    int rc = 0;
    cmd_buff_t *cmd = malloc(sizeof(cmd_buff_t));

    if (cmd == NULL){
        return ERR_MEMORY;
    }

    // TODO IMPLEMENT MAIN LOOP

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';         
    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        rc = build_cmd_buff(cmd_buff, cmd);

        if (rc == OK) {
        } else if (rc == WARN_NO_CMDS) {
            printf("%s", CMD_WARN_NO_CMD);
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, 8);
        } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            printf("Error: Command or arguments too big.\n");
        } else {
            printf("Error occurred\n");
        } 
    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing
    if (exec_built_in_cmd(cmd) == BI_NOT_BI) {
        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
        pid_t pid = fork();
        
        if (pid == 0) {
            if (execvp(cmd->argv[0], cmd->argv) == -1) {
                exit(1);
            }
        } else {
            int child_stat;
            waitpid(pid, &child_stat, 0); 
        }
    }
    clear_cmd_buff(cmd);
}
free_cmd_buff(cmd);
    return OK;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (cmd_line == NULL || *cmd_line == '\0' || *cmd_line == '\n') {
        return WARN_NO_CMDS;
    }

    cmd_buff->_cmd_buffer = cmd_line;
    cmd_buff->argc = 0;
    while (*cmd_line == ' ') cmd_line++;  

    char *ptr = cmd_line;
    bool inQuotes = false;
    char quote = '\0';

    while (*ptr) {
        while (*ptr == ' ') ptr++;  

        if (*ptr == '\0') break;

        if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
            printf("Error: Too many arguments.\n");
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        if (*ptr == '"' || *ptr == '\'') {  
            inQuotes = true;
            quote = *ptr;
            ptr++;  
        }
        cmd_buff->argv[cmd_buff->argc++] = ptr;  



        while (*ptr && (inQuotes || *ptr != ' ')) {
            if (inQuotes && *ptr == quote) {
                ptr++;  
                break;
            }
            ptr++;
        }

        if (!inQuotes && *ptr != '\0') {
            *ptr = '\0';  
            ptr++;
        }
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;  

    for (int i = 0; i < cmd_buff->argc; i++) {
        char *arg = cmd_buff->argv[i];
        if (arg[0] == '"' || arg[0] == '\'') {
            memmove(arg, arg + 1, strlen(arg));  
        }
        char *end_arg = arg + strlen(arg) - 1;




        while (end_arg > arg && *end_arg == ' ') end_arg--;  
        if (*end_arg == '"' || *end_arg == '\'') {
            *end_arg = '\0';  
        }

    }

    return OK;
}




Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "dragon") == 0) {
        return BI_CMD_DRAGON;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    }
    return BI_NOT_BI; 
}


Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (cmd == NULL) {
        return BI_NOT_BI;
    }

    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);  

    switch (cmd_type) {
        case BI_CMD_EXIT:
            free_cmd_buff(cmd);
            exit(0);  
            break;

        case BI_CMD_DRAGON:
            break;

        case BI_CMD_CD:
            if (cmd->argc < 1) {
                break;
            } else {
                if (cmd->argc >= 2) {
                    if (chdir(cmd->argv[1]) != 0) {
                        perror("cd");
                    }
                } else {
                }
            }
            break;

        case BI_NOT_BI:
            return BI_NOT_BI;

        default:
            break;
    }

    return cmd_type;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff != NULL) {
        free(cmd_buff->_cmd_buffer);  
        free(cmd_buff); 
    }
    return 0;  
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;  
    }
    return 0;
}