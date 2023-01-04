#include <iostream>
#include <unistd.h>

int main() {
    // The path of the symbolic link
    const char* symlink_path = "symbolic_link";

    // The buffer to store the path that the symbolic link directs to
    char buffer[1024];

    // Read the contents of the symbolic link and store the path in the buffer
    ssize_t length = readlink(symlink_path, buffer, sizeof(buffer));
    if (length < 0) {
        // An error occurred
        return 1;
    }

    // Truncate the buffer to the actual length of the path
    buffer[length] = '\0';

    std::cout << "Path of symbolic link: " << buffer << std::endl;
    return 0;
}