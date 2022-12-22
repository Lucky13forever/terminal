//
// Created by area51 on 10.12.22.
//

#ifndef TERMINAL_ERRORS_H
#define TERMINAL_ERRORS_H
class Errors{
public:
    void internal_command_not_found();
    void shell_popen_failed();
    void too_many_arguments();
    void invalid_path_provided();
}errors;

//    internal meaning, implemented by this program
void Errors::internal_command_not_found() {
    display.display_message("\nCommand not found!\n");
}

void Errors::shell_popen_failed() {
    display.display_message("\nWhen running in shell, popen failed\n");
}

void Errors::too_many_arguments() {
    display.display_message("\nToo many arguments!\n");
}

void Errors::invalid_path_provided() {
    display.display_message("\nInvalid path provided!\n");
}

#endif //TERMINAL_ERRORS_H
