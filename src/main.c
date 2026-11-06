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
        // printf("line read\n");
        // printf("line = %s\n", line);

        args = parseLine(line);
        // printf("line:\n%s", line);
        
        // int i = 0;
        // while (args[i]!=NULL)
        // {
        //     printf("args = %s\n", args[i]);
        //     i++;
        // }

        int a = checkBuiltIn(args[0]);

        // printf("builtin = %d\n", a);

        //processing and execution of command to be done here
        if(!checkBuiltIn(args[0]))
        {
            executeExternal(args);
        }

        free(line);
        // free(args);

    }

    return 0;
}