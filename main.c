/*
Basic Shell Program
This program is a simple shell program that takes user input and executes it as a command.

Abdulla Sadoun
CSCI 3120
Assignment 2

The program will continue to run until the user enters "exit".
The program uses the fork() system call to create a new process.
The child process then uses the execvp() system call to replace itself with the command specified by the user.
The parent process waits for the child process to complete before continuing.
The program also includes a history feature that allows the user to view the last 10 commands entered when they type in history
The user can also execute a specific command from the history by entering "!n" where n is the number of the command in the history.
The user can also execute the last command in the history by entering "!!".
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_LINE 80 /* The maximum length command */
#define MAX_HISTORY 10 // max amount of saved history
#define MAXSTRENLEN 25 // maximum length of history command
#define Initialloopcounter 0

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* an array of character strings */
    int should_run = 1; /* flag to determine when to exit program */
    char input[100]; // uffer to store user input

    // create a variable to store history of commands (25char maax, 10 commands max)
    char history[MAX_HISTORY][MAXSTRENLEN]; // 2d array for array of strings
    int history_count = Initialloopcounter;

    while (should_run) {
        printf("CSCI320>");
        fflush(stdout); 

        fgets(input, sizeof(input), stdin); // reading user input from stdin
        input[strcspn(input, "\n")] = 0; // remove \n
        strcpy(history[history_count], input); // save the command to history

        if(strcmp(input, "exit") == 0) { // Checking for exit command
            should_run = 0; // exiting the loop 
        }

        if(strcmp(input, "history") == 0) { // Checking for history command
            printf("ID    PID    Command\n"); // print headers
            for (int i = Initialloopcounter; i < history_count; i++) { // printging the id, pid and command
                printf("%d    %d    %s\n", i + 1, getpid(), history[i]);
            }
        }

        if (input[0] == '!') { // executing a specific command from history
            // execute the command from history
            int n = input[1] - '0';
            if (n > history_count || n < 1) {
                printf("Invalid n or Such a command is NOT in history!\n");
                continue;
            } else {
                strcpy(input, history[n - 1]);
            }
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
            if (execvp(args[0], args) == -1) { // child process case
                perror("execvp");
                exit(1); // Terminate child process

            }
        } else if (pid > 0) { // parent process case
            wait(NULL); // wait for child process to complete
        } else {
            perror("fork");
        }
        history_count++;
    }
    return 0;
}

