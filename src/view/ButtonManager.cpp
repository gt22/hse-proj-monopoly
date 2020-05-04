#include "view/ButtonManager.h"

bool Button::isActive() const {
    return active;
}

void Button::setActive(bool b) {
    active = b;
}

void Button::activate() {
    setActive(true);
}

void Button::deactivate() {
    setActive(false);
}


void SpriteButton::setTexture(const std::string& filename) {
    if (!buttonTexture.loadFromFile(filename)) {
        throw std::runtime_error("Unable to load texture"); //TODO: throw more specific type
    }
    buttonTexture.setSmooth(true);
    buttonSprite.setTexture(buttonTexture);
}

void SpriteButton::setSpriteColor(sf::Color color) {
    buttonSprite.setColor(color);
}

void SpriteButton::draw(sf::RenderWindow& window) {
    buttonSprite.setOrigin(getOrigin());
    buttonSprite.setPosition(getPosition());
    buttonSprite.setRotation(getRotation());
    buttonSprite.setScale(getScale());
    setSpriteColor(isActive() ? sf::Color::White : sf::Color(155, 155, 155));
    window.draw(buttonSprite);
}

bool SpriteButton::isValidTarget(sf::Event::MouseButtonEvent mouse) const {
    auto[bx, by] = buttonSprite.getPosition();
    auto[bx_, by_, bw, bh] = buttonSprite.getLocalBounds();
    return bx <= mouse.x && mouse.x <= bx + bw &&
           by <= mouse.y  && mouse.y <= by + bh;
}


void ButtonManager::addButton(std::unique_ptr<Button> button, ButtonManager::Handler handler) {
    buttons.emplace_back(std::move(button), std::move(handler));
}

void ButtonManager::addButton(std::unique_ptr<Button> button, const ButtonManager::SimpleHandler& handler) {
    addButton(std::move(button), [handler](sf::Event::MouseButtonEvent e, Button& _){ return handler(e); });
}

void ButtonManager::apply(const ButtonManager::Modifier& m) {
    for(auto& [btn, handler] : buttons) {
        m(*btn);
    }
}

bool ButtonManager::handle(sf::Event::MouseButtonEvent e) {
    for(auto& [btn, handler] : buttons) {
        if(btn->isValidTarget(e) && handler(e, *btn)) {
            return true;
        }
    }
    return false;
}


