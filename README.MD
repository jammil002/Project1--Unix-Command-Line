### Unix Command Line

#### Instructions

Refer to the Releases tab on the right. Download the CommmandLine executable and navigate to the directory via the command line. Run the CommmandLine executable using the command: **_./CommandLine_**

#### Explanation

The main function of the code is the CommandLine() function which is a loop that continues until the user types 0 or quits the program. The program first calls the HandleInput() function, which gets the input from the user using the getline() function and then calls the HandleTokenization() function.

The HandleTokenization() function splits the input into individual tokens using the strtok() function, and adds these tokens into an array. Finally, the program calls the HandleExecute() function which creates a child process using the fork() system call and executes the command using the execvp() system call. If the command is not found, the perror() function is used to display an error message and the process exits.
