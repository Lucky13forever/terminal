/**
 * The purpose of this header file is to analyze anything related to the user input
 * That includes: pressing arrows, typing commands
 *
 *
 */

#ifndef TERMINAL_USERINPUT_H
#define TERMINAL_USERINPUT_H

class User_Input{
public:
    void identify_internal_command(string command);
    void check_if_command_is_internal(string command);
    string detect_arrow_key(int, string);
    string user_types_command();
}user_input;

string User_Input::detect_arrow_key(int value, string result) {
    display.clear_line();

    history_command.keep_progress(result);
    if (value == KEY_UP)
        history_command.decrement();

    if (value == KEY_DOWN)
        history_command.increment();

    return history_command.desired_command();
}

string User_Input::user_types_command() {
    int key;

    string result = "";
    while ((key = getch()) != ENTER) {

        switch (key) {
            case KEY_UP:
            case KEY_DOWN:
                result = detect_arrow_key(key, result);
                display.display_after_key_press(result);
                break;
            case BACKSPACE:
                display.backspace();
                if (result.empty() == 0)
                    result.pop_back();
                break;
            default:
                printw("%c", key);
                result += key;

        }

    }
    printw("\n");
    //remove spaces in front
    while(result[0] == ' ')
    {
        result.erase(0, 1);
    }
    history_command.push_command(result);
    return result;
}

void User_Input::check_if_command_is_internal(string command)
{

    //i should remove the space in front
    if ( command.rfind(string(EXIT).append(" "), 0) == 0 or strcmp(command.c_str(), EXIT) == 0 )
    {
        exit_command.run(command);
        return;
    }
    if ( command.rfind(string(CLEAR).append(" "), 0) == 0 or strcmp(command.c_str(), CLEAR) == 0 )
    {
        clear_command.run(command);
        return;
    }
    if ( command.rfind(string(HISTORY).append(" "), 0) == 0 or strcmp(command.c_str(), HISTORY) == 0 )
    {
        history_command.run(command);
        return;
    }

    if ( command.rfind(string(PWD).append(" "), 0) == 0 or strcmp(command.c_str(), PWD) == 0 )
    {
        pwd_command.run(command);
        return;
    }

    if ( command.rfind(string(CD).append(" "), 0) == 0 or strcmp(command.c_str(), CD) == 0 )
    {
        cd_command.run(command);
        return;
    }

    if ( command.rfind(string(LS).append(" "), 0) == 0 or strcmp(command.c_str(), LS) == 0 )
    {
        ls_command.run(command);
        return;
    }

    terminal.check_if_external_command_exists(command);
}
#endif //TERMINAL_USERINPUT_H
