#include <ncurses.h>
#include "view.h"

NcursesView::NcursesView() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
}

NcursesView::~NcursesView() {
    endwin();
}

void NcursesView::getActionAlpha() {
    while (true) {
        int enteredChar = getch();

        switch (enteredChar) {
            case 'x':
                return;
                break;

        }
    }
}