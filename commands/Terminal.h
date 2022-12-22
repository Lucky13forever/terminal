//
// Created by area51 on 10.12.22.
//
#ifndef TERMINAL_TERMINAL_H
#define TERMINAL_TERMINAL_H
class Terminal{
    int state = RUNNING;
public:
    const string &getPath() const;

private:

    string path;

    vector<string>prefix = {"area51", ":", "~", "$ "};
    vector<int>color_pairs = {2, 1, 3, 1};
public:
    const vector<string> &getPrefix() const;

    const vector<int> &getColorPairs() const;

    void initialize_path();
    void update_path(string);



public:
    const int get_state() {return state;};
    void terminate() {state = TERMINATED;};
    bool running() {return state == RUNNING;};
    void display_shell_runned_command(string);
    Terminal();
    void configure();
    void execute_external_command(string);
    bool this_command_exists(string);
    string extract_name_of_command(string);
    string is_this_a_valid_path(string);
    string remove_last_slash(string word);
}terminal;

Terminal::Terminal()
{
    initialize_path();
}

void Terminal::configure()
{
    initialize_path();
}

void Terminal::initialize_path(){
    char buffer[1024];
    path = "";
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        path.append(buffer);
    }
    prefix[2] = "~";
    prefix[2].append(path);
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

string Terminal::extract_name_of_command(string command){
    //check if i have a space in the command
    int space_found_at = command.find(" ");

    string rez;
    if (space_found_at != -1)
    {
        rez = command.substr(0, space_found_at);
    }
    else {
        //no space, then this is just the name of the command, no parameters
        rez = command;
    }

    return rez;

}


void Terminal::execute_external_command(string command) {

    //extract the name of the command
    string name = extract_name_of_command(command);

    if (this_command_exists(name) == 1)
    {
        //run exec on it
    }
    else{
        errors.internal_command_not_found();
    }
}

bool Terminal::this_command_exists(string name)
{
    string command = "command -v ";
    command.append(name);
    FILE * pipe = popen(command.c_str(), "r");

    string result = "";
    if (pipe)
    {

        //can open and read
        char * buffer = new char[128];
        if (fgets(buffer, 128, pipe) != NULL)
        {
            result += buffer;
        }

        if (!result.empty())
        {
            //this exists
            return true;
        }

    } else {
        errors.shell_popen_failed();
    }

    pclose(pipe);
    return false;
}

const string &Terminal::getPath() const {
    return path;
}

string Terminal::is_this_a_valid_path(string path){


    struct stat sb;
    // if the user also provided a / at the end, it can be removed
    if (path[ path.size() - 1 ] == '/')
    {
        path.pop_back();
    }

    //if path doesnt start with / -> the user intention is to continue from current path

    string true_path = path;
    string mt;


    if (path == ".")
    {
        //do nothing
        return mt;
    }
    if (path == "..")
    {
        true_path = remove_last_slash(this->path);
        display.display_debug("Removing the last slash new path will be -> " + true_path);
        return true_path;
    }
    if (path[0] != '/')
    {
        true_path = this->path;
        true_path.append("/");
        true_path.append(path);
    }

    if (stat(true_path.c_str(), &sb) == 0)
    {
        display.display_debug(true_path + " is valid -> changing");
        return true_path;
    }
    display.display_debug(true_path + " is invalid, ignoring");
    return mt;
}

void Terminal::update_path(string path) {
    //is the path valid?
    string rez = is_this_a_valid_path(path);
    if (!rez.empty())
    {
        display.display_debug("Variable this->path chaging to " + rez);
        this->path = rez;
        prefix[2] = "~";
        prefix[2].append(rez);
        display.display_debug("Prefix[2] is now " + prefix[2]);
        return;
    }
    //if im here the path was invalid
    errors.invalid_path_provided();
}

string Terminal::remove_last_slash(string word) {
//    first let's find if there is a slash
    int pos_of_last_slash = word.find_last_of("/");

    string mt;

    string debug_message = "Position of last slash is";
    debug_message.append(to_string(pos_of_last_slash) );
    display.display_debug(debug_message);
    if (pos_of_last_slash != -1)
    {
        //if i have one -> then i can remove this from the path
        return word.substr(0, pos_of_last_slash);
    }
    return mt;
}

#endif //TERMINAL_TERMINAL_H
