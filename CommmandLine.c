// James Miller
// CST - 315
// Project 1

// To create and exectuable: gcc CommmandLine.c -o CommandLine
// Run this executable: ./CommandLine

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

char *userInput;

void CommandLine()
{
    printf("Project 1: Command Line Interface \n");
    printf("To leave type 1: \n");
    printf("or \n");
    printf("Enter a single command or a list of commands seperated by a ';':\n");
}

void handleInput()
{
}

int main()
{

    CommandLine();

    return 0;
}