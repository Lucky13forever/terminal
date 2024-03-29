//
// Created by area51 on 22.12.22.
//

#ifndef TERMINAL_SCANNER_H
#define TERMINAL_SCANNER_H

#include "Constants.h"

using namespace std;

class Raw_command{
    string command;
    string action;
public:
    const string &getCommand() const {
        return command;
    }

    const string &getAction() const {
        return action;
    }

    void setAction(const string &action) {
        Raw_command::action = action;
    }

    void setCommand(const string &command) {
        Raw_command::command = command;
    }

    Raw_command(const string &command, const string &action) : command(command), action(action) {}
    Raw_command() {}
};

class Scanner{
    map<string, vector<string> > scan;
    map<string, string> value_of_flag;
    vector<string> flags_that_need_value;
    string last_pushed;
    const char * COMMAND = "command";
    const char * ARGUMENTS = "arguments";
    const char * SHORT_FLAGS = "short_flags";
    const char * LONG_FLAGS = "long_flags";


    bool has_key(map<string, vector<string>> map, const string& key);
    bool is_short_flag(string word);
    bool is_long_flag(string word);
    bool is_argument(string word);
    void add_argument(const string&);
    void find_place(string& word);
    void add_short_flag(string word);
    void add_long_flag(const string& word);
    bool missing(const vector<string>&,const string&);
public:
    Scanner() {};
    explicit Scanner(vector<string>);//list of flags that have to take a value!
    void scan_command(const string& command);
    string get_command() {return scan[COMMAND][0];};
    vector<string> get_arguments() {return scan[ARGUMENTS];};
    vector<string> get_short_flags() {return scan[SHORT_FLAGS];};
    vector<string> get_long_flags() {return scan[LONG_FLAGS];};
    vector<string> get_everything();
    bool found_long_flag(string flag);
    bool is_string_starting_with_dot(string word) {return word[0] == '.';};

    //for sorting
    static string remove_starting_dots_from_string(string word);
    static string transform_to_lower_case(string word);
    static bool compare_strings(const string &, const string &);
    vector<Raw_command> split_command(string);//used when the command has | or > and >>
    static string trim(string);

    string concatenate_path_with_file_name(string, string);
    string extract_file_name_from_path(string);

    bool found_short_flag(string flag);

    string concatenate_two_paths(string current_path, string path);

    string get_relative_path_to_terminal_path(const string &terminal_path, const string &path_file);

    vector<string> concatenate_all_arguments();

    bool added_value_to_flag(const string &basicString);
    string get_value_of_flag(string flag) {return value_of_flag[flag];};
    bool does_flag_need_value(string flag) {
        for (const string& yes_i_need : flags_that_need_value)
        {
            if (flag == yes_i_need)
                return true;
        }
        return false;
    };

    void validate_action(string basicString);
};

Scanner::Scanner(vector<string>flags) {
    for (string flag : flags)
    {
        value_of_flag[flag] = *new string;
        flags_that_need_value.push_back(flag);
    }

}

bool Scanner::has_key(map<string, vector<string>> map, const string& key){

    return map.find(key) != map.end();
}

bool Scanner::is_short_flag(string word)
{
    return word[0] == '-' and ( word.size() == 1 or word[1] != '-' );
}

bool Scanner::is_long_flag(string word)
{
    return word.size() >=2 and word[0] == '-' and word[1] == '-';
}

bool Scanner::is_argument(string word)
{
    return word[0] != '-';
}

void Scanner::add_argument(const string& word){
    scan[ARGUMENTS].push_back(word);
}

void Scanner::add_short_flag(string word) {
    string flag;
    for(int i=1; i<word.size(); i++)
    {
        flag = word[i];
        if (missing(scan[SHORT_FLAGS], flag))
            scan[SHORT_FLAGS].push_back("-" + flag);//add the - in front
    }
}

void Scanner::add_long_flag(const string& word) {
    string flag = word.substr(2, word.size() - 1);
    if (missing(scan[LONG_FLAGS], flag))
        scan[LONG_FLAGS].push_back( "--" + flag );
}
void Scanner::find_place(string& word)
{

    //if word has quotes remove them
    if (word[0] == '\'' or word[0] == '"')
    {
        //if the quotes are not identical -> error
        if (word[0] != word[ word.size() - 1 ])
        {
            throw QuotesDontMatch();
        }
        //if they are the same -> i can remove them
        word.erase(0, 1);
        word.erase(word.size() - 1, 1);
    }
    if (!has_key(scan, COMMAND))
    {
//        if i have no command yet, then this is the first call
        scan[COMMAND].push_back(word);
    }
    else {
        //it can be a value of a flag
        if (!added_value_to_flag(word))
        {
            // 2 options, it's either an argument, or a flag
            if (is_argument(word)) {
                add_argument(word);
            }
            if (is_short_flag(word)) {
                add_short_flag(word);
            }
            if (is_long_flag(word))
            {
                add_long_flag(word);
            }
        }
    }
    this->last_pushed = word;
}


void Scanner::scan_command(const string& command)
{
    scan.clear();
    string word;
    string trim_command = command;
    //remove forward spaces
    while(trim_command[0] == ' ')
        trim_command.erase(0, 1);
    //remove back spaces
    while(trim_command[ trim_command.size() - 1 ] == ' ')
        trim_command.erase( trim_command.size() - 1, 1);

    //replace any \n with ' '
    int pos = trim_command.find('\n');
    while (pos != std::string::npos)
    {
        trim_command.replace(pos, 1, " ");
        pos = trim_command.find('\n');
    }

    bool in_quoted_string = false;
    for (char i : trim_command)
    {
        if (i == '"' or i == '\'')
        {
            //if im in a string
            if (in_quoted_string == false)
                in_quoted_string = true;
            else
                in_quoted_string = false;
        }
        //if im in quote i take space as a content of the quoted string
        if (i == ' ' and in_quoted_string == false)
        {
            if (!word.empty()) {
                find_place(word);
                word.erase();
            }
        } else {
            word += i;
        }
    }
    //if at the end, i'm still in a quote error
    if (in_quoted_string == true)
    {
        throw UnterminatedQuotedString();
    }

    if (!word.empty())
        find_place(word);
}

bool Scanner::missing(const vector<string>& v, const string& word) {
    for (const auto & i : v)
    {
        if (i == word)
        {
            return false;
        }
    }
    return true;
}

bool Scanner::found_long_flag(string flag) {
    for (string long_flag : this->get_long_flags())
    {
        if (long_flag == flag)
            return true;
    }
    return false;
}

bool Scanner::found_short_flag(string flag){
    for (string short_flag : this->get_short_flags())
    {
        if (short_flag == flag)
            return true;
    }
    return false;
}

string Scanner::remove_starting_dots_from_string(string word) {
    int i;
    for (i=0; i<word.size(); i++)
    {
        if (word[i] != '.')
        {
            break;
        }
    }
    if (i == word.size())
    {
        //no letter in this word
        //return original
        return word;
    }
    //if i'm here, remove the first dots
    return word.substr(i, word.size() - 1);
}

string Scanner::transform_to_lower_case(string word) {
    for (char & c : word)
    {
        c = std::tolower(c);
    }
    return word;

}

bool Scanner::compare_strings(const string & left, const string & right) {
    //if left is starting with ., i try to extract only the letters if it even has them
    //like for .git i will change it to git
    string reconstruct_left = Scanner::remove_starting_dots_from_string(left);
    string reconstruct_right = Scanner::remove_starting_dots_from_string(right);

    //lower the case
    reconstruct_left = Scanner::transform_to_lower_case(reconstruct_left);
    reconstruct_right = Scanner::transform_to_lower_case(reconstruct_right);
    return reconstruct_left < reconstruct_right;
}

string Scanner::concatenate_two_paths(string current_path, string path) {
    //if the path starts with / dont do anything
    if (path[0] == '/')
        return path;

    //else concatenate
    return current_path + "/" + path;
}

string Scanner::extract_file_name_from_path(string path) {
    //get the string after the last slash
    int index = path.rfind("/");


    if (index != -1)
    {
        return path.substr(index + 1, path.size() - 1);
    }
    return path;
}

string Scanner::get_relative_path_to_terminal_path(const string & terminal_path, const string & path_file) {

    //so i have the real_path in path_file
    //i need to separate them and remove the / in between
    //path_file is definitely having something extra, like the name of a dict or a file

    //they overlap, and the path_file starts with the terminal path so
    int terminal_path_length = terminal_path.length();
    string relative_path = path_file.substr(terminal_path_length + 1, path_file.length() - 1); // in the left, +1 to eliminate the /
    return relative_path;
}

vector<string> Scanner::get_everything()
{
    vector<string>unu = {this->get_command()};
    vector<string>doi = this->get_arguments();
    vector<string>trei = this->get_short_flags();
    vector<string>patru = this->get_long_flags();

    vector<string> result;

    result.reserve(unu.size() + doi.size() + trei.size() + patru.size());
    result.insert(result.end(), unu.begin(), unu.end());
    result.insert(result.end(), doi.begin(), doi.end());
    result.insert(result.end(), trei.begin(), trei.end());
    result.insert(result.end(), patru.begin(), patru.end());

    return result;
}

vector<Raw_command> Scanner::split_command(string command)
{
    //TODO add exception when user uses to many pipes or to much of >>
    vector<Raw_command> result;
    string word;
    string action = NORMAL_RUN;
    string follow_up_action;
    for(char c : command)
    {
        if (c == '|' or c == '>')
        {
            if (!word.empty())
            {
                result.push_back(*new Raw_command(word, action));
                follow_up_action.clear();
            }
            follow_up_action += c;
            validate_action(follow_up_action);
            word.erase();
        }
        else {
            //word is valid if it is not just a space so
            if (!(word.empty() and c == ' '))
            {
                //if word is empty and this c is just space i can ignore it => use negation
                word += c;
            }
            else {
                //if i end up hear, the user could type >> >, so i need to warn him
                follow_up_action += c;
            }
        }

        //the signs are > and |
        //for signs i cannot have more then 2 signs toghether
        //for the | cannot have more then 1 signs together
        //after creating the sign >> > or |, the only accepted chars are anything else

        if (c == '|')
        {
            action = DO_PIPE;
        }
        if (c == '>')
        {
            if (action == DO_REDIRECTION)
            {
                action = DO_APPEND;
            }
            else {
                action = DO_REDIRECTION;
            }
        }

    }
    if (!word.empty())
        result.push_back(*new Raw_command(word, action));
    return result;
}

string Scanner::trim(string start) {
    string result = start;
    while(result[0] == ' ')
    {
        result.erase(0, 1);
    }
    while(result[ result.length() - 1 ] == ' ')
    {
        result.erase( result.length() - 1, 1 );
    }
    return result;
}

bool Scanner::added_value_to_flag(const string & value) {
    //when creating the scanner, the constructor will now what flags have to take values
    //let's identify if the last pushed is a flag that needs a value

    for (string flag : flags_that_need_value)
    {
        if (this->last_pushed == flag)
        {
            //this flag was inserted last, so this string is his value
            value_of_flag[last_pushed] = value;
            return true;
        }
    }
    return false;
}

void Scanner::validate_action(string basicString) {
    if (! (basicString == "|" or basicString == ">" or basicString == ">>" ) )
    {
        display.display_debug_file("I want to throw an error cause : " + basicString);
        throw SyntaxError(basicString);
    }
}


#endif //TERMINAL_SCANNER_H
