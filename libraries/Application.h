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
//    raw();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr,TRUE);
    mousemask(BUTTON4_PRESSED | BUTTON5_PRESSED, NULL);
    start_color();

    init_pair(COLOR_WHITE_CODE, COLOR_WHITE, 0);
    init_pair(COLOR_GREEN_CODE, COLOR_GREEN, 0);
    init_pair(COLOR_CYAN_CODE, COLOR_CYAN, 0);
    init_pair(COLOR_BLUE_CODE, COLOR_BLUE, 0);
    init_pair(COLOR_RED_CODE, COLOR_RED, 0);
    init_pair(COLOR_YELLOW_CODE, COLOR_YELLOW, 0);
    init_pair(COLOR_MAGENTA_CODE, COLOR_MAGENTA, 0);
    bkgdset(A_NORMAL);


    terminal.configure();
    while (terminal.running())
    {
        display.display_prefix(terminal.getPrefix(), terminal.getColorPairs());
        string result = user_input.user_types_command();
        if (!result.empty())
            user_input.check_if_command_is_internal(result);
    }
    refresh();
    endwin();
}
#endif //TERMINAL_APPLICATION_H
