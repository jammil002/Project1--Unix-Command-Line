#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMANDS 10
#define MAX_INPUT_LENGTH 256

// Function to execute commands.
void execute_commands(char *commands[], int num_commands);
// Handle user input.
void handle_input(char *input);
// Hand the batch input.
void batch_mode(FILE *batch_file);
// Function is hit when user ends using 'exit'.
void end_execution();
// Get help from CLI menu.
void getHelp();
// First function called after main. Begins CLI process.
void command_line();
// Tokenize and prepare commands for execution.
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
            fprintf(stderr, "Error opening batch file '%s'.\n", argv[1]);
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
    int i;
    int pids[num_commands];

    for (i = 0; i < num_commands; i++)
    {
        char *command = commands[i];
        pids[i] = fork();

        if (pids[i] < 0)
        {
            fprintf(stderr, "Fork failed.\n");
            exit(EXIT_FAILURE);
        }
        else if (pids[i] == 0)
        {
            char *tokens[MAX_COMMANDS];
            int num_tokens = 0;

            tokenize_input(command, tokens, &num_tokens);
            if (execvp(tokens[0], tokens) < 0)
            {
                fprintf(stderr, "Error executing command: %s\n", tokens[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Wait for all child processes to complete.
    while (wait(NULL) != -1)
    {
    };
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
    else if (strcmp(tokens[0], "help") == 0)
    {
        getHelp();
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

void getHelp()
{
    printf("-------------------------------------------------------------\n");
    printf("-                      Turtle Shell Help                    -\n");
    printf("-            1. Handles all General Unix Commands.          -\n");
    printf("-          2. Handles Improper Spacing and other Errors.    -\n");
    printf("-              3. Handles Commands via Forking.             -\n");
    printf("-------------------------------------------------------------\n");
}

void command_line()
{
    char input[256];
    // Prints needed CLI information.
    printf("-------------------------------------------------------------\n");
    printf("-             Project 1: Command Line Interface             -\n");
    printf("-                       Turtle Shell                        -\n");
    printf("-                 Exit (exit) | Help (help)                 -\n");
    printf("-              Single Command | Multi-Command (<1>;<2>)     -\n");
    printf("-------------------------------------------------------------\n");
    while (1)
    {
        printf("Turtle Shell >> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            fprintf(stderr, "Error reading input.\n");
            exit(EXIT_FAILURE);
        }
        if (strlen(input) > MAX_INPUT_LENGTH - 1)
        {
            fprintf(stderr, "Input exceeds maximum length.\n");
            continue;
        }
        handle_input(input);
    }
}

void tokenize_input(char *input, char *tokens[], int *num_tokens)
{
    char *token;
    // User input is tokenized base on ; or \n.
    token = strtok(input, " ;\n");
    while (token != NULL && *num_tokens < MAX_COMMANDS)
    {
        tokens[(*num_tokens)++] = token;
        token = strtok(NULL, " ;\n");
    }
    // Token must end with a null.
    tokens[*num_tokens] = NULL;
}
