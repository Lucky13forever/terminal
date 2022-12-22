//
// Created by area51 on 22.12.22.
//

#ifndef TERMINAL_PWD_H
#define TERMINAL_PWD_H
class Pwd{
public:
    void run(string);
}pwd_command;

void Pwd::run(string command)
{
    display.display_message(terminal.getPath());
}
#endif //TERMINAL_PWD_H
