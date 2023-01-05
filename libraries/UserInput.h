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
    MEVENT event;
    while ((key = getch()) != ENTER) {


        if (key == KEY_MOUSE)
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON5_PRESSED)
                {
                    //down
                    display.scroll_down();
                }
                if (event.bstate & BUTTON4_PRESSED)
                {
                    //up
                    display.scroll_up();
                }
            }
        }
        else{

            //listen only if i am at last line
            if(display.am_i_at_the_last_line())
            {

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

                        display.display_char(key);
                        result += key;

                }

            }

        }


    }
    //enter
    display.display_message_with_endl("");
    //remove spaces in front
    while(result[0] == ' ')
    {
        result.erase(0, 1);
    }
    if (!result.empty())
        history_command.push_command(result);
    return result;
}

void User_Input::check_if_command_is_internal(string full_command)
{
    //enter was pressed -> remove everything from cache
    display.clear_cache();
    //stop the display
    display.setShowCache(false);


    Scanner user_input_scanner = *new Scanner();
    //command can be piped like ls | grep "smth", now the scanner will be the one who splits those commands

    string command;
    string result_from_prev_command;
    Raw_command raw;
    vector<Raw_command> raw_commands = user_input_scanner.split_command(full_command);
    Scanner command_scanner = *new Scanner();
    for (int i=0; i<raw_commands.size(); i++)
    {
        raw = raw_commands[i];

        display.display_debug_file("This command is =======");
        display.display_debug_file(raw.getCommand());
        display.display_debug_file("==========");

        if (i == raw_commands.size() - 1)
        {
            //if i have pipes or redirection -> only the final output i am interested in
            display.setShowCache(true);
        }
        //clear display cache
        result_from_prev_command = display.getCache();
        display.clear_cache();

        //get the normal command, trim it
        command = raw.getCommand();
        //trim

        command = user_input_scanner.trim(command);

        string internal_command = command + " " + result_from_prev_command;


        if ( command.rfind(string(EXIT).append(" "), 0) == 0 or strcmp(command.c_str(), EXIT) == 0 )
        {
            exit_command.run(command);
            return;
        }
        if ( command.rfind(string(CLEAR).append(" "), 0) == 0 or strcmp(command.c_str(), CLEAR) == 0 )
        {
            clear_command.run(command);
            continue;
        }
        if ( command.rfind(string(HISTORY).append(" "), 0) == 0 or strcmp(command.c_str(), HISTORY) == 0 )
        {
            history_command.run(command);
            continue;
        }

        if ( command.rfind(string(PWD).append(" "), 0) == 0 or strcmp(command.c_str(), PWD) == 0 )
        {
            pwd_command.run(command);\
            continue;
        }

        if ( command.rfind(string(CD).append(" "), 0) == 0 or strcmp(command.c_str(), CD) == 0 )
        {
            cd_command.run(command);
            continue;
        }

        if ( command.rfind(string(LS).append(" "), 0) == 0 or strcmp(command.c_str(), LS) == 0 )
        {
            ls_command.run(internal_command);
            continue;
        }

        terminal.check_if_external_command_exists(command, result_from_prev_command);
    }

}
#endif //TERMINAL_USERINPUT_H
