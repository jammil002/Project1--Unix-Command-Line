#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMANDS 10
#define MAX_INPUT_LENGTH 256

void ExecuteCommands(char *commands[], int num_commands);
void HandleInput(char *input);
void BatchMode(FILE *batch_file);
void EndExecution();
void GetHelp();
void CommandLine();
void TokenizeInput(char *input, char *tokens[], int *num_tokens);

// File Directory Functions
void CreateDirectory(char *dirname);
void RenameDirectory(char *dirname, char *newName);
void DeleteDirectory(char *dirname);
void CreateFile(char *fileName);
void RenameFile(char *fileName, char *newName);
void EditFile(char *fileName);
void DeleteFile(char *fileName);
void MoveFile(char *sourceFile, char *destinationFile);
void DuplicateFile(char *sourceFile, char *destinationFile);
void SearchFile(char *dirName, char *fileName);
void DisplayDirectoryTree(char *dirName);
void GetBasicFileInformation(char *fileName);
void GetAdvancedFileInformation(char *fileName);
void GetBasicDirectoryInformation(char *dirName);
void GetAdvancedDirectoryInformation(char *dirName);

// File Directory Data Structures
typedef struct FileNode
{
    char charName[MAX_INPUT_LENGTH];
    int isDirectory;
    struct FileNode *next;

} FileNode;

// ! Global Variables
FileNode *fileDirectory = NULL;

// Batch mode is determined by the amount of inputs.
int main(int argc, char **argv)
{
    if (argc > 1)
    {
        // Will open file user inputed. Directions are in the README.
        FILE *batch_file = fopen(argv[1], "r");
        if (batch_file)
        {
            BatchMode(batch_file);
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
        CommandLine();
    }
    return 0;
}

// The ExecuteCommands function takes an array of commands and the number of
// commands to execute, then forks a new process for each command and executes it.
void ExecuteCommands(char *commands[], int num_commands)
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

            TokenizeInput(command, tokens, &num_tokens);
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

void HandleInput(char *input)
{
    char *tokens[MAX_COMMANDS];
    int num_tokens = 0;
    TokenizeInput(input, tokens, &num_tokens);

    // Handle input based on if exit or command is entered.

    if (num_tokens == 0)
    {
        return;
    }

    if (strcmp(tokens[0], "exit") == 0)
    {
        EndExecution();
    }
    else if (strcmp(tokens[0], "help") == 0)
    {
        GetHelp();
    }
    else if (strcmp(tokens[0], "cdir") == 0)
    {
        CreateDirectory(tokens[1]);
    }
    else if (strcmp(tokens[0], "rdir") == 0)
    {
        RenameDirectory(tokens[1], tokens[2]);
    }
    else if (strcmp(tokens[0], "ddir") == 0)
    {
        DeleteDirectory(tokens[1]);
    }
    else if (strcmp(tokens[0], "cfile") == 0)
    {
        CreateFile(tokens[1]);
    }
    else if (strcmp(tokens[0], "rfile") == 0)
    {
        RenameFile(tokens[1], tokens[2]);
    }
    else if (strcmp(tokens[0], "dfile") == 0)
    {
        DeleteFile(tokens[1]);
    }
    else if (strcmp(tokens[0], "mfile") == 0)
    {
        MoveFile(tokens[1], tokens[2]);
    }
    else if (strcmp(tokens[0], "dupfile") == 0)
    {
        DuplicateFile(tokens[1], tokens[2]);
    }
    else if (strcmp(tokens[0], "sfile") == 0)
    {
        SearchFile(tokens[1], tokens[2]);
    }
    else if (strcmp(tokens[0], "dirtree") == 0)
    {
        DisplayDirectoryTree(tokens[1]);
    }
    else if (strcmp(tokens[0], "gbfinfo") == 0)
    {
        GetBasicFileInformation(tokens[1]);
    }
    else if (strcmp(tokens[0], "gafinfo") == 0)
    {
        GetAdvancedFileInformation(tokens[1]);
    }
    else if (strcmp(tokens[0], "gbdinfo") == 0)
    {
        GetBasicDirectoryInformation(tokens[1]);
    }
    else if (strcmp(tokens[0], "gadinfo") == 0)
    {
        GetAdvancedDirectoryInformation(tokens[1]);
    }
    else
    {
        ExecuteCommands(tokens, num_tokens);
    }
}

void BatchMode(FILE *batch_file)
{
    char line[256];
    // Loop through input of batch file.
    while (fgets(line, sizeof(line), batch_file))
    {
        printf("%s", line);
        HandleInput(line);
    }
}

// Function will end the process.
void EndExecution()
{
    printf("Exiting shell.\n");
    exit(EXIT_SUCCESS);
}

void GetHelp()
{
    printf("-------------------------------------------------------------\n");
    printf("-                      Turtle Shell Help                    -\n");
    printf("-            1. Handles all General Unix Commands.          -\n");
    printf("-          2. Handles Improper Spacing and other Errors.    -\n");
    printf("-              3. Handles Commands via Forking.             -\n");
    printf("-------------------------------------------------------------\n");
}

void CommandLine()
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
        HandleInput(input);
    }
}

void TokenizeInput(char *input, char *tokens[], int *num_tokens)
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
