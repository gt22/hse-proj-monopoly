#include <ncurses.h>
#include <string>
#include "Player.h"
#include "View.h"
#include "Manager.h"
#include <memory>

AbstractView::AbstractView(Manager& manager) : manager(manager) {}

MonopolyView::MonopolyView(Manager &manager) : AbstractView(manager) {}

NcursesView::NcursesView(Manager& manager) : MonopolyView(manager) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    curs_set(0);
}

NcursesView::~NcursesView() {
    endwin();
}

// PLAYER
void NcursesView::getCoord(int &x, int &y, int index) {
    if (index >= 40)
        index %= 40;
    if (index < 0)
        index +=40;
    if (index < TILES_PER_LINE) {
        x = tileSizeX / 2 + (index) * tileSizeX;
        y = tileSizeY / 2;
    } else if (index < 2 * TILES_PER_LINE - 1) {
        x = fieldSizeX - tileSizeX / 2;
        y = tileSizeY / 2 + (index - TILES_PER_LINE + 1) * tileSizeY;
    } else if (index < 3 * TILES_PER_LINE - 2) {
        x = fieldSizeX - tileSizeX / 2 - (index - 2 * TILES_PER_LINE + 2) * tileSizeX;
        y = fieldSizeY - tileSizeY / 2;
    } else {
        x = tileSizeX / 2;
        y = fieldSizeY - tileSizeY / 2 - (index - 3 * TILES_PER_LINE + 3) * tileSizeY;
    }
}

void NcursesView::drawPlayer(int x, int y, int color) {
    if(has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(0, COLOR_RED, COLOR_BLACK);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(color));
    mvaddch(y, x, ' '|A_REVERSE);
    attroff(COLOR_PAIR(color));
}

// FIELD

void NcursesView::changeFieldSize() {
    while (maxX % TILES_PER_LINE != 0 && maxX > 0) {
        maxX--;
    }
    while (maxY % TILES_PER_LINE != 0 && maxY > 0) {
        maxY--;
    }
    fieldSizeX = maxX;
    fieldSizeY = maxY;
    tileSizeY = maxY / TILES_PER_LINE;
    tileSizeX = maxX / TILES_PER_LINE;
}

void NcursesView::printGrid() {
    getmaxyx(stdscr, maxY, maxX);
    maxX--;
    maxY--;
    changeFieldSize();

    /* right and left lines*/
    for (int line = 0; line <= fieldSizeX - tileSizeX; line +=(fieldSizeX - tileSizeX)) {
        for (int x = 0; x <= tileSizeX; x += ( tileSizeX))
            for (int y = 0; y <= fieldSizeY; y++) {
                move(y, line + x);
                addch(ACS_VLINE);
            }
    }

    /*bottom and top lines*/
    for (int line = 0; line <= fieldSizeY - tileSizeY; line += (fieldSizeY - tileSizeY)) {
        for (int y = 0; y <= tileSizeY; y+=tileSizeY)
            for (int x = 1; x < fieldSizeX; x++) {
                move(line + y, x);
                addch(ACS_HLINE);
            }
    }
    
    /*cells of right and left */ 
    for (int line = 0; line <= fieldSizeX - tileSizeX; line +=(fieldSizeX - tileSizeX)) {
        for (int y = 0; y <= (fieldSizeY); y+=tileSizeY)
            for (int x = 1; x < tileSizeX; x++) {
                move(y, line + x);
                addch(ACS_HLINE);
            }
    }

     /*cells of bottom and top lines*/
    for (int line = 0; line <= fieldSizeY - tileSizeY; line += (fieldSizeY - tileSizeY)) {
        for (int x = 0; x <= fieldSizeX; x+=tileSizeX)
                for (int y = 1; y < tileSizeY; y++) {
                    move(y + line, x);
                    addch(ACS_VLINE);
                }
    }

    /* corners */
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, fieldSizeX, ACS_URCORNER);
    mvaddch(fieldSizeY, 0, ACS_LLCORNER);
    mvaddch(fieldSizeY, fieldSizeX, ACS_LRCORNER);

    for (int x = 0; x <= fieldSizeX - tileSizeX; x+=(fieldSizeX - tileSizeX))
        for (int y = tileSizeY; y < fieldSizeY; y+=tileSizeY)
            mvaddch(y, x, ACS_LTEE);

    for (int x = tileSizeX; x <= fieldSizeX; x+=(fieldSizeX - tileSizeX))
        for (int y = tileSizeY; y < fieldSizeY; y+= tileSizeY)
            mvaddch(y, x, ACS_RTEE);

    for (int y = 0; y<= fieldSizeY - tileSizeY; y+=(fieldSizeY - tileSizeY))
        for (int x = tileSizeX; x < fieldSizeX; x+=tileSizeX)
            mvaddch(y, x, ACS_TTEE);

    for (int y = tileSizeY; y<= fieldSizeY; y+=(fieldSizeY - tileSizeY))
        for (int x = tileSizeX; x < fieldSizeX; x+=tileSizeX)
            mvaddch(y, x, ACS_BTEE);

    /* crosses */
    for (int y = tileSizeY; y <= fieldSizeY - tileSizeY; y += fieldSizeY - 2 * tileSizeY)
        for (int x = tileSizeX; x <= fieldSizeX - tileSizeX; x += fieldSizeX - 2 * tileSizeX) {
            move(y, x);
            addch(ACS_PLUS);
        }

    refresh();
}


void NcursesView::runGame() {
    //int ind = 0;
    while (true) {
        bool flag = false;
        //printGrid();
        redraw();
        //int x = 0, y = 0;
        int enteredChar = getch();

        switch(enteredChar) {
            case 'x':
                flag = true;
                break;
            case KEY_RIGHT:
                for (int i = 0; i < 6; i++)
                    ar[i]++;
                break;
            case KEY_LEFT:
                for (int i = 0; i < 6; i++)
                    ar[i]--;
                break;
         /*   case 'd':
                erase();
                getCoord(x, y, ind);
                drawPlayer(x, y, COLOR_CYAN);
                break;
            case KEY_RIGHT:
                erase();
                refresh();
                ind++;
                refresh();
                getCoord(x, y, ind);
                drawPlayer(x, y, COLOR_CYAN);
                refresh();
                break;*/

        }
        refresh();
        if (flag)
            break;
    }
}

PlayerReply NcursesView::processRequest(Player& player, PlayerRequest req) {
    return std::make_unique<PlayerReplyData>(PlayerAction::END_TURN);
}

void NcursesView::processMessage(Player &p, PlayerMessage mes) {

}

void NcursesView::redraw() {
    // TODO: const Board& board - лажа с константностью
    clear();
    refresh();
    printGrid();
    // TODO: размер вектора (кол-во игроков?) board.getPlayers()


    size_t size = 6;

    for (size_t i = 0; i < size; i++) {
        int x, y;
        //getCoord(x, y, board.getPlayer(board.getPlayerToken(i)).position);
        getCoord(x, y, ar[i]);
        if (i == 0) {
            x-=2;
            y--;
        } else if (i == 1) {
            y--;
        } else if (i == 2) {
            x+=2;
            y--;
        } else if (i == 3) {
            x-=2;
            y++;
        } else if (i == 4) {
            y++;
        } else if (i == 5) {
            x+=2;
            y++;
        }
        drawPlayer(x, y, i);
    }

}

int NcursesView::transformCoord(int x, int y) {
    if (x > tileSizeX && y > tileSizeY && x < fieldSizeX - tileSizeX && y < fieldSizeY - tileSizeY)
        return -1;
    if (y < tileSizeY)
        return x / tileSizeX;
    else if (x > fieldSizeX - tileSizeX)
        return TILES_PER_LINE - 1 + y / tileSizeY;
    else if (y > fieldSizeY - tileSizeY)
        return 3 * TILES_PER_LINE -  3 - x / tileSizeX;
    else if (x < tileSizeX)
        return 4 * TILES_PER_LINE - 4 - y / tileSizeY;
    return -1;
}

int NcursesView::mouseAction() {
    MEVENT event;
    int ch, pos;
    while (true) {
        ch = getch();
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
                    pos = transformCoord(event.x, event.y);
                    if (pos != -1)
                        break;
                } else if (event.bstate & BUTTON1_PRESSED) {
                    // tile info
                }
            }
        }
    }
    return pos;
}
