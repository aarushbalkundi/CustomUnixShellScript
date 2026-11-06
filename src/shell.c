// #include "shell.h"
#include "../include/shell.h"
#include <glob.h>

void printPrompt()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s", cwd);
    // return 1 /* continue running
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
    char globChars[3] = {'*', '?', '['};

    for (int i = 0; args[i] != NULL; i++)
    {
        for (int j = 0; j<=2; j++)
        {
            if (strchr(args[i], globChars[j]) == NULL)
            {
                // printf("globbing not present\n");
                return 0;
            }
        }
    }
    return 1;
}

void executeExternal(char **args)
{
    if (checkGlob(args) == 0)
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
    else
    {
        printf("globbing not implemented yet\n");
        
    }
}


int executeInternal(char *command, char **args)
{
    if (strcmp(command, "exit") == 0)
    {
        printf("Farewell\n");
        exit(0);
    }

    else if (strcmp(command, "cd") == 0)
    {
        char *path = args[1];
        if(path == NULL)
        {
            path = getenv("HOME");
        }
        if (chdir(path) != 0)
        {
            printf("path does not exist:%s  ", path);
            return -1;
        }
    }

    else if(strcmp(command, "pwd") == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
        return 1;   
    }
    return 1;
}