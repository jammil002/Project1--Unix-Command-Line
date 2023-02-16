### Unix Command Line

#### Instructions

Refer to the Releases tab on the right. Download the CommmandLine executable and navigate to the directory via the command line. Run the CommmandLine executable using the command: **_./CommandLine_**

To run a batch file use the command: **_./CommandLine <filename.sh>_**

#### Explanation

This program is a simple command line interpreter, also known as a shell. The user can enter commands to be executed one at a time, or they can run commands in batch mode by providing a text file as input.

The program starts by defining several functions: execute_commands, handle_input, batch_mode, end_execution, command_line, and tokenize_input.

The handle_input function takes a string input and tokenizes it using tokenize_input. If the input is empty, the function returns. If the input is "exit", the function calls end_execution to exit the shell. Otherwise, the function calls execute_commands with the array of tokens and the number of tokens.

The batch_mode function takes a file pointer to a batch file and reads each line from the file. For each line, the function prints the line and calls handle_input with the line as input.

The end_execution function prints a message indicating that the shell is exiting, then exits the program.

The command_line function starts the command line interface. It prints a welcome message and a prompt, then reads input from the user using fgets. It then calls handle_input with the user's input.

The tokenize_input function takes a string input, an array of tokens, and a pointer to the number of tokens. It uses strtok to tokenize the input by spaces, semicolons, and newlines, and adds each token to the array. The function stops adding tokens when it reaches the end of the input or the maximum number of tokens (defined as MAX_COMMANDS). It then sets the last token in the array to NULL.

The main function checks if the program was run in batch mode or command line mode by checking the number of command line arguments. If a batch file was provided, it opens the file and calls batch_mode with the file pointer. If no arguments were provided, it calls command_line.
