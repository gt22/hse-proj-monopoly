#ifndef _VIEW_H
#define _VIEW_H

#include <string>
#include <curses.h>

class MenuView {
public:
    MenuView();
    ~MenuView();
    void MenuInteraction();
    int getPlayersNumber();
    std::string addPlayer();
private:
    int menuSizeX = 0;
    int menuSizeY = 0;
    static const int NUMBER_OF_BUTTONS = 3;
    WINDOW *w;
};

class MonopolyView {
public:
   // virtual ~MonopolyView();
   // virtual void drawField();
   // virtual void printStatus();
   // virtual Input::InputAction getAction(); //TODO: как православно обработать экшены
};

class NcursesView : public MonopolyView {
public:
    NcursesView();
    ~NcursesView();
    bool getActionAlpha();
    void printGrid();
    void runGame();
    //void drawField() override;
    //void printStatus() override;
    //Input::InputAction getAction() override;
    void changeFieldSize();
private:
    const int TILES_PER_LINE = 11;
    int maxX;
    int maxY;
    int FIELD_SIZE_X;
    int FIELD_SIZE_Y;
    int TILE_SIZE_X;
    int TILE_SIZE_Y;
};
#endif