#ifndef HSEPROJ_SFMLVIEW_H
#define HSEPROJ_SFMLVIEW_H
#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include "EventManager.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <pthread/pthread.h>
#include <thread>

struct ShapeHolder {

    std::array<sf::RectangleShape, Board::FIELD_SIZE> fieldRects;

};

class SFMLView {
public:
    SFMLView(Manager& manager);
    ~SFMLView();
    void mainLoop();

    void redraw(const Board& board);
    PlayerReply processRequest(Player& p, PlayerRequest req);
    void processMessage(Player& p, PlayerMessage mes);
private:
    void onResize(sf::Event::SizeEvent e);

    void drawField(const Board& board);
    void drawPlayers(const Board& board);
    void draw();

    int tmp = 0;

    Manager& manager;

    EventManager events;
    ShapeHolder shapes;
    sf::RenderWindow window;
    sf::Font mainFont;
    std::mutex requestMutex, windowMutex;
    std::condition_variable requestCond;

    //TODO: какой ужас, это же глобальные переменные
    PlayerReply requestReply;
    std::optional<PlayerRequest> curRequest;
    std::string_view curMessage;
    bool shouldClose = false;
};

#endif //HSEPROJ_SFMLVIEW_H