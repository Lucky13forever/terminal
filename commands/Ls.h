//
// Created by area51 on 22.12.22.
//

#ifndef TERMINAL_LS_H
#define TERMINAL_LS_H
using namespace std;
class Ls{

    class Accepted_flags{
    public:
        const char * a = "a";
        const char * all = "all";
        const char * F = "F";
        const char * classify = "classify";
        const char * help = "help";
        const char * l = "l";
        const char * s = "s";
    }accepted_flags;

    string help = "Usage: ls [PATH]... [FLAGS]...\n"
                  "Lists all the files inside the directory which path is what you provided\n"
                  "You can provide more paths, and it will display the result for each one, if it's a valid one\n"
                  "If you mess up the path, the command will warn you\n"
                  "If you provide more paths, but one is invalid, the command will still run for the valid paths\n"
                  "If you don't provide any path at all, the PATH will be the one you ran the command in\n"
                  "If you type any unknown flag, you will be prompted with an error message\n"
                  "Whenever a problem is encountered the command will suggest using the --help flag\n"
                  "\n"
                  "The flags that you can provide\n"
                  "-a --all               do not ignore entries starting with .\n"
                  "-F --classify          append indicator (one of *, /, =, @, |) to entries\n"
                  "                       '/' : Directory\n"
                  "                       '*' : Executable file\n"
                  "                       '@' : Symbolic link\n"
                  "                       '|' : FIFO (named pipe) file\n"
                  "                       '=' : Socket file\n"
                  "   --help              used to display this instructions\n"
                  "-l                     use a long listing format, helps for visibility\n"
                  "-s --size              print the allocated size of each file, in blocks\n";

//the structure we will keep inside a vector, meaning a row and this will contain everything so to make ls -l easier

    class Row_info{
        string name; //the file name
    public:
        explicit Row_info(const string &name) : name(name) {};
        Row_info();

        const string &getName() const {
            return name;
        }

        static bool compareByName(const Row_info &left, const Row_info &right);
    };
    vector< Row_info > rows;

    bool consider_starting_with_dot;

public:
    void run(string);
//we start with the simple ls command, with no flags and no path
//just display the current path

    void identify_next_step();

    void simple_view();

    void initialize_rows(string);

    void sort_rows_after_file_name();
}ls_command;

Ls::Row_info::Row_info() {

}

bool Ls::Row_info::compareByName(const Ls::Row_info &left, const Ls::Row_info &right) {
    //if left is starting with ., i try to extract only the letters if it even has them
    //like for .git i will change it to git
    string reconstruct_left = Scanner::remove_starting_dots_from_string(left.name);
    string reconstruct_right = Scanner::remove_starting_dots_from_string(right.name);
    return reconstruct_left < reconstruct_right;
}

void Ls::run(string command)
{
    //here there is no problem with more arguments
    //but if i receive more paths i should display them all

    scanner.scan_command(command);

    //ignore if the flag a doesnt exist
    consider_starting_with_dot = scanner.found_short_flag(accepted_flags.a);

//    here we will identify what we need, we can get any amount of arguments all will be considered as paths
//    any amount of duplicate flags will be reduced to one appeareance
//    if the help command appears anywhere, the command will not execute, instead we will have the help window open
    identify_next_step();

}

// helper function to avoid making run a long function
// scope: identify what should ls print to the screen
void Ls::identify_next_step() {
    //if help is provided -> only the help message will appear
    // --help is only a long flag
    if (scanner.found_long_flag(accepted_flags.help))
    {
        display.display_message(help);
        return;
    }

    //reaching the bootom means no path, no flag -> simple view
    simple_view();
}

void Ls::simple_view() {
    initialize_rows(terminal.getPath());

    for (int i=0; i<rows.size(); i++)
    {
        display.display_message_with_endl(rows[i].getName());
    }
}

void Ls::initialize_rows(string path) {
//    first clear the memory
    rows.clear();

    //first let's get each of the names
    DIR * dir;
    struct dirent * entry;
    dir = opendir(path.c_str());
    if (dir == NULL)
    {
        //error when opening, should propmpt the error
        errors.cannot_open_dir();
        return;
    }

    Row_info * aux = new Row_info;
    string file_name;
    while ((entry = readdir(dir)) != NULL)
    {
        file_name = entry->d_name;
        // if it start with dot but i ignore dots -> continue
        if (Scanner::is_string_starting_with_dot(file_name) and !consider_starting_with_dot)
            continue;

        //free previous memory;
        delete(aux);

        aux = new Row_info(file_name);
        rows.push_back( * aux );
    }

    closedir(dir);

    sort_rows_after_file_name();
}

void Ls::sort_rows_after_file_name() {
    sort(rows.begin(), rows.end(), Ls::Row_info::compareByName);
}

#endif //TERMINAL_LS_H
