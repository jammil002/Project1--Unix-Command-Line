// James Miller
// CST - 315
// Project 1

// ! To create and exectuable: gcc CommmandLine.c -o CommandLine
// ! Run this executable: ./CommandLine

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// function prototypes
int HandleInput();
void HandleTokenization();
void HandleExecute();
void CommandLine();

// function that contains commandline information
void CommandLine()
{
    printf("-------------------------------------------------------------\n");
    printf("-             Project 1: Command Line Interface             -\n");
    printf("-                   To leave type 0 or                      -\n");
    printf("-                Enter a single command or                  -\n");
    printf("-           A list of commands seperated by a ';'           -\n");
    printf("-------------------------------------------------------------\n");
    if (HandleInput() == 1)
    {
        CommandLine();
    }
}

// function scans user input then sends it to be tokenized
int HandleInput()
{
    char userInput;
    scanf("%c", &userInput);

    if (userInput == '0')
    {
        return 0;
    }

    HandleTokenization(userInput);

    return 1;
}

// function splits up input into individual
void HandleTokenization(char *userInput)
{
    char *tokenArray[256];
    char *tokens;
    int i = 0;

    tokens = strtok(userInput, "\n ");
    while (tokens != NULL)
    {
        tokenArray[i++] = tokens; // Add tokens into the array
        tokens = strtok(NULL, "\n ");
    }
    tokenArray[i] = NULL;
    HandleExecute(tokenArray);
}

void HandleExecute(char *tokens)
{
    int nProcess = fork();
}

int main()
{
    CommandLine();
    return 0;
}