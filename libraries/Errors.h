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
    void no_such_file_or_directory(string);
    void invalid_flag_provided(string);
    void use_help_flag_on_command(string);
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

void Errors::no_such_file_or_directory(string file_name) {
    display.display_message_with_endl("\nCannot access '" + file_name + "'" + " -> No such file or directory!\n");
}

void Errors::invalid_flag_provided(string flag) {
    display.display_message_with_endl("\nUnknown flag, '" + flag + "'" + " -> Invalid flag provided!\n");
}

void Errors::use_help_flag_on_command(string command) {
    display.display_message_with_endl("\nTry running the '" + command + " --help command for more details!\n");
}

#endif //TERMINAL_ERRORS_H
