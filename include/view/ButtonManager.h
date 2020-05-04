#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Button : public sf::Transformable {
public:
    bool isActive() const;
    void setActive(bool b);
    void activate();
    void deactivate();

    virtual bool isValidTarget(sf::Event::MouseButtonEvent mouse) const = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
private:
    bool active;
};

class SpriteButton final : public Button {
public:
    void setTexture(const std::string& filename);
    void setSpriteColor(sf::Color color);

    bool isValidTarget(sf::Event::MouseButtonEvent mouse) const override;
    void draw(sf::RenderWindow& window) override;
private:
    sf::Sprite buttonSprite;
    sf::Texture buttonTexture;
};

class ButtonManager {
    using Handler = std::function<bool(sf::Event::MouseButtonEvent, Button&)>;
    using SimpleHandler = std::function<bool(sf::Event::MouseButtonEvent)>;
    using Modifier = std::function<void(Button&)>;
    void addButton(std::unique_ptr<Button> button, Handler handler);
    void addButton(std::unique_ptr<Button> button, const SimpleHandler& handler);
    void apply(const Modifier& m);

    bool handle(sf::Event::MouseButtonEvent e);
private:
    std::vector<std::pair<std::unique_ptr<Button>, Handler>> buttons;
};

#endif //BUTTONMANAGER_H