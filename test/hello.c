#include <curses.h>

int main(void) {
    initscr(); // start curses
    printw("Hello World! Press any key to exit ...");
    refresh(); // print it on to the real screen
    getch(); // wait for user input
    endwin(); // end curses

    return 0;
}