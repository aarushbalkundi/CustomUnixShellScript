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



/*
3. char **parse_line(char *line){
Make a copy of the line (strtok modifies the string)
Tokenize each argument using strtok. Split by white space or tab or new
line
char *token = strtok(line_copy, “ \t\n”);
while (token != NULL && num_args <MAX_ARGS -1)
 //allocate memory 
args[num_args] = malloc(strlen(token)+1);
copy token
strcpy(args[num_args], token);
num_args++;
token = strtok(NULL, “ \t\n”);
}
args[num_args] = NULL;
free(line_copy);
return args;
}
*/

char **parseLine(char *line)
{
    char *lineCopy = line;
    char *token = strtok(lineCopy, " ");

    int numArgs = 1;
    char **args;
    
    args[0] = malloc(strlen(token)+1);
    args[0] = token;

    while (numArgs < 10)
    {
        token = strtok(NULL, " ");
        args[numArgs] = malloc(strlen(token)+1);
        strcpy(args[numArgs], token);
        printf("%s", token);
    }

    return args;

}


