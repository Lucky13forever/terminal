#include <ncurses.h>

int main() {
    // Initialize ncursed
    initscr();

    // Enable scrolling and line wrapping
    scrollok(stdscr, TRUE);
    wsetscrreg(stdscr, 0, LINES - 1);

    // Print 100 lines of text
    for (int i = 0; i < 100; i++) {
        printw("This is line %d\n", i);
    }

    // Refresh the screen to make the scrolling visible
    refresh();

    // Wait for user input
    int c;
    while ((c = getch()) != 'q') {
        // Scroll up or down based on the key press
        if (c == KEY_PPAGE) {
            wscrl(stdscr, -LINES + 1);
        } else if (c == KEY_NPAGE) {
            wscrl(stdscr, LINES - 1);
        }

        // Refresh the screen to update the scrolling
        refresh();
    }

    // Shut down ncursed
    endwin();

    return 0;
}