#ifndef _VIEW_H
#define _VIEW_H

#include <string>

namespace Input {
    enum class InputType {A, B, EMPTY};
    struct ActionA {

    };

    struct ActionB {

    };
    struct ActionEmpty {

    };
    /* ... */
    union InputData {
        ActionA a;
        ActionB b;
        ActionEmpty empty;
    };

    struct InputAction {
        InputType type;
        InputData data;
    };

}

class MenuView {
public:
    int getPlayersNumber();
    std::string addPlayer();
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
    const int FIELD_SIZE_X = 196;
    const int FIELD_SIZE_Y = 44;
    const int TILE_SIZE_X = 14;
    const int TILE_SIZE_Y = 4;

};
#endif