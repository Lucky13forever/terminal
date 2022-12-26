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
vector<const char *>accepted_flags_list = {
        accepted_flags::a,
        accepted_flags::all,
        accepted_flags::F,
        accepted_flags::classify,
        accepted_flags::help,
        accepted_flags::l,
        accepted_flags::s
};

namespace classifier{
    const char * DIRECTORY = "/";
    const char * EXECUTABLE = "*";
    const char * SYMBOLIC = "@";
    const char * FIFO = "|";
    const char * SOCKET = "=";
    const char * REGULAR = " "; //empty
    const char * NOT_FOUND = "!"; //missing file or directory
}

namespace keywords{
    const char * EXECUTABLE_KEY = "executable";
    const char * SYMBOLIC_KEY = "symbolic link";
}

namespace permission_type{
    const char * DIRECTORY = "d";
    const char * FIFO = "p"; //pipe
    const char * SYMBOLIC = "l";
    const char * SOCKET = "s";
    const char * REGULAR = "-";
}

class Ls{

    bool default_path = true;
    bool display_real_path_of_file = true;
    int total_size_in_blocks = 0;
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
    private:
        string name;
        string real_path;
        string permissions;
        string owner;
        string group;
        string date;
        File * symbolic_link_file;
        struct stat st; // this we call only if exists is set to true
        const char * permission_type;
        int theoretical_length; //this is the length that this file will consume on screen consider the extra spaces the size and the type
        int size_in_blocks;
        int size_in_bytes;
        int n_links;
        bool exists;
        int color;
        const char * type; //that thing at the end /*@=
        int sufix_extra_space_for_simple_view = 1; //this is the minim
        int prefix_extra_space_for_size_in_simple_view = 1; // minimal;
        bool is_this_null=false;


    public:
        const string &getPermissions() const;

        File *getSymbolicLinkFile() const;

        const string &getOwner() const;

        const string &getDate() const;

        int getSizeInBytes() const;

        const string &getGroup() const;

        int getNLinks() const;

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

        const string &getRealPath() const;

        void determine_type();

        const char *getType() const;

        File(string, bool);
        File(void *) {is_this_null=true;};
        int get_theoretical_length() const;
        bool is_directory();
        bool is_symbolic_link();
        string get_path_to_show_to_screen() const;

        void create_permission_string();

        const char *getPermissionType() const;

        const char *get_permission_type();

        void create_owner_string();

        void create_group_string();


        void create_symbolic_link_file();

        void create_date_string();
    };

    class Col_info{
        vector<File> files;
        int longest_theoretical_length = 0;
        int longest_file_name_length = 0;
        int longest_n_links = 0;
        int longest_owner = 0;
        int longest_group = 0;
        int longest_size_in_bytes;
        const char * type_of_the_file_with_longest_name = classifier::REGULAR;
        int longest_size_in_blocks = 0;
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

        void show_permissions(File file);

        void show_n_links(File file);

        void show_owner(File file);

        void show_group(File file);

        void show_size_in_bytes(File file);

        void show_symbolic_link_file(File file);

        void show_date(File file);
    };
    vector< string > files_in_lexicographic_order; //this will keep the files in order, but will be light weight, will only keep the name of the files in an ascending order
    vector< Col_info > columns; //this will be used for displaying
    vector< string > directories_provided;
    set<int> file_name_lengths;
    int file_number = 0;

    bool consider_starting_with_dot{};
    bool l_flag_present = false;

public:
    void run(const string&);
    //we start with the simple ls command, with no flags and no path
    //just display the current path

    void identify_next_step();

    void simple_view();

    void initialize_rows_when_the_path_is_a_directory(string path);

    void sort_rows_after_file_name();

    int how_many_rows_needed_for_simple_view();

    int get_number_of_files_per_line();

    int get_final_num_of_files_per_column();

    void create_file_distribution();

    void create_file_distribution(int final_num_of_files_per_column);

    void display_file_distribution();

    void validate_path(string arg_path);

    void initialize_rows_when_the_path_is_a_file(const string &path);

    void initialize_rows_when_the_path_is_a_file(string file_path);

    void validate_flags();

    bool is_unknown_flag(string flag);

    void initialize_rows_when_the_path_is_a_file();

    void display_total_directory_size_in_blocks();
}ls_command;

Ls::File::File(string file_path, bool symbolic_bro = false) {

    this->real_path = file_path;
    this->name = Scanner::extract_file_name_from_path(this->real_path);

    if (symbolic_bro == true)
    {
        this->name = file_path;
        this->real_path = Scanner::concatenate_two_paths(terminal.getPath(), this->name);
    }
    this->color = COLOR_WHITE_CODE; //this can change afterwads when the type of file is determined
    struct stat st;
    //gotta test the relative path of the file
    this->exists = stat(file_path.c_str(), &st) == 0;

    //must stop the code if it doesnt exist;
    if (!this->exists)
    {
        this->type = classifier::NOT_FOUND;
        throw NoSuchFileOrDirectory(this->real_path);
    }

    //if i reach this -> the file exists
    this->st = st;
    this->size_in_blocks = st.st_blocks / 2;
    this->type = classifier::REGULAR;
    this->permission_type = permission_type::REGULAR;
    this->n_links = 0;

    determine_type();

    //now for the l argument i could need more information about the file

    if (ls_command.l_flag_present)
    {
        //the permisions;
        create_permission_string();
        this->n_links = int (st.st_nlink);
        create_owner_string();
        create_group_string();
        this->size_in_bytes = st.st_size;
        create_symbolic_link_file();
        create_date_string();
    }
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
    if (size > longest_size_in_blocks)
    {
        longest_size_in_blocks = size;
    }
    int file_name_length = file.get_path_to_show_to_screen().length();
    if (file_name_length > longest_file_name_length)
    {
        longest_file_name_length = file_name_length;
        type_of_the_file_with_longest_name = file.getType();
    }
    int file_n_links_length = std::to_string(file.getNLinks()).length();
    display.display_debug_file("This is from the file " + std::to_string(file_n_links_length));
    if (file_n_links_length > longest_n_links)
    {
        longest_n_links = file_n_links_length;
    }
    display.display_debug_file("This is from the longest " + std::to_string(longest_n_links));
    int owner_length = file.getOwner().length();
    if (owner_length > longest_owner)
    {
        longest_owner = owner_length;
    }
    int group_length = file.getGroup().length();
    if (group_length > longest_group)
    {
        longest_group = group_length;
    }
    int size_bytes_length = int (to_string(file.getSizeInBytes()).length());
    if (size_bytes_length > longest_size_in_bytes)
    {
        longest_size_in_bytes = size_bytes_length;
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
        for(int i=0; i < longest_size_in_blocks - file.get_length_of_size_as_string(); i++)
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
    display.display_message_with_color(file.get_path_to_show_to_screen(), file.getColor());
}

void Ls::Col_info::show_suffix_spaces(File file)
{
    int also_the_type = 1;
    string suffix_spaces;
    for(int i=file.get_path_to_show_to_screen().length(); i<longest_file_name_length + also_the_type; i++)
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

        show_permissions(file);

        show_n_links(file);

        show_owner(file);

        show_group(file);

        show_size_in_bytes(file);

        show_date(file);

        //display_the_name_of_the_file
        show_file_name(file);

        //do i have to show the type of the file?
        show_file_type(file);

        //fill in the suffix spaces
        if (!scanner.found_short_flag(accepted_flags::l))
            show_suffix_spaces(file);

        show_symbolic_link_file(file);
    }
}

void Ls::Col_info::show_permissions(Ls::File file) {
    if (scanner.found_short_flag(accepted_flags::l))
    {
        display.display_message(file.getPermissions() + " ");
    }
}

void Ls::Col_info::show_n_links(Ls::File file) {
    if (scanner.found_short_flag(accepted_flags::l))
    {
        //longest_n_links_is the length not the value itself
        int normal_length = int ( to_string(file.getNLinks()).length() );
        display.display_front_spaces(longest_n_links, normal_length);
        display.display_message(std::to_string(file.getNLinks()) + " ");
    }
}

void Ls::Col_info::show_owner(Ls::File file) {
    if (scanner.found_short_flag(accepted_flags::l))
    {
        int normal_length = int (file.getOwner().length());
        display.display_front_spaces(longest_owner, normal_length);
        display.display_message(file.getOwner() + " ");
    }
}

void Ls::Col_info::show_group(Ls::File file) {
    if (scanner.found_short_flag(accepted_flags::l))
    {
        int normal_length = int (file.getGroup().length());
        display.display_front_spaces(longest_group, normal_length);
        display.display_message(file.getGroup() + " ");
    }
}

void Ls::Col_info::show_size_in_bytes(Ls::File file) {
    if (scanner.found_short_flag(accepted_flags::l))
    {
        int normal_length = int (to_string(file.getSizeInBytes()).length());
        display.display_front_spaces(longest_size_in_bytes, normal_length);
        display.display_message(to_string(file.getSizeInBytes()) + " ");
    }
}

void Ls::Col_info::show_symbolic_link_file(Ls::File file) {
    if (scanner.found_short_flag(accepted_flags::l))
    {
        //if this is also a sym link
        if (strcmp(file.getType(), classifier::SYMBOLIC) == 0)
        {
            display.display_message(" -> ");
            display.display_message_with_color(file.getSymbolicLinkFile()->getName(), file.getSymbolicLinkFile()->getColor());
        }
    }
}

void Ls::Col_info::show_date(Ls::File file) {
    if (scanner.found_short_flag(accepted_flags::l))
    {
        display.display_message(file.getDate() + " ");
    }
}


void Ls::run(const string& command)
{

    //clear the memory
    default_path = true;
    display_real_path_of_file = true;
    total_size_in_blocks = 0;
    l_flag_present = false;
    files_in_lexicographic_order.clear();
    file_name_lengths.clear();
    directories_provided.clear();

    display.display_debug("Running ls command");
    //here there is no problem with more arguments
    //but if i receive more paths i should display them all

    scanner.scan_command(command);

    try{
        validate_flags();

        //i could have more potential commands
        //let's check
        //if i have more then one argument -> i will get a list of all the right commands from the scanner TODO

        for(string path : scanner.get_arguments())
        {
            validate_path(path);
        }

        consider_starting_with_dot = scanner.found_short_flag(accepted_flags::a);

    //    here we will identify what we need, we can get any amount of arguments all will be considered as paths
    //    any amount of duplicate flags will be reduced to one appeareance
    //    if the help command appears anywhere, the command will not execute, instead we will have the help window open
        identify_next_step();
    } catch (const InvalidFlag & ex){
        errors.invalid_flag_provided(ex.getFlag());
        errors.use_help_flag_on_command(LS);
    }

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

    //firstly we will consider the file paths provided
    if (files_in_lexicographic_order.size())
    {
        //
        initialize_rows_when_the_path_is_a_file();
        create_file_distribution();
        display_file_distribution();
        display.display_message_with_endl("");
    }

    //now for each of the directories\
    //if this happens, then surely the file arguments have been displayed already so ->
    ls_command.display_real_path_of_file = false;
    for (int i=0; i<directories_provided.size(); i++)
    {
        //foe each of the directories the size must be reset
        total_size_in_blocks = 0;
        string path_to_show = Scanner::get_relative_path_to_terminal_path(terminal.getPath(), directories_provided[i]);
        display.display_message_with_endl(path_to_show + ":");
        initialize_rows_when_the_path_is_a_directory(directories_provided[i]);
        create_file_distribution();
        display_total_directory_size_in_blocks();
        display_file_distribution();
        display.display_message_with_endl("");
    }

    if (default_path)
    {
        //no arguments -> default path;
        //reaching the bootom means no path, no flag -> simple view
        display.display_debug("We reached sample view ");
        simple_view();
    }

}

int Ls::get_number_of_files_per_line(){
    //if the flag l is present, then beacuse i need to display everything in a list -> this function will return 1
    if (scanner.found_short_flag(accepted_flags::l))
    {
        return 1;
    }


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
    display.display_debug("Terminal path to be opened is " + terminal.getPath());
    initialize_rows_when_the_path_is_a_directory(terminal.getPath());


    //now time to create
    display.display_debug("Creating the file distribution");
    create_file_distribution();
    display_total_directory_size_in_blocks();
    display.display_debug("Displaying the file distribution");
    display_file_distribution();
}

//this will take account of the + " " + size + " " + <file_name> + "/" + "   "
//                                                                type   2 spaces
int Ls::File::get_theoretical_length() const
{
    string word = this->get_path_to_show_to_screen();
    int length = 0;

    if (scanner.found_short_flag(accepted_flags::s))
    {
        //if the scanner has the 's' flag -> add an extra space to the front, the size in blocks, and another space
        length += 1; //the first space
        string file_size = std::to_string(this->getSizeInBlocks());
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
    string file = this->getRealPath();
    struct stat st;
    stat(file.c_str(), &st);

    struct stat fstat;
    lstat(file.c_str(), &fstat);

    if (S_ISLNK(fstat.st_mode))
    {
        type = classifier::SYMBOLIC;
        permission_type = permission_type::SYMBOLIC;
        color = COLOR_CYAN_CODE;
        //this is having actually different size, only needs to keep the num of bytes of the path
        this->size_in_blocks = fstat.st_blocks;
    }
    else if (S_ISDIR(st.st_mode))
    {
        type = classifier::DIRECTORY;
        permission_type = permission_type::DIRECTORY;
        color = COLOR_BLUE_CODE;
    }
    else if (S_ISFIFO(st.st_mode))
    {
        type = classifier::FIFO;
        permission_type = permission_type::FIFO;
        color = COLOR_YELLOW_CODE;
    }
    else if (S_ISSOCK(st.st_mode))
    {
        type = classifier::SOCKET;
        permission_type = permission_type::SOCKET;
        color = COLOR_MAGENTA_CODE;
    }
    else if (st.st_mode & S_IXUSR)
    {
        type = classifier::EXECUTABLE;
        color = COLOR_GREEN_CODE;
    }
    else if (S_ISREG(st.st_mode))
    {
        type = classifier::REGULAR;
        color = COLOR_WHITE_CODE;
    }
}

void Ls::File::create_permission_string()
{
    struct stat st;
    stat(this->getRealPath().c_str(), &st);

    //asta voi face cu functie de type la fisier
    //primul append este type-ul
    string result;
    result += this->getPermissionType();
    result.append( (st.st_mode & S_IRUSR) ? "r" : "-" );
    result.append( (st.st_mode & S_IWUSR) ? "w" : "-" );
    result.append( (st.st_mode & S_IXUSR) ? "x" : "-" );
    result.append( (st.st_mode & S_IRGRP) ? "r" : "-" );
    result.append( (st.st_mode & S_IWGRP) ? "w" : "-" );
    result.append( (st.st_mode & S_IXGRP) ? "x" : "-" );
    result.append( (st.st_mode & S_IROTH) ? "r" : "-" );
    result.append( (st.st_mode & S_IWOTH) ? "w" : "-" );
    result.append( (st.st_mode & S_IXOTH) ? "x" : "-" );

    this->permissions = result;
    display.display_debug_file(this->permissions);

}

bool Ls::File::is_directory() {
    string file = this->getRealPath();
    struct stat st;
    stat(file.c_str(), &st);
    if (S_ISDIR(st.st_mode))
    {
        return true;
    }
    return false;
}

bool Ls::File::is_symbolic_link() {
    string file = this->getRealPath();
    struct stat fstat;
    lstat(file.c_str(), &fstat);
    if (S_ISLNK(fstat.st_mode))
    {
        return true;
    }
    return false;
}

const string &Ls::File::getRealPath() const {
    return real_path;
}

string Ls::File::get_path_to_show_to_screen() const {
    //i may not want to full path, but rather the relative path to the terminal path
    if (ls_command.display_real_path_of_file)
    {
        return Scanner::get_relative_path_to_terminal_path(terminal.getPath(), this->getRealPath());
    }
    return this->getName();
}

const char *Ls::File::getPermissionType() const {
    return permission_type;
}

const string &Ls::File::getPermissions() const {
    return permissions;
}

int Ls::File::getNLinks() const {
    return n_links;
}

void Ls::File::create_owner_string() {
    struct passwd * pw = getpwuid(st.st_uid);
    this->owner = pw->pw_name;
}

void Ls::File::create_group_string() {
    struct group * gr = getgrgid(st.st_gid);
    this->group = gr->gr_name;
}

const string &Ls::File::getOwner() const {
    return owner;
}

const string &Ls::File::getGroup() const {
    return group;
}

int Ls::File::getSizeInBytes() const {
    return size_in_bytes;
}

void Ls::File::create_symbolic_link_file() {

    if (!this->is_symbolic_link()){
        return;
    }

    char buffer[1024];

    ssize_t length = readlink(this->getRealPath().c_str(), buffer, sizeof(buffer));
    if (length < 0) {
        // An error occurred, so the sym link will stay empty
        return;
    }
    buffer[length] = '\0';

    //this is the path kept in the symbolic link
    string path_kept_in_symbolic_link = buffer;
    //now i need to find it's color -> so i will simply create a new file with it
    //no need to check if it exists, if this file exists -> stat returned 1 -> so the link exists
    this->symbolic_link_file = new File(path_kept_in_symbolic_link, true);

    //also update the size of the syml link
    this->size_in_bytes = path_kept_in_symbolic_link.length();

}

Ls::File *Ls::File::getSymbolicLinkFile() const {
    return symbolic_link_file;
}

void Ls::File::create_date_string() {

    struct tm* tm_time = localtime(&this->st.st_mtime);
    if (tm_time == nullptr) {
        // An error occurred
        return;
    }

    // Format the date and time in the same way as the ls -l command
    char buffer[BUFFER_SIZE];
    strftime(buffer, sizeof(buffer), "%b %e %H:%M", tm_time);

    this->date = buffer;
}

const string &Ls::File::getDate() const {
    return date;
}


//keep how many files are there, and the set in desc order of length
void Ls::initialize_rows_when_the_path_is_a_directory(string path) {


//    first clear the memory
    files_in_lexicographic_order.clear();
    file_name_lengths.clear();

    //first let's get each of the names
    DIR * dir;
    struct dirent * entry;
    display.display_debug("I want to open " + path);
    dir = opendir(path.c_str());
    if (dir == nullptr)
    {
        //error when opening, should propmpt the error
        errors.cannot_open_dir();
        return;
    }
    string file_name;
    display.display_debug("Opening the directory");
    while ((entry = readdir(dir)) != nullptr)
    {
        file_name = entry->d_name;
        // if it start with dot but i ignore dots -> continue
        if (Scanner::is_string_starting_with_dot(file_name) and !consider_starting_with_dot)
            continue;

        string real_path_without_file_name = Scanner::concatenate_two_paths(terminal.getPath(), path);
        string real_path_of_file = Scanner::concatenate_two_paths(real_path_without_file_name, file_name);

        try{

            File * new_file = new File(real_path_of_file);
            //keep the size
            this->total_size_in_blocks += new_file->getSizeInBlocks();


            files_in_lexicographic_order.push_back(new_file->getRealPath());
            file_name_lengths.insert( - new_file->get_theoretical_length() );
        } catch (NoSuchFileOrDirectory error)
        {
            //continue;
        }


    }

    closedir(dir);
    sort_rows_after_file_name();
}

void Ls::initialize_rows_when_the_path_is_a_file(){

    for(string file_ : files_in_lexicographic_order)
    {
        File * new_file = new File(file_);
        file_name_lengths.insert( - new_file->get_theoretical_length() );
    }

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

void Ls::validate_path(string arg_path) {
    //arg path could be just a name, a relative path or a full path -> we need to change it to full path
    //if i got any arg_path
    default_path = false;
    try{
        display.display_debug("This is the arg_path received : " + arg_path);


        //get the real_path
        string real_path = Scanner::concatenate_two_paths(terminal.getPath(), arg_path);

        File path_provided = *new File(real_path);

        //if it is a directory continue with the normal design
        //be carefull that a symbolic link could be a directory
        if (path_provided.is_directory()){

            display.display_debug("It seems to be a directory");
            directories_provided.push_back(path_provided.getRealPath());
        }
        //if it is a file. think of it like a new directory that only has that file, we will only modify the files_in_lexicographic_order
        else{
            display.display_debug("It seems to be just a file");
            files_in_lexicographic_order.push_back(path_provided.getRealPath());
        }

    } catch (const NoSuchFileOrDirectory & ex) {
        errors.no_such_file_or_directory(arg_path);
    }

}

bool Ls::is_unknown_flag(string flag)
{
    for (const char * accepted : accepted_flags_list)
    {
        if (strcmp(accepted, flag.c_str()) == 0)
            return false;
    }
    return true;
}

void Ls::validate_flags() {
    for (string long_flag : scanner.get_long_flags())
    {
        if (is_unknown_flag(long_flag))
        {
            throw InvalidFlag(long_flag);
        }
    }

    for (string short_flag : scanner.get_short_flags())
    {
        if (is_unknown_flag(short_flag))
        {
            throw InvalidFlag(short_flag);
        }
    }

    if (scanner.found_short_flag(accepted_flags::l))
    {
        this->l_flag_present = true;
    }
}

void Ls::display_total_directory_size_in_blocks() {
    if (scanner.found_short_flag(accepted_flags::s))
    {
        display.display_message_with_endl("total " + std::to_string(this->total_size_in_blocks));
    }
}

#endif //TERMINAL_LS_H
