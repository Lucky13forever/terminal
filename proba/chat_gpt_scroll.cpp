#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;
int main() {
    // Initialize ncursed
    initscr();

    // Enable scrolling and line wrapping
    scrollok(stdscr, TRUE);
    wsetscrreg(stdscr, 0, LINES - 1);

    // Create a list of items to display
    std::vector<std::string> items = {
            "Item 1", "Item 2", "Item 3", "Item 4", "Item 5",
            "Item 6", "Item 7", "Item 8", "Item 9", "Item 10"
    };

    // Print the items to the screen
    for (const std::string& item : items) {
        printw("%s\n", item.c_str());
    }

    // Refresh the screen to make the scrolling visible
    refresh();

    // Wait for the user to press a key before exiting
    getch();

    // Shut down ncursed
    endwin();

    return 0;
}