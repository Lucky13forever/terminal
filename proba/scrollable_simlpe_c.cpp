#include <curses.h>

int main() {
    // Initialize ncursed
    initscr();

    // Enable scrolling and line wrapping
    scrollok(stdscr, TRUE);
    wsetscrreg(stdscr, 0, LINES - 1);

    // Print some text that will cause the window to scroll
    for (int i = 0; i < 100; i++) {
        printw("This is line %d\n", i);
    }

    // Refresh the screen to make the scrolling visible
    refresh();

    // Wait for the user to press a key before exiting
    getch();

    // Shut down ncursed
    endwin();

    return 0;
}