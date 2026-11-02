//including everything for now, will need to recheck these after completing project
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define CHAR_LIMIT 1024

void printPrompt()
{
    printf("-->");
    fflush(stdout);  
}

char *readLine(void)
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


int main()
{
    char *line;
    char **args;

    printf("Custom Shell\n");
    
    while (true)
    {
        printPrompt();
    
        line = readLine();
        // args = parseLine(line);
        
        //processing and execution of command to be done here

        free(line);
        // free(args);

    }

    return 0;
}