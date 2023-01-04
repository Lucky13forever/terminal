#include <sys/stat.h>  // for stat
#include <unistd.h>  // for stat
#include <string>  // for std::string
#include <iostream>
using namespace std;
int main()
{
    std::string filename = "venv";

    struct stat st;
    if (stat(filename.c_str(), &st) == 0) {
    std::cout << "Number of blocks allocated: " << st.st_blocks / 2<< std::endl;
    } else {
    std::cerr << "Error getting file information" << std::endl;
    }
    return 0;
}
