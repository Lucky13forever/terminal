//
// Created by area51 on 08.12.22.
//

#ifndef PROJECT_DISPLAY_H
#define PROJECT_DISPLAY_H
using namespace std;
class Display{
private:
    int prefix_length; // backspace could potentially delete the prefix, so each time we display_new_command() we also change thi
public:
    void clear_screen(); // clear_tool the entire screen
    void backspace(); // remove the last n chars from the screen
    void clear_line(); // clear_tool the last written line
    void display_new_command(int (*Output_Stream) (const char *, ...), string);
    void display_new_command(string);
    void display_char(char);
    void display_after_key_press(string);
    void display_message(string message) { printw("%s", message.c_str());};
    const int get_prefix_length() {return prefix_length;};
    void display_shell_runned_command(string);
}display;

//used to show the user, that the code waits for a command
void Display::display_new_command(int (*Output_Stream) (const char *, ...), string message)
{
    Output_Stream("%s", message.c_str());
    prefix_length = message.size();
}

void Display::display_after_key_press(string result)
{
    display.display_new_command(&printw, PREFIX);
    printw("%s", result.c_str());
}

void Display::clear_line(){

    int y, x;
//    get current cursor position
    getyx(stdscr, y, x);
//    move to beginning of line
    move(y, 0);
//    delete from start till end of line
    clrtoeol();
}

void Display::backspace(){
    int y, x;
    getyx(stdscr, y, x);
//        to make sure the PREFIX isnt touched
    if (x - 1 < display.get_prefix_length())
    {
        return;
    }
//        printw("%d | %dt\n", x - i, display.get_prefix_length());
    move(y, x - 1);
    clrtoeol();
}

void Display::clear_screen() {
    erase();
}

void Display::display_new_command(string command) {
    display_new_command(printw, command);
}

void Display::display_char(char c) {
    printw("%c", c);
}
#endif //PROJECT_DISPLAY_H
