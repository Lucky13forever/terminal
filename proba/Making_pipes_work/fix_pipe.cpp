#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#define BUFFER_SIZE 1024;
using namespace std;

int main(int argc, char *argv[]) {

    string previous_command_result = "build_and_run";
    string result;
    int pipe_fd[2];
    pipe(pipe_fd);

    int pid = fork();
    if (pid == 0){

        //firstly send previous command result to stdin
        write(pipe_fd[1], previous_command_result.c_str(), previous_command_result.length());
        //now duplicate it
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);


        //to construct argv, i need to make a vector of all arguments, i will use scanner for this
//        char ** argv = return_char_pointer_from_vector_of_strings(terminal_scanner.get_everything());

        char * argv[] = {"grep", "a", NULL};
        //redirect everything to pipe
        dup2(pipe_fd[1], STDERR_FILENO);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execvp(argv[0], argv);

    } else {

        //if execvp runs but returns an error, the error code should be different then 0
        close(pipe_fd[1]);

        int status;
        waitpid(pid, &status, 0);
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
    cout << result;
    return 0;
}




