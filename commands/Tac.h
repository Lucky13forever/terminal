//
// Created by area51 on 06.01.23.
//

#ifndef TERMINAL_TAC_H
#define TERMINAL_TAC_H

using namespace std;

namespace tac_flags{
    const char * help = "--help";
    const char * b = "-b";
    const char * before = "--before";
    const char * s = "-s";
}

vector<const char *>accepted_tac_flags = {
        tac_flags::help,
        tac_flags::b,
        tac_flags::before,
        tac_flags::s,
};

Scanner tac_scanner = *new Scanner({"-s"});

class Tac{
private:

    vector<string>lines;
    string separator = "\n";

    string help_message = "Usage: tac [OPTION]... [FILE]...\n"
                          "Write each FILE to standard output, last line first.\n"
                          "\n"
                          "With no FILE, or when FILE is -, read standard input.\n"
                          "\n"
                          "Mandatory arguments to long options are mandatory for short options too.\n"
                          "  -b, --before             attach the separator before instead of after\n"
                          "  -s, --separator=STRING   use STRING as the separator instead of newline\n";
public:
    void run(string);
    void identify_next_step();

    bool is_unknown_flag(string flag);

    void validate_flags();

    void validate_arguments();

    void solve_for_each_argument();

    void individual_argument(string basicString);

    string remove_consecutive_slashes(string basicString);

    string remove_any_final_slash(string basicString);

    string remove_last_word(string basicString);

    void show_each_line();

    void place_separator_in_front(string basicString);

    void place_separator_in_back(string basicString);

    void read_from_stdin();

    void transpose_this_to_lines(string basicString);
}tac_command;

void Tac::run(string command = "")
{
    tac_scanner = *new Scanner({"-s"});
    tac_scanner.scan_command(command);

    this->lines.clear();
    this->separator = '\n';

    try{
        validate_flags();



        identify_next_step();



    } catch (const InvalidFlag & ex)
    {
        errors.invalid_flag_provided(ex.getFlag());
        errors.use_help_flag_on_command(TAC);
    } catch (const FlagHasNoValue & ex){
        errors.flag_has_no_value(ex.getFlag());
        errors.use_help_flag_on_command(TAC);
    }
}

bool Tac::is_unknown_flag(string flag)
{
    for (const char * accepted : accepted_tac_flags)
    {
        if (strcmp(accepted, flag.c_str()) == 0)
            return false;
    }
    return true;
}

void Tac::validate_flags() {
    for (string long_flag : tac_scanner.get_long_flags())
    {
        if (is_unknown_flag(long_flag))
        {
            throw InvalidFlag(long_flag);
        }
    }

    for (string short_flag : tac_scanner.get_short_flags())
    {
        if (is_unknown_flag(short_flag))
        {
            throw InvalidFlag(short_flag);
        }
        //check if the flag has value if needed
        if (tac_scanner.does_flag_need_value(short_flag) and tac_scanner.get_value_of_flag(short_flag).empty())
        {
            //if the flag needs a value, but it's empty -> error
            throw FlagHasNoValue(short_flag);
        }
    }
}

void Tac::identify_next_step() {
    if (tac_scanner.found_long_flag(tac_flags::help))
    {
        display.display_a_new_line_for_each_endl(this->help_message);
        return;
    }

    //if no help let's check the number of arguments
    try{
        validate_arguments();

        solve_for_each_argument();

    } catch (const MissingArguments & ex)
    {
        //if no argumnents solve for a string;
//        display.display_message_with_endl("Read from stdin");
        read_from_stdin();
    }

    if (tac_scanner.found_short_flag(tac_flags::s))
    {
        separator = tac_scanner.get_value_of_flag(tac_flags::s);
    }
    if (tac_scanner.found_short_flag(tac_flags::b) or tac_scanner.found_long_flag(tac_flags::before)){
        place_separator_in_front(separator);
    }
    else {
        place_separator_in_back( separator);
    }

    show_each_line();
}

void Tac::validate_arguments() {
    if (tac_scanner.get_arguments().size() == 0)
    {
        throw MissingArguments();
    }
}

void Tac::solve_for_each_argument() {
    for(string path : tac_scanner.get_arguments())
    {
        individual_argument(path);
    }
}

void Tac::individual_argument(string file_name) {
    //if i have a slash or none -> .
    //other then that remove the last slash
    //this can work on multiply arguments, keep the ls logic

    //remove any consecutive /
    //check if file exists
    struct stat st;
    bool exists = stat(file_name.c_str(), &st) == 0;
    if (!exists)
    {
        //throw error
        errors.no_such_file(file_name);
        return;
    }

    //if im here -> file exists
    //TODO USE LATER
    vector<string> lines_to_add;

    ifstream fin(file_name.c_str());
    string line;
    while(getline(fin, line))
    {
        lines.insert(lines.begin(), line);
    }
//    reverse(lines_to_add.begin(), lines_to_add.end());

    //check if -b is present

    //append this line to the original vector
    this->lines.reserve(lines.size() + lines_to_add.size());
    this->lines.insert(lines.end(), lines_to_add.begin(), lines_to_add.end());
}

string Tac::remove_consecutive_slashes(string basicString) {
    string res;
    res += basicString[0];
    for (int i=1; i<basicString.length(); i++)
    {
        if (basicString[i] == '/' and basicString[i - 1] == '/')
        {
            continue;
        }
        res += basicString[i];
    }
    return res;
}

string Tac::remove_any_final_slash(string basicString) {
    while(basicString[ basicString.length() - 1 ] == '/')
    {
        basicString.erase(basicString.length() - 1, 1);
    }
    return basicString;
}

string Tac::remove_last_word(string basicString) {
    int pos = basicString.rfind('/');
    if (pos != std::string::npos)
    {
        //i found a slash -> remove the last word
        basicString.erase(pos, basicString.length() - pos);
        if (!basicString.empty())
        {
            return basicString;
        }
    }
    return ".";
}

void Tac::show_each_line() {
    for (string line : this->lines)
    {
        display.display_message(line);
    }
}

void Tac::place_separator_in_front(string sep) {
    for (string & line : this->lines)
    {
        line.insert(0, sep);
    }
}

void Tac::place_separator_in_back(string sep) {
    for (string & line : this->lines)
    {
        line += sep;
    }
}

void Tac::read_from_stdin() {
    int key;
    string result;
    while ((key = getch()) != CTRL_D)
    {
        if(key == BACKSPACE){

            display.backspace(false);
            if (!result.empty())
                result.erase(result.size() - 1, 1);
        }
        else{
            display.display_char(key);

            result += key;
        }
    }

    display.display_debug_file("Message from stdin: " + result);
    transpose_this_to_lines(result);
}

void Tac::transpose_this_to_lines(string basicString) {
    int start = 0;
    int end = basicString.find('\n', start);
    while(end != std::string::npos)
    {
        string substring = basicString.substr(start, end - start);

        this->lines.push_back(substring);

        start = end + 1;

        end = basicString.find('\n', start);
    }
    std::reverse(this->lines.begin(), this->lines.end());
}


#endif //TERMINAL_TAC_H
