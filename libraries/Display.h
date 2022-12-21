//
// Created by area51 on 08.12.22.
//

#ifndef PROJECT_DISPLAY_H
#define PROJECT_DISPLAY_H
using namespace std;
class Display{
private:
    int prefix_length; // backspace could potentially delete the prefix, so each time we display_new_command() we also change thi
    vector<string>prefix;
    vector<int>colors;
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
    void display_prefix(vector<string>, vector<int>);
}display;

void Display::display_prefix(vector<string> prefix, vector<int> colors)
{
    this->prefix = prefix;
    this->colors = colors;

    prefix_length = 0;
    for(int i=0; i<prefix.size(); i++)
    {
        prefix_length += prefix[i].length();
        attron(COLOR_PAIR( colors[i] ));
        printw("%s", prefix[i].c_str());
        attroff(COLOR_PAIR( colors[i] ));
    }
}

//used to show the user, that the code waits for a command
void Display::display_new_command(int (*Output_Stream) (const char *, ...), string message)
{
    Output_Stream("%s", message.c_str());
    prefix_length = message.size();
}

void Display::display_after_key_press(string result)
{
    display.display_prefix(this->prefix, this->colors);
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
