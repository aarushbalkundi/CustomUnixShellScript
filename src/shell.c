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

}

