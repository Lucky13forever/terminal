#include <bits/stdc++.h>
#include <sys/wait.h>
using namespace std;



//we will only send the name of the command, this only checks if the command exists, not if the whole line sent by the user can be executed


bool this_command_exists(string name)
{
    string command = "command -v ";
    command.append(name);
    FILE * pipe = popen(command.c_str(), "r");

    string result = "";
    if (pipe)
    {

        //can open and read
        char * buffer = new char[128];
        if (fgets(buffer, 128, pipe) != NULL)
        {
            result += buffer;
        }

        if (!result.empty())
        {
            //this exists
            return true;
        }

    } else {
        //display error
    }

    pclose(pipe);
    return false;
}
string extract_name_of_command(string command){
    //check if i have a space in the command
    int space_found_at = command.find(" ");

    string rez;
    if (space_found_at != -1)
    {
        rez = command.substr(0, space_found_at);
    }
    else {
        //no space, then this is just the name of the command, no parameters
        rez = command;
    }

    return rez;
}
int main()
{

    bool rez = this_command_exists("cd1");
    cout << rez;
    return 0;
}