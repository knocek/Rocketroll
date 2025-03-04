#ifndef ROCKET_H
#define ROCKET_H

#include <SFML/Graphics.hpp>
#include <string>

class Rocket {
private:
    sf::Texture texture; // rocket texture
    sf::Texture fireTexture;  // fire texture
    sf::Sprite sprite; // rocket sprite
    sf::Sprite fireSprite;   // fire sprite 
    std::string name;     // rocket name
    sf::Vector2f velocity; // rocket velocity
    float angle; // rocket angle in radians
    float thrustAngle;
        float thrustX, thrustY;
    const float angVel = 0.05f;

    float fuel;
    float maxFuel;
    float thrust;

    // stałe
    const float GRAVITY = 9.8f;
    const float ROTATION_SPEED = 0.05f;
    const float ROTATION_DAMPING = 0.995f;
    const float MAX_GIMBAL_ANGLE = 0.36f;
    const float AUTO_CENTER_SPEED = 0.05f;
    const float MASS = 1.0f;
    const float FUEL_CONSUMPTION_RATE = 10.0f;

    sf::RectangleShape fuelBar;

    // sf::Texture fireTexture;
    // sf::Sprite fireSprite;

public:
    Rocket(const std::string& texturePath, const std::string& rocketName);

    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window) const;
    void update(float deltaTime, float wind);
    void rotateLeft();
    void rotateRight();
    sf::FloatRect getBounds() const;

    std::string getName() const;

    sf::Vector2f getVelocity() const;
    float getAngle() const;  

    
    void reset(
        const sf::Vector2f& position,
        float initialAngle,
        float initialDownwardVelocity,
        float initialSidewardVelocity,
        float fuel,
        float thrust
    );

    sf::Vector2f getPosition() const;
};

#endif // ROCKET_H