#include <ncurses.h>
#include <bits/stdc++.h>
using namespace std;
int main(void)
{
    int i = 2, height, width;
    WINDOW *nou;

    initscr();
    getmaxyx(stdscr, height, width);
    nou = newwin(height - 2, width - 2, 1, 1);

    scrollok(nou,TRUE);
    int j = 0;
    while(j < 50)
    {

        getch();
        j++;
        string message = "\nDown at \n";
        message += j;

        wprintw(nou, message.c_str() , i);
        ++i;
        wrefresh(nou);
    }

    endwin();
    return 0;
}