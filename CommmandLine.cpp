#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

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
        try
        {
            CreateDirectory(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Create Directory Command Syntax - cdir <directory-name>");
        }
    }
    else if (strcmp(tokens[0], "rdir") == 0)
    {
        try
        {
            RenameDirectory(tokens[1], tokens[2]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Rename Directory Command Syntax - rdir <old-directory-name> <new-directory-name>");
        }
    }
    else if (strcmp(tokens[0], "ddir") == 0)
    {
        try
        {
            DeleteDirectory(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Delete Directory Command Syntax - ddir <directory-name>");
        }
    }
    else if (strcmp(tokens[0], "cfile") == 0)
    {
        try
        {
            CreateFile(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Create File Command Syntax - cfile <file-name>");
        }
    }
    else if (strcmp(tokens[0], "rfile") == 0)
    {
        try
        {
            RenameFile(tokens[1], tokens[2]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Rename File Command Syntax - rfile <old-file-name> <new-file-name>");
        }
    }
    else if (strcmp(tokens[0], "dfile") == 0)
    {
        try
        {
            DeleteFile(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Delete File Command Syntax - dfile <file-name>");
        }
    }
    else if (strcmp(tokens[0], "mfile") == 0)
    {
        try
        {
            MoveFile(tokens[1], tokens[2]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Move File Command Syntax - mfile <source-file> <destination-file>");
        }
    }
    else if (strcmp(tokens[0], "dupfile") == 0)
    {
        try
        {
            DuplicateFile(tokens[1], tokens[2]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Duplicate File Command Syntax - dfile <source-file> <destination-file>");
        }
    }
    else if (strcmp(tokens[0], "sfile") == 0)
    {
        try
        {
            SearchFile(tokens[1], tokens[2]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Search File Command Syntax - dfile <file-name> <directory-name>");
        }
    }
    else if (strcmp(tokens[0], "dirtree") == 0)
    {
        try
        {
            DisplayDirectoryTree(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Display Directory Command Syntax - dirtree <directory-name>");
        }
    }
    else if (strcmp(tokens[0], "gbfinfo") == 0)
    {
        try
        {
            GetBasicFileInformation(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Get Basic File Info Command Syntax - gbfinfo <file-name>");
        }
    }
    else if (strcmp(tokens[0], "gafinfo") == 0)
    {
        try
        {
            GetAdvancedFileInformation(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Get Advanced File Info Command Syntax - gafinfo <file-name>");
        }
    }
    else if (strcmp(tokens[0], "gbdinfo") == 0)
    {
        try
        {
            GetBasicDirectoryInformation(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Get Basic Directory Info Command Syntax - gbdinfo <directory-name>");
        }
    }
    else if (strcmp(tokens[0], "gadinfo") == 0)
    {
        try
        {
            GetAdvancedDirectoryInformation(tokens[1]);
        }
        catch (const std::exception &e)
        {
            printf("TurtleError > Get Advanced Directory Info Command Syntax - gadinfo <directory-name>");
        }
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

// File Functions
void CreateDirectory(char *dirname)
{
    if (mkdir(dirname, 0755) < 0)
    {
        perror("Error creating directory");
        return;
    }
    // Add the new directory to the internal file directory
    FileNode *newDir = (FileNode *)malloc(sizeof(FileNode));
    strncpy(newDir->charName, dirname, MAX_INPUT_LENGTH);
    newDir->isDirectory = 1;
    newDir->next = fileDirectory;
    fileDirectory = newDir;
}
void RenameDirectory(char *dirname, char *newName)
{
    if (rename(dirname, newName) < 0)
    {
        perror("Error renaming directory");
        return;
    }

    // Update the directory name in the internal file directory
    FileNode *cur = fileDirectory;
    while (cur != NULL)
    {
        if (strcmp(cur->charName, dirname) == 0 && cur->isDirectory)
        {
            strncpy(cur->charName, newName, MAX_INPUT_LENGTH);
            break;
        }
        cur = cur->next;
    }
}
void DeleteDirectory(char *dirname)
{
    if (rmdir(dirname) < 0)
    {
        perror("Error deleting directory");
        return;
    }

    // Remove the directory from the internal file directory
    FileNode *prev = NULL, *cur = fileDirectory;
    while (cur != NULL)
    {
        if (strcmp(cur->charName, dirname) == 0 && cur->isDirectory)
        {
            if (prev == NULL)
            {
                fileDirectory = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }
            free(cur);
            break;
        }
        prev = cur;
        cur = cur->next;
    }
}

void RenameFile(char *fileName, char *newName)
{
    if (rename(fileName, newName) < 0)
    {
        perror("Error renaming file");
        return;
    }
}
void DeleteFile(char *fileName)
{
    if (remove(fileName) < 0)
    {
        perror("Error deleting file");
        return;
    }
}

void CreateFile(char *fileName) {}
void EditFile(char *fileName) {}
void MoveFile(char *sourceFile, char *destinationFile) {}
void DuplicateFile(char *sourceFile, char *destinationFile) {}
void SearchFile(char *dirName, char *fileName) {}
void DisplayDirectoryTree(char *dirName) {}

void GetBasicFileInformation(char *fileName)
{
    struct stat fileInfo;
    if (stat(fileName, &fileInfo) < 0)
    {
        perror("Error getting file information");
        return;
    }

    printf("File name: %s\n", fileName);
    printf("Size: %ld bytes\n", (long)fileInfo.st_size);
    printf("Last modified: %s", ctime(&fileInfo.st_mtime));
}
void GetAdvancedFileInformation(char *fileName)
{
    struct stat file_info;
    if (stat(fileName, &file_info) < 0)
    {
        perror("Error getting file information");
        return;
    }

    printf("File name: %s\n", fileName);
    printf("Size: %ld bytes\n", (long)file_info.st_size);
    printf("User ID: %u\n", file_info.st_uid);
    printf("Group ID: %u\n", file_info.st_gid);
    printf("Permissions: %o\n", file_info.st_mode & 0777);
    printf("Last modified: %s", ctime(&file_info.st_mtime));
}
void GetBasicDirectoryInformation(char *dirName)
{
    struct stat dirInfo;
    if (stat(dirName, &dirInfo) < 0)
    {
        perror("Error getting directory information");
        return;
    }

    printf("Directory name: %s\n", dirName);
    printf("Last modified: %s", ctime(&dirInfo.st_mtime));
}
void GetAdvancedDirectoryInformation(char *dirName)
{
    struct stat dirInfo;
    if (stat(dirName, &dirInfo) < 0)
    {
        perror("Error getting directory information");
        return;
    }

    printf("Directory name: %s\n", dirName);
    printf("User ID: %u\n", dirInfo.st_uid);
    printf("Group ID: %u\n", dirInfo.st_gid);
    printf("Permissions: %o\n", dirInfo.st_mode & 0777);
    printf("Last modified: %s", ctime(&dirInfo.st_mtime));
}
