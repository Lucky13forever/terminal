//https://www.youtube.com/watch?v=6xbLgZpOBi8
#include <bits/stdc++.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;
int main()
{
    string show;
    initscr();
    int fd[2];
    int keep[2];
    if (pipe(fd) == -1)
    {
        return 1;
    }

    pipe(keep);
    int pid1 = fork();
    if (pid1 == 0){

        string result = "here\n"
                        "i\n"
                        "go\n"
                        "again\0";
//        result = "unu.txt";
        write(fd[1], result.c_str(), result.length());
//        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        close(keep[0]);
        close(keep[1]);
        return 0;
    }

    int pid2 = fork();
    if (pid2 == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        dup2(keep[1], STDOUT_FILENO);//in output i keep the result
        dup2(keep[1], STDERR_FILENO);//keep the errors
        close(fd[0]);
        close(fd[1]);
        close(keep[0]);
        close(keep[1]);
        char * argv[] = {"sort", "-r", NULL};
        execvp(argv[0], argv);
    }

    close(fd[0]);
    close(fd[1]);
    close(keep[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    if (pid2 and pid1){

        char buffer[1025];
        int num_bytes;
        while( (num_bytes = read(keep[0], buffer, 1024)) > 0 )
        {
            buffer[num_bytes] = 0;
            show += buffer;
        }
//        printw(show.c_str());
        close(keep[0]);


    }
    printw(show.c_str());
    printw("finished");
    getch();
    endwin();
    return 0;
}