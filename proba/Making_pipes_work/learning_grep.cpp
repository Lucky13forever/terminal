#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    // Run the "ls" command and save the output in a variable
    char result[1024];
    FILE *fp = popen("ls", "r");
    string all;
    while (fgets(result, sizeof(result), fp) != NULL){
        all += result;
    }
    pclose(fp);

    // Create a pipe for the child process to read from
    int pipefd[2];
    pipe(pipefd);

    // Create a child process
    int pid = fork();
    if (pid == 0) {
        // This is the child process

        // Close the write end of the pipe
        close(pipefd[1]);

        // Redirect the read end of the pipe to the STDIN of the child
        dup2(pipefd[0], STDIN_FILENO);

        // Run the "grep" command using execvp
        char *cmd = "grep";
        char *args[] = {"grep", "s", NULL};
        execvp(cmd, args);

        // If execvp returns, it must have failed
        perror("execvp failed");
        exit(1);
    } else {
        // This is the parent process

        // Close the read end of the pipe
        close(pipefd[0]);

        // Write the output of "ls" to the write end of the pipe
        write(pipefd[1], all.c_str(), all.length());
        close(pipefd[1]);

        // Wait for the child to complete
        wait(NULL);
        printf("Child process complete\n");
    }

    return 0;
}




