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

    void no_arguments();

    void no_such_file(string);

    void flag_has_no_value(const string &basicString);

    void syntax_error(const string &sign);
}errors;

//    internal meaning, implemented by this program
void Errors::internal_command_not_found() {
    display.display_message_with_endl("Command not found!");
}

void Errors::shell_popen_failed() {
    display.display_message_with_endl("When running in shell, popen failed");
}

void Errors::too_many_arguments() {
    display.display_message_with_endl("Too many arguments!");
}

void Errors::no_arguments(){
    display.display_message_with_endl("Missing arguments!");
}

void Errors::invalid_path_provided() {
    display.display_message_with_endl("Invalid path provided!");
}

void Errors::cannot_open_dir() {
    display.display_message_with_endl("Cannot open the current directory, you are probably missing permisions to do so!");
}

void Errors::no_such_file_or_directory(string file_name) {
    display.display_message_with_endl("Cannot access '" + file_name + "'" + " -> No such file or directory!");
    display.display_message_with_endl("");
}

void Errors::invalid_flag_provided(string flag) {
    display.display_message_with_endl("Unknown flag, '" + flag + "'" + " -> Invalid flag provided!");
}

void Errors::use_help_flag_on_command(string command) {
    display.display_message_with_endl("Try running the '" + command + " --help command for more details!");
}

void Errors::no_such_file(string file_name) {
    display.display_message_with_endl("cannot open '" + file_name + "'" + " -> No such file!");
}

void Errors::flag_has_no_value(const string & flag) {
    display.display_message_with_endl("The flag '" + flag + "' hasn't received a value");
}

void Errors::syntax_error(const string & sign)
{
    display.display_message_with_endl("syntax error near unexcepted token '" + sign + "'. Valid options are '>', '>>' and '|' .");
}

#endif //TERMINAL_ERRORS_H
