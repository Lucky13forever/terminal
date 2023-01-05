//
// Created by area51 on 08.12.22.
//
//TODO selecting in terminal doesnt work anymore
#ifndef PROJECT_DISPLAY_H
#define PROJECT_DISPLAY_H
using namespace std;

//SE - start, end
struct SE{
    int start_line;
    int end_line;
};

//this will keep track of every line in the terminal
//it's job is to keep, no to display
class Display{
private:
    int prefix_length; // backspace could potentially delete the prefix, so each time we display_new_command() we also change thi
    vector<string>prefix;
    vector<int>colors;
    int current_line_pos=0; //what is the last line on screen index? At what line index is the cursor?
    string cache; //keep track of everything shown on screen, usefull to keep track of command output
    bool show_cache = true;

public:
    void clear_screen(bool from_command = false); // clear_tool the entire screen
    void backspace(); // remove the last n chars from the screen
    void clear_line(); // clear_tool the last written line
    void display_new_command(string);
    void display_char(char);
    void display_after_key_press(string);
    void display_message_with_endl(string message, bool keep = true);
    const int get_prefix_length() {return prefix_length;};
    void display_shell_runned_command(string);
    void display_prefix(vector<string>, vector<int>);
    void display_debug(string);
    void display_message(string);
    void display_message_with_color(string, int, bool keep = true);
    void display_front_spaces(string longest, string normal);

    void display_debug_file(string message);

    void display_front_spaces(int longest, int normal);

    void display_history_line(int i, const char *string);

    void scroll_down();

    void scroll_up();

    bool am_i_at_the_last_line();

    struct SE starting_line_when_scrolling_up();

    int starting_line_when_scrolling_down();

    void scroll_effect(SE index);

    void display_endl();
    void clear_cache() {cache.erase();};

    const string &getCache() const;

    void setShowCache(bool showCache);

    void display_external_command_output(string basicString);
}display;

void Display::display_prefix(vector<string> prefix, vector<int> colors)
{
    //clear anything in front
    clear_line();
    this->prefix = prefix;
    this->colors = colors;

    prefix_length = 0;
    for(int i=0; i<prefix.size(); i++)
    {
        prefix_length += prefix[i].length();
        display_message_with_color(prefix[i], colors[i]);
    }
}

void Display::display_after_key_press(string result)
{
    //woh, so if i try to use history -> then i must clear the last line from scroll_object
    scroll_object.clear_last_line();
    display.display_prefix(this->prefix, this->colors);
    display_message(result);
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

    //when backspace is activated, meaning i reach this line
    //backspace can remove only one char at time, so if it was activated, it can only be active at user input, so it's ok to remove the last message
    //definetely the last message is a char
//    scroll_object.delete_last_message_from_current_line();
    scroll_object.delete_last_char_from_current_line();
}

void Display::clear_screen(bool from_command) {
    erase();

    if (from_command)
    {
        //do this only if the clear_command sends it
        //when clearing the screen, clear the memory of scroll also
        scroll_object.clear();
        //also update the index
        current_line_pos = 0;
    }
}

void Display::display_new_command(string message) {
    display_message(message);
    prefix_length = message.size();
}

void Display::display_char(char c) {
    string message;
    message += c;
    display_message(message);
}

void Display::display_debug(string debug) {
    if (DEBUG == 1 or DEBUG == 3){
        printw("\n------------------------------\n\n");
        printw("%s", debug.c_str());
        printw("\n\n------------------------------\n");
    }
}

void Display::display_debug_file(string message) {
    if (DEBUG >= 2){

        string command = "echo '";
        command += message + "'" + " >> debug_file.txt";
        FILE * file = popen(command.c_str(), "r");
        pclose(file);
    }
}

void Display::display_front_spaces(int longest, int normal) {
    //12345 - longest
    //  123 - normal

//    scope, display those 2 extra space in front
    string extra_spaces;
    for(int i=0; i<longest - normal; i++)
    {
        extra_spaces += " ";
    }
    display.display_message(extra_spaces);
}






void Display::display_message(string command) {
    display_message_with_color(command, COLOR_WHITE_CODE);
}

//every message will go through this function, this is where scroll will get updated
//keep means keep in scroll_object;
void Display::display_message_with_color(string message, int color, bool keep) {

    cache += message;

    if (!display.show_cache)
        return;

    attron(COLOR_PAIR( color ));
    printw("%s", message.c_str());
    attroff(COLOR_PAIR( color ));

    if(keep)
        scroll_object.add_message_to_current_line(message, color);
}

void Display::display_endl()
{
    //also keep \n in the result;
    cache += "\n";
    if (display.show_cache)
        printw("\n");
}

void Display::display_message_with_endl(string message, bool keep) {
    display_message(message);

    if (display.show_cache and keep){
        //when "\n" encounterd i need to make new line
        scroll_object.new_line();

        //this means cursor went down
        current_line_pos += 1;
    }

    display_endl();

}

bool Display::am_i_at_the_last_line()
{
    //cause current_line_pos is the actual index -> therefore the -1
    return current_line_pos == scroll_object.get_nr_of_lines() - 1;
}

void Display::scroll_up()
{
    int screen_rows, screen_cols;
    getmaxyx(stdscr, screen_rows, screen_cols);

    //    if i exceed the screen capacity
    if (current_line_pos > screen_rows - 1)
    {
        struct SE index;
        index.end_line = current_line_pos - 1;
        index.start_line = index.end_line - screen_rows + 1;
        //cause i went up
        current_line_pos = index.end_line;
        this->scroll_effect(index);
    }


}

void Display::scroll_down()
{
    int screen_rows, screen_cols;
    getmaxyx(stdscr, screen_rows, screen_cols);

    //               i am not at the last line                            the nr of lines exceeds the screen capacity
    if (current_line_pos < scroll_object.get_nr_of_lines() - 1 and scroll_object.get_nr_of_lines() - 1 > screen_rows - 1)
    {
        struct SE index;
        index.start_line = current_line_pos - screen_rows + 1;
        index.end_line = current_line_pos + 1;
        current_line_pos = index.end_line;
        this->scroll_effect(index);
    }

}

void Display::scroll_effect(struct SE index) {
    //clear the screen
    display.clear_screen();
    for(int i=index.start_line; i<=index.end_line; i++)
    {
        Line line_to_display = scroll_object.get_line(i);
        for (Message messages_to_display : line_to_display.getMessages())
        {
            display.display_message_with_color(messages_to_display.getMessage(), messages_to_display.getColor(), false);
        }
        if (i != index.end_line )
            display.display_endl();
    }
}

const string &Display::getCache() const {
    return cache;
}

void Display::setShowCache(bool showCache) {
    show_cache = showCache;
}

void Display::display_external_command_output(string result) {
    int start = 0;
    int end = result.find('\n', start);
    while(end != std::string::npos)
    {
        string substring = result.substr(start, end - start);

        display.display_message_with_endl(substring);

        start = end + 1;

        end = result.find('\n', start);
    }
}


#endif //PROJECT_DISPLAY_H
