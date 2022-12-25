//
// Created by area51 on 22.12.22.
//
#ifndef TERMINAL_CD_H
#define TERMINAL_CD_H
class Cd{
public:
    void run(string);
    bool is_valid(string command);
    bool validate();
    void action();

    string is_this_a_valid_path(string path);
    string remove_last_slash(string word);
}cd_command;

bool Cd::validate()
{
    //if to much arguments -> error
    if (scanner.get_arguments().size() >= 2 )
    {
        errors.too_many_arguments();
        return false;
    }
    return true;
}

void Cd::run(string command) {
    //only one argument

    display.display_debug("RUNNING CD COMMAND");
    scanner.scan_command(command);

    if(validate())
    {
        action();
    }
}

void Cd::action() {
    //if no arguments path goes to /
    string true_path;
    if (scanner.get_arguments().size() == 0)
    {
        terminal.update_path("/");
        return;
    }

    //if only one argument, as it should be, it's clear if i reach this line size is 1
    true_path = is_this_a_valid_path(scanner.get_arguments()[0]);

    if (true_path.empty())
    {
        //path invalid
        errors.invalid_path_provided();
        return;
    }

    if (true_path == ".")
    {
        //no error
        // no need to do smth
        return;
    }
    //if it's valid -> send to the terminal so it change it
    terminal.update_path(true_path);
}

//if i am in /home/area51/faculta and the arg is ../..
//then instead of /home/area51/faculta/../.. if should display only /home

string Cd::is_this_a_valid_path(string path){


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
        return ".";
    }
    if (path == "..")
    {
        true_path = remove_last_slash(terminal.getPath());
        display.display_debug("Removing the last slash new path will be -> " + true_path);
        return true_path;
    }
    if (path[0] != '/')
    {
        true_path = terminal.getPath();
        //if im in / directory then i will have // as a prefix to the path, in this case remove the first one
        if (true_path[ true_path.size() - 1 ] != '/')
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

string Cd::remove_last_slash(string word) {
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

#endif //TERMINAL_CD_H
