//
// Created by area51 on 10.12.22.
//
#ifndef TERMINAL_TERMINAL_H
#define TERMINAL_TERMINAL_H
class Terminal{
    int state = RUNNING;

    string path;

    vector<string>prefix = {"area51", ":", "~", "$ "};
    vector<int>color_pairs = {2, 1, 3, 1};
public:
    const vector<string> &getPrefix() const;

    const vector<int> &getColorPairs() const;

    void update_current_path();

public:
    const int get_state() {return state;};
    void terminate() {state = TERMINATED;};
    bool running() {return state == RUNNING;};
    void display_shell_runned_command(string);
    Terminal();
    void configure();
}terminal;

Terminal::Terminal()
{
    update_current_path();
}

void Terminal::configure()
{
    update_current_path();
}

void Terminal::update_current_path(){
    char buffer[1024];
    path = "~";
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        path.append(buffer);
    }
    prefix[2] = path;
}

void Terminal::display_shell_runned_command(string command) {
    FILE *fpipe;

    char c = 0;

    if (0 == (fpipe = (FILE*)popen(command.c_str(), "r")))
    {
        errors.shell_popen_failed();
        return;
    }

    while (fread(&c, sizeof c, 1, fpipe))
    {
        display.display_char(c);
    }
    pclose(fpipe);
}

const vector<string> &Terminal::getPrefix() const {
    return prefix;
}

const vector<int> &Terminal::getColorPairs() const {
    return color_pairs;
}

#endif //TERMINAL_TERMINAL_H
