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
        args = parseLine(line);
        
        printf("line:\n%s", line);
        
        char **ptr = args;
        int i = 0;
        while (ptr[i]!=NULL)
        {
            printf("%s", ptr[i]);
            i++;
        }

        //processing and execution of command to be done here

        free(line);
        // free(args);

    }

    return 0;
}