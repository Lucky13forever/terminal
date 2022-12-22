#include <sys/stat.h>
#include <iostream>
#include <string.h>
using namespace std;
bool is_this_a_valid_path(string path){
    struct stat sb;

    if (stat(path.c_str(), &sb) == 0)
    {
        return true;
    }
    return false;
}

int main() {

    cout << is_this_a_valid_path("/home/area51/faculta/OS/terminal/one");

    return 0;
}