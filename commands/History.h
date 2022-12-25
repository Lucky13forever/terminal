#ifndef PROJECT_HISTORY_H
#define PROJECT_HISTORY_H
using namespace std;
class History{
private:
    vector<string>all_commands; // all official commands ran by the user, so each of these command were followed by an ENTER
    int index;  // holds the index of the current string  resulted by the UP and DOWN KEYS
    int enters;
public:
    History();
    void push_command(string);
    void keep_progress(string); // keep the progress in current_command, this will start when the UP or DOWN key are pressed
    void increment(); // UP key is pressed
    void decrement(); // DOWN key is pressed
    string desired_command(); // return the command after KEYS have been used
    string command_on_index(int index){ return all_commands[index];};
    void run(int (*Output_Stream) (const char *, ...)); // means running the history command
    void run(string);
}history_command;

History::History(){
    index = -1;
    enters = 0;
}

//this will take place when ENTER key is pressed
void History::push_command(string command) {
//    remove backup command
    if (enters < all_commands.size())
    {
        all_commands.pop_back();
    }

    if (strcmp(command.c_str(), "") == 0)
        return;
    all_commands.push_back(command);
    enters++;
    index = all_commands.size();
}

void History::keep_progress(string command) {

    if (enters == all_commands.size()){

//        printw("da man\n");
        all_commands.push_back(command);
    }
}

void History::increment() {
    index++;

    if (index > all_commands.size() - 1)
        index = all_commands.size() - 1;
}

void History::decrement() {
    index--;

    if (index < 0)
        index = 0;
}

string History::desired_command()
{
    if (all_commands.empty())
    {
        return "";
    }
    string result = all_commands[index];
    if (index == all_commands.size() - 1 and enters < all_commands.size())
    {
//        it means i just returned the backup, so no need to keep it anymore
        all_commands.pop_back();
    }
    return result;
}

void History::run(int (*Output_Stream) (const char *, ...)) {
    for(int i=0; i<all_commands.size(); i++)
    {
        Output_Stream("%d %s\n", i + 1, all_commands[i].c_str());
    }
}

void History::run(string command = "") {
    for(int i=0; i<all_commands.size(); i++)
    {
        printw("%d %s\n", i + 1, all_commands[i].c_str());
    }
}

#endif //PROJECT_HISTORY_H
