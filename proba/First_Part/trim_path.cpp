#include <iostream>
#include <string>
#include <vector>

std::string trim_path(const std::string& path) {
    // Split the path into a vector of directory names
    std::vector<std::string> dirs;
    size_t start = 0;
    size_t end = path.find('/');
    while (end != std::string::npos) {
        dirs.push_back(path.substr(start, end - start));
        start = end + 1;
        end = path.find('/', start);
    }
    dirs.push_back(path.substr(start));

    // Trim the path by removing "." and ".." directory names
    std::vector<std::string> trimmed_dirs;
    for (const std::string& dir : dirs) {
        if (dir == ".") {
            // Skip "." directory names
            continue;
        } else if (dir == "..") {
            // Remove the previous directory name if ".." is encountered
            if (!trimmed_dirs.empty()) {
                trimmed_dirs.pop_back();
            }
        } else {
            // Keep other directory names
            trimmed_dirs.push_back(dir);
        }
    }

    // Rebuild the trimmed path
    std::string trimmed_path;
    for (const std::string& dir : trimmed_dirs) {
        trimmed_path += '/' + dir;
    }

    return trimmed_path;
}

int main() {
    std::string path = "/faculta/OS/terminal/../terminal/../unu/";
    std::string trimmed_path = trim_path(path);
    std::cout << "Trimmed path: " << trimmed_path << std::endl;
    return 0;
}