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
        printf("line read\n");
        printf("line = %s\n", line);

        args = parseLine(line);
        printf("line:\n%s", line);
        

        //processing and execution of command to be done here

        free(line);
        // free(args);

    }

    return 0;
}