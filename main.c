/*
Basic Shell Program
This program is a simple shell program that takes user input and executes it as a command.
The program will continue to run until the user enters "exit".
Abdulla Sadoun
CSCI 3120
*/
/*
Assignment 2
Specifically, this assignment consists of two tasks:
a) Creating the child process and executing the command via the child process.
b) Modifying the shell to allow a history feature.
The details of these tasks will be described in the following sections.
2) Creating a Child Process: The first task is to modify the main() function in the code shown
above so that a child process is created using fork() and thereafter the child process
replaces itself with the command specified by the user using execvp(). To accomplish this
task, your program needs to parse what the user has entered into separate tokens and store
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

    while (should_run) {
        printf("CSCI320>");
        fflush(stdout);

        fgets(input, sizeof(input), stdin); // reading user input from stdin
        input[strcspn(input, "\n")] = 0; // Remove newline character

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
    
        /**
        * After reading user input, the steps are:
        
        * (1) Fork a child process using fork()
            Note that: The fork() system call is invoked inside a while
            loop. If the while loop is not well programmed, it will
            invoke fork() continuously, potentially leading to many
            child processes. When there are too many dangling processes
            under your account, your CSID will be suspended. You are
            encouraged to use the commands in Appendix 2 to monitor your
            processes. And if you cannot log into “csci3120.cs.dal.ca”
            using your CSID any more, you need to contact with
            cshelp@cs.dal.ca to unlock your CSID.

        * (2) The child process will invoke execvp()
            Note that:
            (i) If the user input is an invalid command, execvp() will
            not replace the child process with a binary executable.
            Instead, execvp() returns the value of “-1”. In this case,
            the child process should check the return value and invoke
            exit() system call to terminate itself. Otherwise, the child
            process will keep running, potentially leading to the
            situation where the parent waits for the child to terminate
            and the child becomes a clone of the parent (note that, in
            this case, the child would create its own child process if
            the user enters another command).
            (ii)If the user input is a valid command, exit() does not
            need to be invoked because the child process will be
            replaced with the binary executable corresponding to the
            valid command, and the executable can be terminated
            properly. The details can be found in Section 3.4.b.c.

        * (3) Parent will invoke wait()
        **/

        
    }
    return 0;
}

