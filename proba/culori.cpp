#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
using namespace  std;
int main() {

    char * const argv[] = {"/bin/ls", "-l", "-a", NULL};



    pid_t child_pid = fork();

    int result = -1;
    if (child_pid == 0)
    {
//        in child
        result = execv("/bin/ls", argv);
        result = 5;
    }
    else{
        int status;
        wait(&status);
        //in parent
        cout << "BEFORE";
        cout << "RESULT\n" << result << "\n";
        cout << "CE PIZDA MASII";
    }

    return 0;
}