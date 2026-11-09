// #include "shell.h"
#include "../include/shell.h"
#include <glob.h>

void printPrompt()
{
    printf("myshell--> ");
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

    if(line == NULL){
        return NULL;

    }

    char *lineCopy = malloc(strlen(line) + 1);

    if(lineCopy == NULL){
        return NULL;
    }
    strcpy(lineCopy, line);

    char *token = strtok(lineCopy, " ");

    char **args = calloc(ARG_LIMIT + 1, sizeof(char *));

    if(args == NULL){
        free(lineCopy);
        return NULL;
    }



    int numArgs = 0;
    while (numArgs < ARG_LIMIT && token != NULL)
    {
        args[numArgs] = malloc(strlen(token)+1);

        if (args[numArgs] == NULL){

            for (int i = 0; i < numArgs; i++){
                free(args[i]);
            }
            free(args);
            free(lineCopy);
            return NULL;
        }
        strcpy(args[numArgs], token);
        // printf("arg%d = %s\n", numArgs, args[numArgs]);
        token = strtok(NULL, " ");
        numArgs ++;
    }
    args[numArgs] = NULL;

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
    if(args == NULL){
        return 0;
    }

    for(int i = 1; args[i] != NULL; i++){
        if(strchr(args[i], '*') != NULL || strchr(args[i], '?') != NULL || strchr(args[i], '[') != NULL){
            return 1;
        }
    }
    return 0;

}

char **expandGlob(char **args)
{
    if(args == NULL){
        return NULL;
    }

    glob_t globBuff;
    size_t totalArgs = 0;

    for(int i = 0; args[i] != NULL; i++){
        if(i >0 && (strchr(args[i], '*') != NULL || strchr(args[i], '?') != NULL || strchr(args[i], '[') != NULL)){

            memset(&globBuff, 0, sizeof(globBuff));

            if(glob(args[i], GLOB_NOCHECK, NULL, &globBuff) != 0){
                globfree(&globBuff);
                return NULL;
            }


            totalArgs += globBuff.gl_pathc;
            globfree(&globBuff);
        }
        else{
            totalArgs++;
        }

    }

    char **resultArgs = calloc(totalArgs + 1, sizeof(char *));

    if(resultArgs == NULL){

        return NULL;
    }

    size_t index = 0;

    for(int i = 0; args[i] != NULL; i++){
        if (i > 0 && (strchr(args[i], '*') != NULL || strchr(args[i], '?') != NULL || strchr(args[i], '[') != NULL)){
            memset(&globBuff, 0, sizeof(globBuff));

            if(glob(args[i], GLOB_NOCHECK, NULL, &globBuff) != 0){
                globfree(&globBuff);
                freeArgs(resultArgs);
                return NULL;
            }

            for(size_t j = 0; j < globBuff.gl_pathc; j++){
                resultArgs[index] = malloc(strlen(globBuff.gl_pathv[j]) + 1);
                if(resultArgs[index] == NULL){

                    globfree(&globBuff);

                    freeArgs(resultArgs);

                    return NULL;
                }
                strcpy(resultArgs[index], globBuff.gl_pathv[j]);
                index++;
            }
            globfree(&globBuff);
        }
        else{
            resultArgs[index] = malloc(strlen(args[i]) + 1);
            if(resultArgs[index] == NULL){
                freeArgs(resultArgs);
                return NULL;
            }
            strcpy(resultArgs[index], args[i]);
            index++;
        }
    }

    resultArgs[index] = NULL;
    return resultArgs;



}

void executeExternal(char **args)
{

    if (checkGlob(args) == 0)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Failed to fork");
            return;
        }

        if (pid == 0)
        {
            //child process
            execvp(args[0], args);
            perror("myshell");
            _exit(127);


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
        char **resultArgs = expandGlob(args);

        if(resultArgs == NULL){
            printf("Failed to expand glob\n");
            return;
        }
        pid_t pid = fork();
        if (pid < 0){
            perror("Failed to fork");
            freeArgs(resultArgs);
            return;
        }

        if (pid == 0){
            execvp(resultArgs[0], resultArgs);
        }
        else{
            int status;
            if(waitpid(pid, &status, 0) == -1){
                perror("child did not return");
            }
        }
        freeArgs(resultArgs);

    }
}


int executeInternal(char *command, char **args){
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

void freeArgs(char **args)
{
    if (args == NULL)
        return;

    for (int i = 0; args[i] != NULL; i++)
    {
        free(args[i]);
    }

    free(args);
}