//
// Created by area51 on 10.12.22.
//

#ifndef TERMINAL_ERRORS_H
#define TERMINAL_ERRORS_H
class Errors{
public:
    void internal_command_not_found();
    void shell_popen_failed();
}errors;

//    internal meaning, implemented by this program
void Errors::internal_command_not_found() {
    display.display_message("\nCommand not found!\n");
}

void Errors::shell_popen_failed() {
    display.display_message("\nWhen running in shell, popen failed\n");
}
#endif //TERMINAL_ERRORS_H
