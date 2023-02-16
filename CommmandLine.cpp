#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMANDS 10

void execute_commands(char *commands[], int num_commands);
void handle_input(char *input);
void batch_mode(FILE *batch_file);
void end_execution();
void command_line();
void tokenize_input(char *input, char *tokens[], int *num_tokens);

// Batch mode is determined by the amount of inputs.
int main(int argc, char **argv)
{
    if (argc > 1)
    {
        // Will open file user inputed. Directions are in the README.
        FILE *batch_file = fopen(argv[1], "r");
        if (batch_file)
        {
            batch_mode(batch_file);
            fclose(batch_file);
        }
        else
        {
            fprintf(stderr, "Error opening batch file.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        command_line();
    }
    return 0;
}

// The execute_commands function takes an array of commands and the number of
// commands to execute, then forks a new process for each command and executes it.
void execute_commands(char *commands[], int num_commands)
{
    pid_t pid;
    int status;
    int i;

    // This will loop through the commands. Creating a child and parent process.
    // The child process will run the command while the parent process will continue after.
    for (i = 0; i < num_commands; i++)
    {
        char *command = commands[i];
        pid = fork();
        // Fork between parent and child process.
        if (pid < 0)
        {
            fprintf(stderr, "Fork failed.\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            char *tokens[MAX_COMMANDS];
            int num_tokens = 0;
            // Commands are tokenized to correctlye be ran.
            tokenize_input(command, tokens, &num_tokens);
            if (execvp(tokens[0], tokens) < 0)
            {
                // If command get here child process has failed.
                fprintf(stderr, "Error executing command: %s\n", tokens[0]);
                exit(EXIT_FAILURE);
            }
        }
        while (wait(&status) > 0)
        {
            // Wait for child processes to complete.
        }
    }
}

void handle_input(char *input)
{
    char *tokens[MAX_COMMANDS];
    int num_tokens = 0;
    tokenize_input(input, tokens, &num_tokens);

    // Handle input based on if exit or command is entered.

    if (num_tokens == 0)
    {
        return;
    }

    if (strcmp(tokens[0], "exit") == 0)
    {
        end_execution();
    }
    else
    {
        execute_commands(tokens, num_tokens);
    }
}

void batch_mode(FILE *batch_file)
{
    char line[256];
    // Loop through input of batch file.
    while (fgets(line, sizeof(line), batch_file))
    {
        printf("%s", line);
        handle_input(line);
    }
}

// Function will end the process.
void end_execution()
{
    printf("Exiting shell.\n");
    exit(EXIT_SUCCESS);
}

void command_line()
{
    char input[256];
    printf("-------------------------------------------------------------\n");
    printf("-             Project 1: Command Line Interface             -\n");
    printf("-                   To leave type 'exit' or                 -\n");
    printf("-                Enter a single command or                  -\n");
    printf("-           A list of commands seperated by a ';'           -\n");
    printf("-------------------------------------------------------------\n");
    while (1)
    {
        printf("Turtle Shell >> ");
        fgets(input, sizeof(input), stdin);
        handle_input(input);
    }
}

void tokenize_input(char *input, char *tokens[], int *num_tokens)
{
    char *token;
    // Strings are to
    token = strtok(input, " ;\n");
    while (token != NULL && *num_tokens < MAX_COMMANDS)
    {
        tokens[(*num_tokens)++] = token;
        token = strtok(NULL, " ;\n");
    }
    tokens[*num_tokens] = NULL;
}
