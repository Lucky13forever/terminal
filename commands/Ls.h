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


    int chars_on_single_line = CHARS_ON_SINGLE_LINE;
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

    class Col_info{
        vector<string> file_names;
        int longest_length = 0;
    public:

        void insert_file(string file);
        void add_spaces();
        int get_file_number() {return file_names.size();};
        string get_file(int index);
    };
    vector< Row_info > rows;
    vector< Col_info > columns;
    set<int> file_name_lengths;

    bool consider_starting_with_dot;

public:
    void run(string);
//we start with the simple ls command, with no flags and no path
//just display the current path

    void identify_next_step();

    void simple_view();

    void initialize_rows(string);

    void sort_rows_after_file_name();

    int get_average_length_of_file_names();

    int how_many_rows_needed_for_simple_view();

    int get_number_of_files_per_line();

    int get_final_num_of_files_per_column();

    void create_file_distribution();

    void create_file_distribution(int final_num_of_files_per_column);

    void display_file_distribution();
}ls_command;

void Ls::Col_info::insert_file(string file) {
    file_names.push_back(file);
    if (file.length() > longest_length)
        longest_length = file.length();
}

void Ls::Col_info::add_spaces() {
    for (int i=0; i<file_names.size(); i++)
    {
        for(int j=file_names[i].length();  j<longest_length; j++)
        {
            file_names[i].append(" ");
        }
        //add a final space also
        file_names[i].append(" ");
    }
}

string Ls::Col_info::get_file(int index) {
    string mt;
    if (index >= file_names.size())
        return mt;
    else
        return file_names[index];
}

Ls::Row_info::Row_info() {

}

bool Ls::Row_info::compareByName(const Ls::Row_info &left, const Ls::Row_info &right) {
    //if left is starting with ., i try to extract only the letters if it even has them
    //like for .git i will change it to git
    string reconstruct_left = Scanner::remove_starting_dots_from_string(left.name);
    string reconstruct_right = Scanner::remove_starting_dots_from_string(right.name);

    //lower the case
    reconstruct_left = Scanner::transform_to_lower_case(reconstruct_left);
    reconstruct_right = Scanner::transform_to_lower_case(reconstruct_right);
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

int Ls::get_average_length_of_file_names()
{
    int length = 0;
    for (int i=0; i<rows.size(); i++)
    {
        length += rows[i].getName().length();
    }
    return length / rows.size();
}

int Ls::how_many_rows_needed_for_simple_view()
{
    int average_string_length = get_average_length_of_file_names();

    // plus 2 cause we consider some additional spaces
    int total_chars_needed = rows.size() * ( average_string_length + 2);

    return 1;

}

int Ls::get_number_of_files_per_line(){
//    in order to replcate the simple 'ls' view, we need to calculate how many rown we will have
//    i will first calculate the average length of the file_names;

    //get the number of cols of the screen so i know the limit of the screen
    int screen_rows, screen_cols;

    getmaxyx(stdscr, screen_rows, screen_cols);

    display.display_debug("The number of chars on one line is " + std::to_string(screen_cols));

    //determine how many file_name will have each line
    //we try to push as much until we reach the end of the line
    //the colums will be separated by a space
    int total_chars_on_line_so_far = 0;
    int number_of_files_per_line = 0;
    for (int length : file_name_lengths)
    {
        total_chars_on_line_so_far += - length + 1; // + the space

        display.display_debug("So far having :" + std::to_string(total_chars_on_line_so_far));
        if (total_chars_on_line_so_far > screen_cols)
        {
            break;
        }
        number_of_files_per_line += 1;
    }

    //if i have a file that name exceeds the screen
    if (number_of_files_per_line == 0)
        number_of_files_per_line = 1;
    display.display_debug("The max number of files that will appear per line is " + std::to_string(number_of_files_per_line) );
    return number_of_files_per_line;
}

int Ls::get_final_num_of_files_per_column()
{

    int number_of_files_per_line = get_number_of_files_per_line();
    int total_num_of_files = rows.size();
    int final_num_of_files_per_column = 1;
    double choice;
    for (int i=1; ; i++)
    {
        choice = double ( double (total_num_of_files) / double (i));
        //can it be done?
        if (choice > number_of_files_per_line)
        {
            continue;
        } else
        {
            //found a solution to distribute
            final_num_of_files_per_column = i;
            break;
        }
    }
    int final_num_of_colums =  rows.size() / final_num_of_files_per_column;
    display.display_debug("The number of files per column after distributing is " + std::to_string(final_num_of_files_per_column));
    display.display_debug("The number of final colums is " + std::to_string(final_num_of_colums));

    return final_num_of_files_per_column;
}

void Ls::simple_view() {
    initialize_rows(terminal.getPath());

    //now time to create
    create_file_distribution();
    display_file_distribution();
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

        //add the length to the set
        //place with - cause i want desc order
        file_name_lengths.insert( - file_name.length() );

        aux = new Row_info(file_name);
        rows.push_back( * aux );
    }

    closedir(dir);

    sort_rows_after_file_name();
}

void Ls::sort_rows_after_file_name() {
    sort(rows.begin(), rows.end(), Ls::Row_info::compareByName);
}

void Ls::create_file_distribution() {
    //clear any previous data
    columns.clear();

    int final_num_of_files_per_column = get_final_num_of_files_per_column();

    int total_num_of_files = rows.size();

    Col_info * aux = new Col_info();
    for (int i=0; i<total_num_of_files; i+=final_num_of_files_per_column)
    {
        delete(aux);
        aux = new Col_info();
        for (int j=i; j < i + final_num_of_files_per_column and j < total_num_of_files; j++)
        {
            aux->insert_file( rows[j].getName() );
        }
        aux->add_spaces();
        columns.push_back(*aux);
    }

}

void Ls::display_file_distribution() {


    for (int i = 0; i<columns[0].get_file_number(); i++)
    {
        for (int j=0; j<columns.size(); j++)
        {
            display.display_message(columns[j].get_file(i));
        }
        display.display_message_with_endl("");
    }

}

#endif //TERMINAL_LS_H
