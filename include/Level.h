#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/System/Vector2.hpp>

struct Level
{
    int levelNumber;
    sf::Vector2f startingPosition;
    float initialAngle;
    float initialDownwardVelocity;
    float initialSidewardVelocity;
    float fuel;
    float thrust;
    float windStrength;
};

#endif
