//
// Created by area51 on 22.12.22.
//

#ifndef TERMINAL_LS_H
#define TERMINAL_LS_H
using namespace std;

namespace accepted_flags{
    const char * a = "a";
    const char * all = "all";
    const char * F = "F";
    const char * classify = "classify";
    const char * help = "help";
    const char * l = "l";
    const char * s = "s";
}


namespace classifier{
    const char * DIRECTORY = "/";
    const char * EXECUTABLE = "*";
    const char * SYMBOLIC = "@";
    const char * FIFO = "|";
    const char * SOCKET = "=";
    const char * REGULAR = " "; //empty
}

namespace keywords{
    const char * EXECUTABLE_KEY = "executable";
    const char * SYMBOLIC_KEY = "symbolic link";
}

class Ls{

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

    class File{
        string name;
        int theoretical_length; //this is the length that this file will consume on screen consider the extra spaces the size and the type
        int size_in_blocks;
        int color;
        const char * type; //that thing at the end /*@=
        int sufix_extra_space_for_simple_view = 1; //this is the minim
        int prefix_extra_space_for_size_in_simple_view = 1; // minimal;
        bool is_this_null=false;

    public:

        const string &getName() const {
            return name;
        }

        int getSizeInBlocks() const {
            return size_in_blocks;
        }

        int getColor() const {
            return color;
        }

        bool notNull() {
            return !is_this_null;
        }
        int get_length_of_size_as_string() const{
            return std::to_string(size_in_blocks).length();
        }

        void determine_type();

        const char *getType() const;

        File(string);
        File(void *) {is_this_null=true;};
        int get_theoretical_length() const;

    };

    class Col_info{
        vector<File> files;
        int longest_theoretical_length = 0;
        int longest_file_name_length = 0;
        const char * type_of_the_file_with_longest_name = classifier::REGULAR;
        int longest_size = 0;
    public:

        int get_file_number() {return files.size();};
        File get_file(int index);

        void insert_file(File file);

        bool is_there_a_file_with_this_index(int index);
        void show_file_info_on_screen(int index);

        void show_size_information(File file);

        void show_file_type(File file);

        void show_file_name(File file);

        void show_suffix_spaces(File file);
    };
    vector< string > files_in_lexicographic_order; //this will keep the files in order, but will be light weight, will only keep the name of the files in an ascending order
    vector< Col_info > columns; //this will be used for displaying
    set<int> file_name_lengths;
    int file_number = 0;

    bool consider_starting_with_dot{};

public:
    void run(const string&);
    //we start with the simple ls command, with no flags and no path
    //just display the current path

    void identify_next_step();

    void simple_view();

    void initialize_rows(const string&);

    void sort_rows_after_file_name();

    int how_many_rows_needed_for_simple_view();

    int get_number_of_files_per_line();

    int get_final_num_of_files_per_column();

    void create_file_distribution();

    void create_file_distribution(int final_num_of_files_per_column);

    void display_file_distribution();
}ls_command;

Ls::File::File(string file_name) {
    this->name = file_name;
    struct stat st;
    stat(file_name.c_str(), &st);
    this->size_in_blocks = st.st_blocks / 2;

    this->type = classifier::REGULAR;
    determine_type();
}
void Ls::Col_info::insert_file(File file) {

    this->files.push_back(file);

    //find out the max theoretical length for this col
    int length = file.get_theoretical_length();
    if (length > longest_theoretical_length)
    {
        longest_theoretical_length = length;
    }
    int size = std::to_string(file.getSizeInBlocks()).length();
    if (size > longest_size)
    {
        longest_size = size;
    }
    int file_name_length = file.getName().length();
    if (file_name_length > longest_file_name_length)
    {
        longest_file_name_length = file_name_length;
        type_of_the_file_with_longest_name = file.getType();
    }
}

bool Ls::Col_info::is_there_a_file_with_this_index(int index)
{
    return index < files.size();
}

Ls::File Ls::Col_info::get_file(int index) {
   if (is_there_a_file_with_this_index(index))
   {
       return files[index];
   }
   return *new File(nullptr);
}

void Ls::Col_info::show_size_information(File file)
{
    //do i have to show the size? 's'
    string front_spaces;
    if (scanner.found_short_flag(accepted_flags::s))
    {
        //the first space
        front_spaces = " ";
        //add any extra space to line up the names of the files
        for(int i=0; i<longest_size - file.get_length_of_size_as_string(); i++)
        {
            front_spaces += " ";
        }
        //display this extra spaces
        display.display_message(front_spaces);

        //also i need to display the actual size
        display.display_message(std::to_string(file.getSizeInBlocks()));
        //then an extra space between the file size and the file name
        display.display_message(" ");
    }
}

void Ls::Col_info::show_file_type(File file)
{
    //do i have to show the type of the file? 'F'
    if (scanner.found_short_flag(accepted_flags::F))
    {
        //this is only one char
        //for now i will display ? because the type is not yet configured
        //TODO
        display.display_message(file.getType());
    }
}

void Ls::Col_info::show_file_name(File file)
{
    //display_the_name_of_the_file
    //TODO color the output
    display.display_message(file.getName());
}

void Ls::Col_info::show_suffix_spaces(File file)
{
    int also_the_type = 1;
    string suffix_spaces;
    for(int i=file.getName().length(); i<longest_file_name_length + also_the_type; i++)
    {
        suffix_spaces += " ";
    }
    //extra space for visibilty
    suffix_spaces += "  ";
    display.display_message(suffix_spaces);
}

void Ls::Col_info::show_file_info_on_screen(int index) {

    display.display_debug("Showing the file information");
    if (is_there_a_file_with_this_index(index))
    {
        File file = files[index];
        //do i have to show the size? 's'
        show_size_information(file);

        //display_the_name_of_the_file
        show_file_name(file);

        //do i have to show the type of the file?
        show_file_type(file);

        //fill in the suffix spaces
        show_suffix_spaces(file);
    }
}

void Ls::run(const string& command)
{

    display.display_debug("Running ls command");
    //here there is no problem with more arguments
    //but if i receive more paths i should display them all

    scanner.scan_command(command);

    //ignore if the flag a doesnt exist
    consider_starting_with_dot = scanner.found_short_flag(accepted_flags::a);

//    here we will identify what we need, we can get any amount of arguments all will be considered as paths
//    any amount of duplicate flags will be reduced to one appeareance
//    if the help command appears anywhere, the command will not execute, instead we will have the help window open
    identify_next_step();

}

// helper function to avoid making run a long function
// scope: identify what should ls print to the screen
void Ls::identify_next_step() {
    display.display_debug("Identifying next step!");
    //if help is provided -> only the help message will appear
    // --help is only a long flag
    if (scanner.found_long_flag(accepted_flags::help))
    {
        display.display_message(help);
        return;
    }

    //reaching the bootom means no path, no flag -> simple view
    simple_view();
}

int Ls::get_number_of_files_per_line(){
//    in order to replcate the simple 'ls' view, we need to calculate how many rown we will have
//    i will first calculate the average length of the files;

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
    int total_num_of_files = int ( files_in_lexicographic_order.size() );
    int final_num_of_files_per_column = 1;
    double choice;
    for (int i=1; i<=total_num_of_files ; i++)
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
    display.display_debug("The number of files per column after distributing is " + std::to_string(final_num_of_files_per_column));

    return final_num_of_files_per_column;
}

//this means ls with maybe flags, but no arguments
void Ls::simple_view() {
    initialize_rows(terminal.getPath());


    //now time to create
    display.display_debug("Creating the file distribution");
    create_file_distribution();
    display.display_debug("Displaying the file distribution");
    display_file_distribution();
}

//this will take account of the + " " + size + " " + <file_name> + "/" + "   "
//                                                                type   2 spaces
int Ls::File::get_theoretical_length() const
{
    string word = this->getName();
    int length = 0;

    if (scanner.found_short_flag(accepted_flags::s))
    {
        //if the scanner has the 's' flag -> add an extra space to the front, the size in blocks, and another space
        length += 1; //the first space
        File * new_file = new File(word);

        string file_size = std::to_string(new_file->getSizeInBlocks());
        length += int ( file_size.length() ); //counting the size

        length += 1; //again an extra space;
    }
    //now i count the space of the file itlsef
    length += int ( word.length() );
    if (scanner.found_short_flag(accepted_flags::F))
    {
        //this will only display a char so
        length += 1;
    }

    //2 final spaces at the end
    length += 2;
    //this means that they will be stick to each other at a gap of 2 spaces
    length += 1; //sometimes, when i display the size, the gap will be 3 spaces
    return length;
}

const char *Ls::File::getType() const {
    return type;
}

void Ls::File::determine_type() {
    string file = this->getName();
    struct stat st;
    stat(file.c_str(), &st);

    struct stat fstat;
    lstat(file.c_str(), &fstat);

    if (S_ISLNK(fstat.st_mode))
    {
        type = classifier::SYMBOLIC;
    }
    else if (S_ISDIR(st.st_mode))
    {
        type = classifier::DIRECTORY;
    }
    else if (S_ISFIFO(st.st_mode))
    {
        type = classifier::FIFO;
    }
    else if (S_ISSOCK(st.st_mode))
    {
        type = classifier::SOCKET;
    }
    else if (st.st_mode & S_IXUSR)
    {
        type = classifier::EXECUTABLE;
    }
    else if (S_ISREG(st.st_mode))
    {
        type = classifier::REGULAR;
    }
}


//keep how many files are there, and the set in desc order of length
void Ls::initialize_rows(const string& path) {
//    first clear the memory
    files_in_lexicographic_order.clear();
    file_name_lengths.clear();

    //first let's get each of the names
    DIR * dir;
    struct dirent * entry;
    dir = opendir(path.c_str());
    if (dir == nullptr)
    {
        //error when opening, should propmpt the error
        errors.cannot_open_dir();
        return;
    }
    string file_name;
    while ((entry = readdir(dir)) != nullptr)
    {
        file_name = entry->d_name;
        // if it start with dot but i ignore dots -> continue
        if (Scanner::is_string_starting_with_dot(file_name) and !consider_starting_with_dot)
            continue;

        files_in_lexicographic_order.push_back(file_name);
        //add the length to the set
        //place with - cause i want desc order
        File * new_file = new File(file_name);
        file_name_lengths.insert( - new_file->get_theoretical_length() );

    }

    closedir(dir);
    sort_rows_after_file_name();
}

void Ls::sort_rows_after_file_name() {
    sort(files_in_lexicographic_order.begin(), files_in_lexicographic_order.end(), Scanner::compare_strings);
}

void Ls::create_file_distribution() {
    //clear any previous data
    columns.clear();

    int final_num_of_files_per_column = get_final_num_of_files_per_column();

    int total_num_of_files = int( files_in_lexicographic_order.size() );

    Col_info * aux = new Col_info();
    for (int i=0; i<total_num_of_files; i+=final_num_of_files_per_column)
    {
        delete(aux);
        aux = new Col_info();
        for (int j=i; j < i + final_num_of_files_per_column and j < total_num_of_files; j++)
        {
            File new_file = *new File(files_in_lexicographic_order[j]);
            aux->insert_file(new_file );
        }
        columns.push_back(*aux);
    }

}

void Ls::display_file_distribution() {

    display.display_debug("Yep trying to show you the files");

    for (int i = 0; i<columns[0].get_file_number(); i++)
    {
        for (int j=0; j<columns.size(); j++)
        {
            columns[j].show_file_info_on_screen(i);
        }
        display.display_message_with_endl("");
    }

}

#endif //TERMINAL_LS_H
