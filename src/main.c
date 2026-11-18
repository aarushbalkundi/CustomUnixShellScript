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

        if(line == NULL)
        {
            break;
        }
        // printf("line read\n");
        // printf("line = %s\n", line);

        // -----------------phase 2----------------------------------------------
        char **redir = checkRedirection(line);

        // if (redir[0] != NULL)
        // {
        //     printf("redirection not implemented yet\n");
        //     continue;
        // }

        int i = 0;
        while (redir[i] != NULL)
        {
            printf("found %s\n", redir[i]);
            i++;
        }


        // -----------------phase 2----------------------------------------------
        

        args = parseLine(line);

        
        if( args == NULL || args[0] == NULL){

            free(args);

            free(line);
            continue;
        }

        if (checkGlob(args)){
            char **expanded = expandGlob(args);

            if (expanded != NULL){

                for (int i = 0; expanded[i] != NULL; i++){
                    free(expanded[i]);
                }

                free(expanded);
            }
        }

        // printf("line:\n%s", line);

        // int i = 0;
        // while (args[i]!=NULL)
        // {
        //     printf("args = %s\n", args[i]);
        //     i++;
        // }

        //int a = checkBuiltIn(args[0]);

        // printf("builtin = %d\n", a);

        //processing and execution of command to be done here
        if(!checkBuiltIn(args[0]))
        {
            executeExternal(args);
        }
        else
        {
            // printf("inside internal execution\n");
            executeInternal(args[0], args);
        }

        free(line);
        // free(args);

    }


    return 0;
}