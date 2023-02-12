// James Miller
// CST - 315
// Project 1

// ! To create and exectuable: gcc CommmandLine.c -o CommandLine
// ! Run this executable: ./CommandLine

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

// function prototypes
int HandleInput();
void HandleTokenization();
void HandleExecute();
void CommandLine();
int main();

// global variables
size_t total = 5;
char *userInput = NULL;
char *tokenArray[256];
char *tokens;
bool isFirstRun = true;

// function that contains commandline information
void CommandLine()
{
    if (isFirstRun)
    {
        printf("-------------------------------------------------------------\n");
        printf("-             Project 1: Command Line Interface             -\n");
        printf("-                   To leave type 'q' or                    -\n");
        printf("-                Enter a single command or                  -\n");
        printf("-           A list of commands seperated by a ';'           -\n");
        printf("-------------------------------------------------------------\n");
        isFirstRun = false;
    }
    printf("Turtle Shell >> ");
    int quit = HandleInput();

    if (quit == 0)
    {
        return;
    }

    CommandLine();
}

// function scans user input then sends it to be tokenized
int HandleInput()
{
    int result = getline(&userInput, &total, stdin);

    if (result == -1)
    {
        printf("EOF detected, quitting...\n");
        return 0;
    }

    if (strcmp(userInput, "0") == 0)
    {
        return 0;
    }

    HandleTokenization();

    return 1;
}

// function splits up input into individual
void HandleTokenization()
{

    int i = 0;

    tokens = strtok(userInput, "\n ");
    while (tokens != NULL)
    {
        tokenArray[i++] = tokens; // Add tokens into the array
        tokens = strtok(NULL, "\n ");
    }
    tokenArray[i] = NULL;
    free(userInput);
    HandleExecute();
}

void HandleExecute()
{
    int nProcess = fork();
    if (nProcess != 0)
    {
        int wait;
        waitpid(-1, &wait, 0);
    }
    else
    {
        if (execvp(tokenArray[0], tokenArray) == -1)
        {
            perror("Wrong command");
            exit(errno);
        }
    }
}

int main()
{
    CommandLine();
    return 0;
}