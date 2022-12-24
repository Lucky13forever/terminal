#include <unistd.h>
#include <cstdio>
#include <cstring>
using namespace std;
#include <iostream>
int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Create a child process to execute the command
    string result;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // This is the child process

        // Close the read end of the pipe
        close(pipefd[0]);

        // Redirect the stdout of the child process to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Execute the command
        char* program = "file";
        char* args[] = {"file", "terminal", NULL};
        execvp(program, args);

        // If execution fails, the function will return and set errno
        perror("execvp");
        return 1;
    } else {
        // This is the parent process

        // Close the write end of the pipe
        close(pipefd[1]);

        // Read the output from the pipe
        char buffer[1024];
        ssize_t num_read;

        while ((num_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            result += buffer;
        }
        if(!result.empty())
        {
            if (result.find("executable") != -1)
            {
                cout << "executable file";
            }
        }
        // Close the read end of the pipe
        close(pipefd[0]);
    }
    return 0;
}