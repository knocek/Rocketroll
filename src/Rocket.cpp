#include "../include/Rocket.h"
#include <iostream>
#include <cmath>

// Rocket constructor
Rocket::Rocket(const std::string& texturePath, const std::string& rocketName) 
    : name(rocketName), angle(0), thrustAngle(0), thrustX(0), thrustY(0) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load rocket texture: " << texturePath << std::endl;
        exit(1);
    }
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f); // Set origin to center for rotation
    sprite.setScale(0.5f, 0.5f); // Scale down the sprite to make it smaller

    // Load fire texture
    if (!fireTexture.loadFromFile("../assets/images/fire2.png")) {
        std::cerr << "Failed to load fire texture!" << std::endl;
        exit(1);
    }
    fireSprite.setTexture(fireTexture);
    fireSprite.setOrigin(fireTexture.getSize().x / 2, 0.0f); // Set origin to the top center
    fireSprite.setScale(0.3f, 0.3f); // Adjust scale as needed

    // Initialize fuel bar
    fuelBar.setSize(sf::Vector2f(200.0f, 20.0f));
    fuelBar.setFillColor(sf::Color::Green);
    fuelBar.setPosition(10, 60);

    // Set default parameters
    fuel = maxFuel = 100.0f;
    thrust = 1500.0f;

    // Initial velocity
    velocity = sf::Vector2f(0.0f, 100.0f);
}

// Reset method
void Rocket::reset(
    const sf::Vector2f& position,
    float initialAngle,
    float initialDownwardVelocity,
    float initialSidewardVelocity,
    float fuelAmount,
    float thrustPower
) {
    setPosition(position.x, position.y);
    angle = initialAngle;
    thrustAngle = 0.0f;
    velocity = sf::Vector2f(initialSidewardVelocity, initialDownwardVelocity);
    fuel = maxFuel = fuelAmount;
    thrust = thrustPower;
}

void Rocket::update(float deltaTime, float wind) {
    // Apply gravity
    velocity.y += GRAVITY * deltaTime;
    velocity.x += wind * deltaTime;

    // Consume fuel if available
    if (fuel > 0.0f) {
        // Calculate thrust direction based on rocket's angle and gimbal adjustment
        float totalThrustAngle = angle + thrustAngle;
        thrustX = 2*thrust * std::sin(totalThrustAngle);
        thrustY = thrust * std::cos(totalThrustAngle);

        // Adjust velocity based on thrust direction
        velocity.x += (thrustX / MASS) * deltaTime;
        velocity.y -= (thrustY / MASS) * deltaTime; // Subtract to counteract gravity

        // Consume fuel
        fuel -= FUEL_CONSUMPTION_RATE * deltaTime;
        if (fuel < 0.0f) fuel = 0.0f;
    } else {
        thrustX = 0.0f;
        thrustY = 0.0f;
    }

    // Calculate torque and apply rotation
    float torque = (thrustX / MASS) * deltaTime * 0.05f; // Adjust torque influence
    angle += torque;
    angle *= ROTATION_DAMPING; // Apply damping to stabilize rotation

    // Auto-center thrust angle
    if (thrustAngle > 0) {
        thrustAngle = std::max(0.0f, thrustAngle - AUTO_CENTER_SPEED * deltaTime);
    } else if (thrustAngle < 0) {
        thrustAngle = std::min(0.0f, thrustAngle + AUTO_CENTER_SPEED * deltaTime);
    }

    // Update position
    sprite.move(velocity * deltaTime);
    sprite.setRotation(angle * 180 / 3.14159); // Convert radians to degrees

    float fireOffsetX = 82.0f;
    float fireOffsetY = 140.0f;

    sf::Vector2f localFirePosition(fireOffsetX, sprite.getLocalBounds().height / 2.0f + fireOffsetY);
    sf::Vector2f globalFirePosition = sprite.getTransform().transformPoint(localFirePosition);

    // Update fire sprite position and rotation
    fireSprite.setPosition(globalFirePosition);
    // fireSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + 50.0f);
    fireSprite.setRotation((angle + thrustAngle) * 180 / 3.14159); // Convert radians to degrees

    // Update fuel bar
    float fuelPercentage = fuel / maxFuel;
    fuelBar.setSize(sf::Vector2f(200.0f * fuelPercentage, 20.0f));
}

void Rocket::draw(sf::RenderWindow& window) const {
    window.draw(sprite);

    // Draw fire sprite if thrust is active
    if (fuel > 0.0f) {
        window.draw(fireSprite);
    }

    // Draw fuel bar
    window.draw(fuelBar);

    // Optional: Draw velocity, thrust, and angle vectors for debugging
    sf::VertexArray velocityLine(sf::Lines, 2);
    velocityLine[0].position = sprite.getPosition();
    velocityLine[0].color = sf::Color::Cyan;
    velocityLine[1].position = sprite.getPosition() + velocity * 2.0f; // Adjust scaling factor as needed
    velocityLine[1].color = sf::Color::Cyan;
    window.draw(velocityLine);

    // Draw thrust vector
    sf::VertexArray thrustLine(sf::Lines, 2);
    thrustLine[0].position = sprite.getPosition();
    thrustLine[0].color = sf::Color::Red;
    thrustLine[1].position = sprite.getPosition() - sf::Vector2f(thrustX, thrustY) * 2.0f; // Adjust scaling factor as needed
    thrustLine[1].color = sf::Color::Red;
    window.draw(thrustLine);

    // Draw angle indicator
    sf::VertexArray angleLine(sf::Lines, 2);
    angleLine[0].position = sprite.getPosition();
    angleLine[0].color = sf::Color::Green;
    angleLine[1].position = sprite.getPosition() - sf::Vector2f(std::sin(angle), -std::cos(angle)) * 50.0f; // Adjust length as needed
    angleLine[1].color = sf::Color::Green;
    window.draw(angleLine);
}

void Rocket::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Rocket::rotateLeft() {
    thrustAngle -= ROTATION_SPEED;
    angle -= angVel; 
    if (thrustAngle < -MAX_GIMBAL_ANGLE) {
        thrustAngle = -MAX_GIMBAL_ANGLE; // Limit to maximum gimbal angle
    }
}

void Rocket::rotateRight() {
    thrustAngle += ROTATION_SPEED;
    angle += angVel;
    if (thrustAngle > MAX_GIMBAL_ANGLE) {
        thrustAngle = MAX_GIMBAL_ANGLE; // Limit to maximum gimbal angle
    }
}

sf::FloatRect Rocket::getBounds() const {
    return sprite.getGlobalBounds();
}

std::string Rocket::getName() const {
    return name;
}

sf::Vector2f Rocket::getVelocity() const {
    return velocity;
}

float Rocket::getAngle() const {
    return angle;
}

sf::Vector2f Rocket::getPosition() const {
    return sprite.getPosition();
}