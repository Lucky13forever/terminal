#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

char ** return_char_pointer_from_vector_of_strings(vector<string> argv_vector)
{
    char ** argv = new char *[argv_vector.size() + 1];

    for(int i=0; i<argv_vector.size(); i++)
    {
        argv[i] = new char[ argv_vector[i].length() + 1 ];
        strcpy(argv[i], argv_vector[i].c_str());
        argv[i][argv_vector[i].length()] = 0;
    }
    return argv;
}

string command = "cat";
string argument = "unu.txty76127";
string mt;
vector<string> argv_vector = {command, argument};

int main()
{

    int pipe_fd[2];
    pipe(pipe_fd);

    int pid = fork();
    if (pid == 0){
        //child
        close(pipe_fd[0]);

        char ** argv = return_char_pointer_from_vector_of_strings(argv_vector);

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
            string result;
            int num_of_bytes = 0;
            char buffer[1024];
            while ( ( num_of_bytes = read(pipe_fd[0], buffer, 1024) ) > 0 )
            {
                result += buffer;
            }
            cout << result;
        }
        close(pipe_fd[0]);
    }
    return 0;
}