//
// Created by area51 on 10.12.22.
//

#ifndef TERMINAL_EXIT_H
#define TERMINAL_EXIT_H
class Exit{
public:
    const int get_state() {return terminal.get_state();};
    void run(string);
}exit_command;

void Exit::run(string command = "")
{
    terminal.terminate();
}
#endif //TERMINAL_EXIT_H
