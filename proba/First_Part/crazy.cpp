#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
using namespace std;
int main()
{
    char * argv[] = {"./fix_error_on_same_line", NULL};

    int pipe_fd[2];
    pipe(pipe_fd);
    dup2();
    FILE * file = popen(argv[0], "r");

    char * buffer = new char[1024];
    while (fgets(buffer, 1024, file) != NULL)
    {
        cout << 1 << "\n";
    }

    return 0;
}