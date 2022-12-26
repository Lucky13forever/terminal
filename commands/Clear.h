//
// Created by area51 on 08.12.22.
//

#ifndef PROJECT_CLEAR_H
#define PROJECT_CLEAR_H
class Clear{
public:
    void run(string); // clear_tool the entire screen
    void backspace(); // remove the last n chars from the screen
    void clear_line(); // clear_tool the last written line
}clear_command;

void Clear::clear_line(){

    int y, x;
//    get current cursor position
    getyx(stdscr, y, x);
//    move to beginning of line
    move(y, 0);
//    delete from start till end of line
    clrtoeol();
}

void Clear::backspace(){
    int y, x;
    getyx(stdscr, y, x);
//        to make sure the PREFIX isnt touched
    if (x - 1 < display.get_prefix_length())
    {
        return;
    }
    move(y, x - 1);
    clrtoeol();
}

void Clear::run(string command="") {
    display.clear_screen(true);
}
#endif //PROJECT_CLEAR_H
