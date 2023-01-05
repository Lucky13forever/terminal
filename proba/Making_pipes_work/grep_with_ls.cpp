#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <iostream>
#define BUFFER_SIZE 1024;
using namespace std;

int main(int argc, char *argv[]) {

    initscr();

    string result;
    string all = "build_and_run\n"
                 "bom\n"
                 "just_one\n"
                 "again\n";

    //0 - read
    //1 - write
    int pipe_fd[2];
    int history_of_0 = pipe_fd[0];
    pipe(pipe_fd);

    int main_pid = fork();
    if (main_pid == 0){


        int second_fd[2];
        pipe(second_fd);
        int second_pid = fork();
        if (second_pid == 0) {
            // This is the child process

            // Close the write end of the pipe
            close(second_fd[1]);

            // Redirect the read end of the pipe to the STDIN of the child
            dup2(second_fd[0], STDIN_FILENO);

            // Run the "grep" command using execvp
            char *args[] = {"sort",  NULL};

            dup2(pipe_fd[1], STDERR_FILENO);
            dup2(pipe_fd[1], STDOUT_FILENO);
            execvp(args[0], args);

            // If execvp returns, it must have failed
            perror("execvp failed");
            exit(1);
        } else {
            // This is the parent process

            // Close the read end of the pipe
            close(second_fd[0]);

            // Write the output of "ls" to the write end of the pipe
            write(second_fd[1], all.c_str(), all.length());
            close(second_fd[1]);

            // Wait for the child to complete
            wait(NULL);
        }
    } else {

        //no need for stdin anymore
        dup2(pipe_fd[0], history_of_0);
        //if execvp runs but returns an error, the error code should be different then 0
        close(pipe_fd[1]);

        int status;
//        string result;
        waitpid(main_pid, &status, 0);
        if (WIFEXITED(status)){
            int es = WEXITSTATUS(status);
            if (es == 0)
            {
                //success;
            }
            else {
                //got an error, so result will keep the error
            }
            int num_of_bytes;
            char buffer[1024];
            while ( ( num_of_bytes = read(pipe_fd[0], buffer, 1024) ) > 0 )
            {
                result += buffer;
            }
        }
        close(pipe_fd[0]);
    }
    printw(result.c_str());
    refresh();
    getch();
    endwin();
    return 0;
}




