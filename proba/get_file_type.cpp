#include <sys/stat.h>  // for stat
#include <unistd.h>  // for stat
#include <string>  // for std::string
#include <iostream>
using namespace std;

bool check_file_type(string file, string type)
{
    string command = "file ";
    command += file + " | grep " + "'" + type + "'";

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

bool is_symbolic_link(string file)
{
    return check_file_type(file, "symbolic link");
}
int main()
{
    string file = "terminal";
    struct stat st;
    stat(file.c_str(), &st);

    struct stat fstat;
    lstat(file.c_str(), &fstat);

    if (S_ISLNK(fstat.st_mode)) {
        printf("%s is a symbolic link\n", file.c_str()); // good
    } else if (S_ISDIR(st.st_mode)) {
        printf("%s is a directory\n", file.c_str()); // good
    } else if (st.st_mode & S_IXUSR) {
        printf("%s is an executable file\n", file.c_str()); // good
    } else if (S_ISFIFO(st.st_mode)) {
        printf("%s is a FIFO\n", file.c_str()); // good
    } else if (S_ISSOCK(st.st_mode)) {
        printf("%s is a socket\n", file.c_str());
    } else if (S_ISREG(st.st_mode)) {
        printf("%s is a regular file\n", file.c_str());
    } else {
        printf("%s is an unknown type of file\n ", file.c_str());
    }

    return 0;
}
