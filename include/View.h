#ifndef _VIEW_H
#define _VIEW_H

#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include <string>
#include <ncurses.h>

class AbstractView {
public:
    explicit AbstractView(Manager& manager);
    virtual ~AbstractView() = default;
protected:
    Manager& manager;
};

class MenuView final : public AbstractView {
public:
    explicit MenuView(Manager& manager);
    ~MenuView() override;
    void menuInteraction();
    int getPlayersNumber();
    void addPlayerMenu();
private:
    int menuSizeX = 0;
    int menuSizeY = 0;
    static const int NUMBER_OF_BUTTONS = 3;
};

class MonopolyView : public AbstractView {
public:
    explicit MonopolyView(Manager& manager);
   // virtual void drawField();
   // virtual void printStatus();

   virtual void redraw(const Board& board) = 0;
   virtual PlayerReply processRequest(Player& p, PlayerRequest req) = 0;

   // virtual Input::InputAction getAction(); //TODO: как православно обработать экшены
};

class NcursesView final : public MonopolyView {
public:
    explicit NcursesView(Manager& manager);
    ~NcursesView() override;
    bool getActionAlpha();
    void printGrid();
    void runGame();
    void redraw(const Board& board) override;
    //void drawField() override;
    //void printStatus() override;
    PlayerReply processRequest(Player& p, PlayerRequest req) override;
    void changeFieldSize();
private:
    const static int TILES_PER_LINE = 11;
    int maxX = 0;
    int maxY = 0;
    int fieldSizeX = 0;
    int fieldSizeY = 0;
    int tileSizeX = 0;
    int tileSizeY = 0;

};
#endif