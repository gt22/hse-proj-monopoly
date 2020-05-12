#ifndef SFMLVIEW_H
#define SFMLVIEW_H
#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include "EventManager.h"
#include "Board.h"
#include "BoardModel.h"
#include "ButtonManager.h"
#include "TooltipManager.h"
#include <SFML/Graphics.hpp>

//#include <pthread/pthread.h>

#include <mutex>
#include <condition_variable>
#include <pthread.h>
#include <thread>
#include <iostream>


struct ShapeHolder {

    std::array<sf::RectangleShape, Board::FIELD_SIZE> fieldRects;

};

class ButtonImage {
public:
    ButtonImage() = default;
    void setTexture(const std::string& textureName) {
        if (!buttonTexture.loadFromFile(textureName)) {
            std::cout << "Load END_TURN failed!\n";
        }
        buttonTexture.setSmooth(true);
        buttonSprite.setTexture(buttonTexture);
        buttonWidth = buttonSprite.getTexture()->getSize().x * buttonSprite.getScale().x;
        buttonHeight = buttonSprite.getTexture()->getSize().y * buttonSprite.getScale().y;
    }

    void setSpriteColor(sf::Color color) {
        buttonSprite.setColor(color);
    }

    void setPosition(sf::Vector2f point) {
        buttonSprite.setPosition(point);
    }

    void setScale(sf::Vector2f scale) {
        buttonSprite.setScale(scale);
        buttonWidth = buttonSprite.getTexture()->getSize().x * buttonSprite.getScale().x;
        buttonHeight = buttonSprite.getTexture()->getSize().y * buttonSprite.getScale().y;
    }

    bool getClickability() {
        return isClickable;
    }

    void setClickability(bool b) {
        if(b) makeClickable(); else makeUnclickable();
    }

    void toggleClickability() {
        setClickability(!getClickability());
    }

    void makeUnclickable() {
        if (!isClickable)
            return;
        isClickable = false;
        setSpriteColor(sf::Color(155, 155, 155));
    }

    void makeClickable() {
        if (isClickable)
            return;
        isClickable = true;
        setSpriteColor(sf::Color(255, 255, 255));
    }


    void drawTo(sf::RenderWindow &window) {
        window.draw(buttonSprite);
    }

    bool isMouseOver(sf::RenderWindow &window) const {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;

        int buttonPosX = buttonSprite.getPosition().x;
        int buttonPosY = buttonSprite.getPosition().y;

        int buttonxPosWidth = buttonSprite.getPosition().x + buttonWidth;
        int buttonyPosHeight = buttonSprite.getPosition().y + buttonHeight;

        return mouseX < buttonxPosWidth && mouseX > buttonPosX && mouseY < buttonyPosHeight && mouseY > buttonPosY;
    }

private:
    sf::Sprite buttonSprite;
    sf::Texture buttonTexture;
    int buttonWidth;
    int buttonHeight;
    bool isClickable = true;
};

class ButtonText {
public:
    ButtonText() = default;
    ButtonText(std::string buttonText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
        button.setSize(buttonSize);
        button.setFillColor(bgColor);

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
    explicit SFMLView(Manager& manager);
    void dispose();
    void mainLoop();

    void redraw(const Board& board);
    PlayerReply processRequest(Player& p, PlayerRequest req);
    NumReply processNum(Player &p);

    void processMessage(Player& p, PlayerMessage mes);
private:
    void onResize(sf::Event::SizeEvent e);

    void drawField(const BoardModel& board);
    void drawPlayers(const BoardModel& board);
    void drawMoney(const BoardModel& board);
    void drawMessage();
    void draw();
    void handleRequest();
    void makeReply(PlayerReply rep);
    void makeNumReply(NumReply rep); //
    BoardModel getModel();

    void addActionButton(PlayerAction action, const std::string& texture,
                         std::string tooltip, const std::function<void()>& handler);

    template<typename T>
    auto makeReplyGenerator() {
        return [this]() { makeReply(std::make_unique<T>()); };
    }

    int tmp = 0;

    Manager& manager;

    EventManager events;
    ButtonManager buttons;
    TooltipManager tooltips;

    std::unordered_map<PlayerAction, SpriteButton&> actionButtons;
    ShapeHolder shapes;
    sf::RenderWindow window;
    sf::Font mainFont;
    std::mutex boardMutex, windowMutex, requestMutex;
    std::condition_variable requestCond;
    std::condition_variable numCond; //
    BoardModel model;

    std::optional<PlayerRequest> curRequest;
    PlayerReply curReply;
    NumReply curNum; //

    // временный ужас
    sf::Text box;
    sf::Text message;

    Token curTurnBy;
    bool shouldClose = false;
};

#endif //SFMLVIEW_H