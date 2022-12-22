//
// Created by area51 on 22.12.22.
//

#ifndef TERMINAL_CD_H
#define TERMINAL_CD_H
class Cd{
public:
    void run(string);
    bool is_valid(string command);
    bool validate();
    void action();
}cd_command;

bool Cd::validate()
{
    //if to much arguments -> error
    if (scanner.get_arguments().size() >= 2 )
    {
        errors.too_many_arguments();
        return false;
    }
    return true;
}

void Cd::run(string command) {
    //only one argument

    display.display_debug("RUNNING CD COMMAND");
    scanner.scan_command(command);

    if(validate())
    {
        action();
    }
}

void Cd::action() {
    //if no arguments path goes to /
    if (scanner.get_arguments().size() == 0)
    {
        terminal.update_path("/");
        return;
    }

    //if only one argument, as it should be, it's clear if i reach this line size is 1
    terminal.update_path(scanner.get_arguments()[0]);
}

#endif //TERMINAL_CD_H
