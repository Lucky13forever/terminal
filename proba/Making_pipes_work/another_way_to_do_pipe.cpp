#include <ncurses.h>
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <cstring>
#include <string.h>
using namespace std;

string ls(void) {
    // Open the current directory
    DIR *dir = opendir(".");
    if (dir == NULL) {
        // opendir failed, return an error message
        return "Error opening directory\n";
    }

    // Allocate a buffer to hold the output of the "ls" command
    string output;
    if (output.empty()) {
        // malloc failed, return an error message
        closedir(dir);
        return "Error allocating memory\n";
    }
    output[0] = '\0';

    // Read the contents of the directory and store the names in the output buffer
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        output += entry->d_name;
        output += "\n";
    }

    // Close the directory and return the output
    closedir(dir);
    return output;
}

int main() {
    // Initialize NCursed
    FILE *fd = fopen("/dev/tty", "r+");
    SCREEN *scr = newterm(NULL, fd, fd);

    // Execute the "ls" command and store the result in a buffer
    string ls_output = ls();
    string result;

    // Create a pipe to communicate with the "grep" command
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        // pipe failed, print an error message and exit
        printw("Error creating pipe\n");
        endwin();
        return 1;
    }

    // Fork the process
    pid_t pid = fork();
    if (pid == -1) {
        // fork failed, print an error message and exit
        printw("Error forking process\n");
        endwin();
        return 1;
    }

    if (pid == 0) {
        // This is the child process, which will execute the "grep" command

        // Close the write end of the pipe
        close(pipefd[1]);

        // Replace the standard input with the read end of the pipe
//        dup2(pipefd[0], STDIN_FILENO);

        // Execute the "grep" command
        char *argv[] = {"grep", "a", NULL};
        execvp("grep", argv);

        // execvp failed, print an error message and exit
        printw("Error executing command\n");
        endwin();
        return 1;
    } else {
        // This is the parent process, which will write the output of the "ls" command to the pipe

        // Close the read end of the pipe
        close(pipefd[0]);

        // Write the output of the "ls" command to the pipe
        write(pipefd[1], ls_output.c_str(), ls_output.length());

        // Close the write end of the pipe
        close(pipefd[1]);

        // Wait for the child process to finish
        wait(NULL);

        //after waiting get the content
        int num_of_bytes;
        char buffer[1024];
        printw("acii");
        while ( ( num_of_bytes = read(pipefd[0], buffer, 1024) ) > 0 )
        {
            result += buffer;
        }

    }
    printw("hey man");
    getch();

    endwin();
    return 0;
}