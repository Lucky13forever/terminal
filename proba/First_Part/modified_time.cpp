#include <iostream>
#include <sys/stat.h>
#include <time.h>

std::string get_ls_date(const std::string& path) {
    struct stat file_stat;
    if (stat(path.c_str(), &file_stat) < 0) {
        // An error occurred
        return "";
    }

    // Convert the modification time to a struct tm
    struct tm* tm_time = localtime(&file_stat.st_mtime);
    if (tm_time == nullptr) {
        // An error occurred
        return "";
    }

    // Format the date and time in the same way as the ls -l command
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%b %e %H:%M", tm_time);

    return buffer;
}

int main() {
    std::string path = "/home/area51/faculta/OS/COPIE";
    std::string date = get_ls_date(path);
    std::cout << date << std::endl;
    return 0;
}
