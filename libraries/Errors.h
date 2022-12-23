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
    void cannot_open_dir();
}errors;

//    internal meaning, implemented by this program
void Errors::internal_command_not_found() {
    display.display_message_with_endl("\nCommand not found!\n");
}

void Errors::shell_popen_failed() {
    display.display_message_with_endl("\nWhen running in shell, popen failed\n");
}

void Errors::too_many_arguments() {
    display.display_message_with_endl("\nToo many arguments!\n");
}

void Errors::invalid_path_provided() {
    display.display_message_with_endl("\nInvalid path provided!\n");
}

void Errors::cannot_open_dir() {
    display.display_message_with_endl("\nCannot open the current directory, you are probably missing permisions to do so!\n");
}

#endif //TERMINAL_ERRORS_H
