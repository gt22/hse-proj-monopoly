#ifndef HSEPROJ_SFMLVIEW_H
#define HSEPROJ_SFMLVIEW_H
#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include "EventManager.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#include <pthread/pthread.h>
#include <mutex>
#include <condition_variable>
#include <pthread.h>
#include <thread>

struct ShapeHolder {

    std::array<sf::RectangleShape, Board::FIELD_SIZE> fieldRects;

};

class Button {
public:
    Button() = default;
    Button(std::string buttonText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
        button.setSize(buttonSize);
        button.setFillColor(bgColor);

        // Get these for later use:
        buttonWidth = buttonSize.x;
        buttonHeight = buttonSize.y;

        text.setString(buttonText);
        text.setCharacterSize(charSize);
        text.setFillColor(textColor);
    }

    // Pass font by reference:
    void setFont(sf::Font &fonts) {
        text.setFont(fonts);
    }

    void setBackColor(sf::Color color) {
        button.setFillColor(color);
    }

    void setTextColor(sf::Color color) {
        text.setFillColor(color);
    }

    void setPosition(sf::Vector2f point) {
        button.setPosition(point);

        // Center text on button:
        float xPos = (point.x + buttonWidth / 2) - (text.getLocalBounds().width / 2);
        float yPos = (point.y + buttonHeight / 2.2) - (text.getLocalBounds().height / 2);
        text.setPosition(xPos, yPos);
    }

    void drawTo(sf::RenderWindow &window) {
        window.draw(button);
        window.draw(text);
    }

    // Check if the mouse is within the bounds of the button:
    bool isMouseOver(sf::RenderWindow &window) const {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;

        int buttonPosX = button.getPosition().x;
        int buttonPosY = button.getPosition().y;

        int buttonxPosWidth = button.getPosition().x + buttonWidth;
        int buttonyPosHeight = button.getPosition().y + buttonHeight;

        return mouseX < buttonxPosWidth && mouseX > buttonPosX && mouseY < buttonyPosHeight && mouseY > buttonPosY;
    }

private:
    sf::RectangleShape button;
    sf::Text text;

    int buttonWidth;
    int buttonHeight;
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

    // временный ужас
    Button btn1;

    //TODO: какой ужас, это же глобальные переменные
    PlayerReply requestReply;
    std::optional<PlayerRequest> curRequest;
    std::string_view curMessage;
    bool shouldClose = false;
};

#endif //HSEPROJ_SFMLVIEW_H