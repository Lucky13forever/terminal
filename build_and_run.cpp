#include <unistd.h>
#include <iostream>

int main() {
    char *const argv[] = {"g++", "terminal.cpp",  "-lncurses" ,"-o", "terminal", nullptr};
    execv("/usr/bin/g++", argv);
    return 0;
}