/*
Abdulla Sadoun
CSCI 3120
Assignment 2

Basic Shell Program
This program is a simple shell program that takes user input and executes it as a command.

The program will continue to run until the user enters "exit".
The program uses the fork() system call to create a new process.
The child process then uses the execvp() system call to replace itself with the command specified by the user.
The parent process waits for the child process to complete before continuing.
The program also includes a history feature that allows the user to view the last 10 commands entered.
The user can also execute a specific command from the history by entering "!n" where n is the number of the command in the history.
The user can also execute the last command in the history by entering "!!".

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
//#include <errno.h>
#define MAX_LINE 80 /* The maximum length command */

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* an array of character strings */
    int should_run = 1; /* flag to determine when to exit program */
    char input[100]; // Buffer to store user input

    // create a variable to store history of commands (25char maax, 10 commands max)
    char history[10][25];
    int history_count = 0;
    

    while (should_run) {
        printf("CSCI320>");
        fflush(stdout);

        fgets(input, sizeof(input), stdin); // reading user input from stdin
        input[strcspn(input, "\n")] = 0; // Remove newline character
        // save the command to history
        strcpy(history[history_count], input);


        if(strcmp(input, "exit") == 0) { // Checking for exit command
            should_run = 0; // exiting the loop 
        }
        
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // execvp expects a NULL terminated array

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(1); // Terminate child process

            }
        } else if (pid > 0) {
            // Parent process
            wait(NULL); // Wait for child to complete
        } else {
            // Fork failed
            perror("fork");
        }

        history_count++;
    }
    return 0;
}

