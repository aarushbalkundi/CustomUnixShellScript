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

    // line[strcspn(line, "\n")] = '\0';   // remove the \n character at the end of the input
    return line;
}



char **parseLine(char *line)
{
    char *lineCopy = malloc(sizeof(line) + 1);
    strcpy(lineCopy, line);

    char *token = strtok(lineCopy, " ");

    char **args = malloc(sizeof(lineCopy) * ARG_LIMIT);
    

    
    int numArgs = 0;
    while (numArgs < ARG_LIMIT && token != NULL)
    {
        args[numArgs] = malloc(sizeof(token)+1);
        
        if (args[numArgs] == NULL)
        {
            free(args);
        }
        strcpy(args[numArgs], token);
        token = strtok(NULL, " ");
        numArgs ++;
    }

    free(lineCopy);
    free(args);
    
    return args;
}


