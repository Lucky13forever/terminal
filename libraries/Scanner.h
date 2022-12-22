//
// Created by area51 on 22.12.22.
//

#ifndef TERMINAL_SCANNER_H
#define TERMINAL_SCANNER_H
using namespace std;
class Scanner{
    map<string, vector<string> > scan;
    const char * COMMAND = "command";
    const char * ARGUMENTS = "arguments";
    const char * SHORT_FLAGS = "short_flags";
    const char * LONG_FLAGS = "long_flags";


    static bool has_key(map<string, vector<string>> map, const string& key);
    static bool is_short_flag(string word);
    static bool is_long_flag(string word);
    static bool is_argument(string word);
    void add_argument(const string&);
    void find_place(const string& word);
    void add_short_flag(string word);
    void add_long_flag(const string& word);
    static bool missing(const vector<string>&,const string&);
public:
    void scan_command(const string& command);
    string get_command() {return scan[COMMAND][0];};
    vector<string> get_arguments() {return scan[ARGUMENTS];};
    vector<string> get_short_flags() {return scan[SHORT_FLAGS];};
    vector<string> get_long_flags() {return scan[LONG_FLAGS];};
    void show_command();
    void show_arguments();
    void show_short_flags();
    void show_long_flags();
    void show_all();
}scanner;

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
            scan[SHORT_FLAGS].push_back(flag);
    }
}

void Scanner::add_long_flag(const string& word) {
    string flag = word.substr(2, word.size() - 1);
    if (missing(scan[LONG_FLAGS], flag))
        scan[LONG_FLAGS].push_back( flag );
}
void Scanner::find_place(const string& word)
{
    if (!has_key(scan, COMMAND))
    {
//        if i have no command yet, then this is the first call
        scan[COMMAND].push_back(word);
    }
    else {
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


void Scanner::scan_command(const string& command)
{
    scan.clear();
    string word;
    for (char i : command)
    {
        if (i == ' ')
        {
            if (!word.empty()) {
                find_place(word);
                word.erase();
            }
        } else {
            word += i;
        }
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

void Scanner::show_command() {
    cout << "\nCommand\n";
    cout << scanner.get_command();
    cout << "\n";
}

void Scanner::show_arguments() {

    cout << "\nArguments\n";
    for (const string& arg : scanner.get_arguments())
    {
        cout << arg << "\n";
    }
    cout << "\n";
}

void Scanner::show_short_flags() {

    cout << "\nShort flags\n";
    for (const string& arg : scanner.get_short_flags())
    {
        cout << arg << "\n";
    }
}

void Scanner::show_long_flags() {

    cout << "\nLong Flags\n";
    for (const string& arg : scanner.get_long_flags())
    {
        cout << arg << "\n";
    }
}

void Scanner::show_all() {
    scanner.show_command();
    scanner.show_arguments();
    scanner.show_short_flags();
    scanner.show_long_flags();
}

#endif //TERMINAL_SCANNER_H
