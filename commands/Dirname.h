//
// Created by area51 on 05.01.23.
//

#ifndef TERMINAL_DIRNAME_H
#define TERMINAL_DIRNAME_H
using namespace std;

namespace dirname_flags{
    const char * help = "--help";
}

Scanner dirname_scanner = *new Scanner();

class Dirname{
private:

    string help_message = "Usage: dirname [OPTION] NAME...\n"
                                  "Output each NAME with its last non-slash component and trailing slashes\n"
                                  "removed; if NAME contains no /'s, output '.' (meaning the current directory).\n"
                                  "Dirname can take any number of arguments\n"
                                  "Accepted flags are:\n"
                                  "  --help        open this help message\n"
                                  "Examples:\n"
                                  "  dirname /usr/bin/          -> \"/usr\"\n"
                                  "  dirname dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n"
                                  "  dirname stdio.h            -> \".\"";
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
}dirname_command;

void Dirname::run(string command = "")
{
    dirname_scanner = *new Scanner();
    dirname_scanner.scan_command(command);

    try{
        validate_flags();



        identify_next_step();



    } catch (const InvalidFlag & ex)
    {
        errors.invalid_flag_provided(ex.getFlag());
        errors.use_help_flag_on_command(DIRNAME);
    }
}

bool Dirname::is_unknown_flag(string flag)
{
    for (const char * accepted : accepted_flags_list)
    {
        if (strcmp(accepted, flag.c_str()) == 0)
            return false;
    }
    return true;
}

void Dirname::validate_flags() {
    for (string long_flag : dirname_scanner.get_long_flags())
    {
        if (is_unknown_flag(long_flag))
        {
            throw InvalidFlag(long_flag);
        }
    }

    for (string short_flag : dirname_scanner.get_short_flags())
    {
        if (is_unknown_flag(short_flag))
        {
            throw InvalidFlag(short_flag);
        }
    }
}

void Dirname::identify_next_step() {
    if (dirname_scanner.found_long_flag(dirname_flags::help))
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
        errors.no_arguments();
        errors.use_help_flag_on_command(DIRNAME);
    }
}

void Dirname::validate_arguments() {
    if (dirname_scanner.get_arguments().size() == 0)
    {
        throw MissingArguments();
    }
}

void Dirname::solve_for_each_argument() {
    for(string path : dirname_scanner.get_arguments())
    {
        individual_argument(path);
    }
}

void Dirname::individual_argument(string basicString) {
    //if i have a slash or none -> .
    //other then that remove the last slash
    //this can work on multiply arguments, keep the ls logic

    //remove any consecutive /
    basicString = remove_consecutive_slashes(basicString);
    basicString = remove_any_final_slash(basicString);
    display.display_debug_file(basicString);
    basicString = remove_last_word(basicString);

    display.display_message_with_endl(basicString);
}

string Dirname::remove_consecutive_slashes(string basicString) {
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

string Dirname::remove_any_final_slash(string basicString) {
    while(basicString[ basicString.length() - 1 ] == '/')
    {
        basicString.erase(basicString.length() - 1, 1);
    }
    return basicString;
}

string Dirname::remove_last_word(string basicString) {
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

#endif //TERMINAL_DIRNAME_H
