#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @file 412shell.c
 * @brief A simple command interpreter.
 */

/**
 * @brief Maximum number of arguments allowed for a command.
 */
#define MAX_ARGS 64

/**
 * @brief Maximum length of user input allowed.
 */
#define MAX_INPUT 1024

/**
 * @brief A struct to hold the parsed command.
 */
typedef struct {
    /**
     * @brief The command to execute.
     */
    char* command;

    /**
     * @brief The arguments to pass to the command.
     */
    char* args[MAX_ARGS];

    /**
     * @brief Number of arguments in args.
     */
    unsigned short numArgs;

    /**
     * @brief Output filename for redirection, if any.
     */
    char* outputFile;
} ParsedCommand;

/**
 * @brief Forks the current process and executes the parsed command in the child process.
 *
 *  This is the function you will use to fork to create a child process
 *  The child process will execute a user specified command (see the given ParsedCommand struct)
 *  The three major steps are listed as TODOs for you below
 *  Make sure you handle any errors and/or fork failure appropriately
 *
 * @param command The parsed command to execute.
 */
void do_fork(const ParsedCommand* command) {
    //Start the child process
    pid_t child = fork();
    
    //Error handling
    if (child < 0) {
        // Add error handling for failed fork
        perror("fork failed");
        return;
    }
    
    //This will only occur within the child process
    if (child == 0) {  
        //If outputFile given
        if (command->outputFile != NULL) {
            //create file descriptor to use later in the dup2 command
            //O_WRONLY means write only, O_CREATE means create the file if not present, O_TRUNIC truncates file if it does exists, and set file permissions of 644.
            int newfd = open(command->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            //Error handling
            if (newfd < 0) {
                perror("open failed");
                exit(1);
            }
            //redirect stdout to file. all output is handled in utils, so we only add to file if necessary.
            if (dup2(newfd, STDOUT_FILENO) < 0) {
                perror("dup2 failed");
                exit(1);
            }
            //close descriptor
            close(newfd);
        }
        
        // execv expects first arg to be the program name
        // +2 for command itself and NULL terminator
        char* args[MAX_ARGS + 2];  
        //First arg is command
        args[0] = command->command;
        //loop through args and add to array
        for (int i = 0; i < command->numArgs; i++) {
            args[i + 1] = command->args[i];
        }
        //NULL Terminate, required by execvp
        args[command->numArgs + 1] = NULL;  
        
        //Use execvp to fix file path problems with gradescope
        execvp(command->command, args);
        // If execv returns, there was an error
        perror("execv failed");
        exit(1);
    }
    
    //Parent process waits for child to complete and collects exit status, cleaning up the memory.
    if (child > 0) {  
        int status;
        waitpid(child, &status, 0);
    }
}



/**
 * @brief Parse input into a ParsedCommand struct.
 *
 * The input is expected to be in the format "command arg1 arg2 ... argN".
 *
 * @param input The input command string.
 * @return The parsed command.
 */
ParsedCommand parse_command(const char* input) {
    ParsedCommand command = {NULL, {NULL}, 0, NULL};

    char* token = strtok((char*)input, " ");
    while (token != NULL) {
        if (command.command == NULL) {
            command.command = token;
        } else if (strncmp(token, ">", 1) == 0) { // is strcmp a memory safe operation? what are some alternatives?
            // Advance to next token for output file
            token = strtok(NULL, " ");
            command.outputFile = token;
        } else {
            command.args[command.numArgs++] = token;
        }
        token = strtok(NULL, " ");
    }

    // Remove wrapping quotes from arguments
    // Variables are just for readability :)
    char firstArgFirstChar = command.args[0][0];
    unsigned int lastArgIndex = command.numArgs - 1;
    unsigned int lastArgLastCharIndex = strlen(command.args[lastArgIndex]) - 1;
    char lastArgLastChar = command.args[lastArgIndex][lastArgLastCharIndex];

    if (command.numArgs > 0 && firstArgFirstChar == '"' && lastArgLastChar == '"') {
        // Remove the last quote first. If there is only one arg, then shifting the
        // string to remove the first quote will change the index of the last quote.
        // Removing the last quote first means we can avoid another strlen operation.
        command.args[lastArgIndex][lastArgLastCharIndex] = '\0';
        command.args[0]++;
    }

    return command;
}

/**
 * @brief Main function for the shell.
 *
 * Reads input from the user, parses it into a command and its arguments, then creates a child process to execute the command.
 *
 * @return 0 on success.
 */
int main() {
    char input[MAX_INPUT]; // Stores user input

    // Main loop: prompt for input, parse, and execute commands until "exit" is entered
    // is strcmp a memory safe operation? what are some alternatives?
    while (printf("412shell> ") && fflush(stdout) == 0 && fgets(input, MAX_INPUT, stdin) && strcmp(input, "exit\n") != 0) {
        // Strip newline from input
        char* newline = strchr((char*)input, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }

        ParsedCommand command = parse_command(input); // Parse the user input into a command
        do_fork(&command);
    }

    return 0;
}
