//including everything for now, will need to recheck these after completing project
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <glob.h>

#define CHAR_LIMIT 1024


// input handling functions
void printPrompt();
char *readLine();
char **parseLine(char *line);


