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

// Function Prototypes
int HandleInput();
void HandleTokenization(char userInput[]);
void HandleExecute();
void CommandLine();

// Global Variables
size_t total = 5;
char *tokenArray[256];
bool isFirstRun = true;

// Function that contains shell display:
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

// Function scans user input then sends it to be tokenized:
int HandleInput()
{
    size_t bufsize = 32;
    char userInput[128];
    int result = getline(userInput, &bufsize, stdin);

    if (result == -1)
    {
        printf("EOF detected, quitting...\n");
        return 0;
    }

    if (userInput[1] == 'q')
    {
        return 0;
    }

    HandleTokenization(userInput);

    return 1;
}

// Function splits up input into individual commands:
void HandleTokenization(char userInput[])
{
    char *tokens;
    int i = 0;

    tokens = strtok(userInput, "\n ");
    while (tokens != NULL)
    {
        tokenArray[i++] = tokens;
        tokens = strtok(NULL, "\n ");
    }
    tokenArray[i] = NULL;
    HandleExecute();
}

void HandleExecute()
{
    //** Fork creates a child process
    int childStatus;
    pid_t childProcess = fork();
    if (childProcess == 0)
    {
        //** This is handled by the child process.
        execvp(tokenArray[0], tokenArray);

        //** If the execvp command returns, there must of been an error.
        perror("Unknown command. \n");
        exit(errno);
    }
    else
    {
        //** This is then handled by the parent process.
        //** We must wait for the child process to finish.
        pid_t tpid = wait(&childStatus);

        // After the waiting is complete the process will move on.
    }
}

int main()
{
    CommandLine();
    return 0;
}