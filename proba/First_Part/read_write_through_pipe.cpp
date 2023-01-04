#include <bits/stdc++.h>
#include <sys/wait.h>
using namespace std;
int main()
{
//    0 - read
//    1 - write
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid == 0)
    {
        //child
        //this will read
        close(pipefd[1]);

        char * buffer = new char[1024];
        string output = "";
        while(read(pipefd[0], buffer, 1024) > 0)
        {
            output.append(buffer);
        }
        close(pipefd[0]);

        cout << "Message from father : "<<output<<"\n";
    } else {
        //father
        //this will write
        close(pipefd[0]);
        string input = "This comes from the father";
        write(pipefd[1], input.c_str(), input.size());

        close(pipefd[1]);

        int status;
        waitpid(pid, &status, 0);

    }
    return 0;
}