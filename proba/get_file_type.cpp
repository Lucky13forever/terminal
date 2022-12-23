#include <sys/stat.h>  // for stat
#include <unistd.h>  // for stat
#include <string>  // for std::string
#include <iostream>
using namespace std;

bool check_file_type(string file, string type)
{
    string command = "file ";
    command += file + " | grep " + type;

    FILE * f = popen(command.c_str(), "r");

    string result;
    char * buffer = new char[1024];
    while(std::fgets(buffer, 1024, f) != NULL)
    {
        result += buffer;
    }

    if (!result.empty())
    {
        return 1;
    }
    return 0;
}

bool is_executable(string file)
{
    return check_file_type(file, "executable");
}
int main()
{

    string path = "nou.txt";

    cout << is_executable(path);
    return 0;
}
