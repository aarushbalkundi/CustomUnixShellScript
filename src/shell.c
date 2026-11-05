// #include "shell.h"
#include "../include/shell.h"


void printPrompt()
{
    printf("-->");
    fflush(stdout);  
}

char *readLine()
{
    char *line = malloc(CHAR_LIMIT);
    if (line == NULL) {
        perror("myshell: malloc");
        exit(1);
    }

    if (fgets(line, CHAR_LIMIT, stdin) == NULL) {
        free(line);
        return NULL;          
    }

    line[strcspn(line, "\n")] = '\0';   // remove the \n character at the end of the input
    return line;
}



char **parseLine(char *line)
{
    char *lineCopy = malloc(strlen(line) + 1);
    strcpy(lineCopy, line);

    char *token = strtok(lineCopy, " ");

    char **args = malloc(sizeof(lineCopy) * ARG_LIMIT);
    

    
    int numArgs = 0;
    while (numArgs < ARG_LIMIT && token != NULL)
    {
        args[numArgs] = malloc(strlen(token)+1);
        
        if (args[numArgs] == NULL)
        {
            free(args);
        }
        strcpy(args[numArgs], token);
        // printf("arg%d = %s\n", numArgs, args[numArgs]);
        token = strtok(NULL, " ");
        numArgs ++;
    }

    free(lineCopy);
    
    return args;
}



int checkBuiltIn(const char *command)
{
    const char *builtins[] = {"exit", "cd", "pwd", NULL};

    for (int i = 0; i<3; i++)
    {
        if (strcmp(builtins[i], command) == 0)
        {
            return 1;
        }
    } 
    return 0;
}


//need to implement later
int checkGlob(char **args)
{
    return 0;
}

void executeExternal(char **args)
{
    if (!checkGlob(args))
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Failed to fork");
        }

        if (pid == 0)
        {
            //child process
            execvp(args[0], args);
        }
        else
        {
            //parent process
            int status;
            if(waitpid(pid, &status, 0) == -1)
            {
                perror("child did not return");
            }
        }
    }
}
