#ifndef DRONESHIP_H
#define DRONESHIP_H

#include <SFML/Graphics.hpp>

class Droneship {
public:
    Droneship(const std::string& textureParh);

    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;     
};

#endif