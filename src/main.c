#include "../include/shell.h"


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