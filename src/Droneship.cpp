#include "../include/Droneship.h"
#include <iostream>

Droneship::Droneship(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load droneship texture: " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    sprite.setScale(0.5f, 0.5f);
}

void Droneship::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Droneship::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Droneship::getBounds() const {
    return sprite.getGlobalBounds();
}
