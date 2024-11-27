/* gopipe.c

 * Execute up to four instructions, piping the output of each into the
 * input of the next.

 * Author: Ranga Sashank Gurram
 */


#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

#define MAX_COMMAND_LENGTH 80
#define MAX_COMMANDS 4
#define TOKENS 8



void do_logic(char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH], int num_commands) 
{
    
    //setting up pipes
    int fds[num_commands - 1][2];
    
    for(int i = 0; i < num_commands-1; i++) {
        pipe(fds[i]);
    }
    
    int x = 0;
    while(x < num_commands) {
        char *cmd[TOKENS];
        int count = 0;
        char *token = strtok(commands[x], " ");
        
        while(token!= NULL) {
            cmd[count] = token;
            token = strtok(NULL, " ");
            count ++;
            
        
        }
        
        cmd[count] = NULL;
        int pid = fork();
        
        if(pid == 0) {
            if(x!=0) {
                dup2(fds[x - 1][0], 0);
            }
            
            if(x < num_commands - 1) {
                dup2(fds[x][1], 1);
            }
            
            for(int i = 0; i < num_commands - 1; i++){
                close(fds[i][0]);
                close(fds[i][1]);
            }
            
            execve(cmd[0], cmd, NULL);
            exit(1);
        }
        
        else{
            if(x!=0) {
                close(fds[x-1][0]);
                close(fds[x-1][1]);
            }
        }
        waitpid(-1, NULL, 0);
        x++;
    }

} 



int main()
{
    char command[MAX_COMMAND_LENGTH] = "";
    char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];
    int num_commands = 0;
    int count = 1;

    while (count <= MAX_COMMANDS)
    {
        
        if (read(0, command, MAX_COMMAND_LENGTH) > 1)
        {
            command[strlen(command) - 1] = '\0';
            strcpy(commands[num_commands], command);
            num_commands++;
            
        }
        else
        {
            break;
        }
        count++;
    }

    do_logic(commands, num_commands);

    return 0;
}

