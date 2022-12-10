//
// Created by area51 on 10.12.22.
//

#ifndef TERMINAL_APPLICATION_H
#define TERMINAL_APPLICATION_H
class Application{
public:
    void start_terminal();
}application;

void Application::start_terminal()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    scrollok(stdscr,TRUE);
    noecho();
    while (terminal.running())
    {
        display.display_new_command(PREFIX);
        string result = user_input.user_types_command();
        user_input.check_if_command_is_internal(result);
    }
    refresh();
    endwin();
}
#endif //TERMINAL_APPLICATION_H
