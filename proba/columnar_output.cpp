#include <algorithm>
#include <dirent.h>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> names;

    // Read the names of the files and directories in the current directory
    DIR *dir = opendir(".");
    if (dir == nullptr) {
        std::cerr << "Error opening directory" << std::endl;
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        names.push_back(entry->d_name);
    }
    closedir(dir);

    // Sort the names alphabetically
    std::sort(names.begin(), names.end());

    // Determine the width of the columns based on the length of the longest file name
    int width =
            std::max_element(names.begin(), names.end(),
                             [](const std::string &a, const std::string &b) {
                                 return a.size() < b.size();
                             })
                    ->size();

    // Determine the number of columns that can fit on a single row
    char *cols_str = getenv("COLUMNS");
    int terminal_width = cols_str ? atoi(cols_str) : 250;
    int num_columns = (terminal_width + 1) / (width + 1);

    // Calculate the number of rows needed
    int num_rows = (names.size() + num_columns - 1) / num_columns;

    // Print the names in a columnar format
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_columns; ++col) {
            int index = row + col * num_rows;
            if (index < names.size()) {
                std::cout << std::setw(width + 1) << std::left << names[index];
            }
        }
        std::cout << std::endl;
    }

    return 0;
}




