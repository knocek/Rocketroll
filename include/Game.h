#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "../include/Rocket.h"
#include "../include/Droneship.h"
#include "../include/Level.h"
#include <string>
#include <vector>

class Game {
private:
    sf::RenderWindow& window;      // main window reference
    sf::Font font;                 // game font
    sf::Text gameText;             
    sf::Text pausedText;           // text "GAME PAUSED"
    sf::Text returnText;           // text "Return to Game"

    sf::Texture gameTexture;        // texture for background
    sf::Sprite gameSprite;          // sprite for background 

    sf::Texture pauseTexture;       // texture for pause button
    sf::Sprite pauseButton;         // sprite for pause button

    bool inOptions;                // if menu selected

    // levels
    int currentLevel;
    std::vector<Level> levels;
    sf::Text levelText;

    Rocket* rocket; // dynamic rocket function
    Droneship* droneship; // droneship member

    // landing outcome
    bool hasLanded;
    bool hasCrashed;
    sf::Text outcomeText;

    const float WATER_LEVEL = 1000.0f;
    std::vector<std::string> backgroundImages;

public:
    Game(sf::RenderWindow& window, const std::string& rocketName);  // game constructor
    ~Game();

    void run();                      // main game loop
    void draw();                     // draw game elements
    void handleInput();   
    void loadLevel(int levelNumber);
    void resetGame();           // input
    void drawPausedScreen();         //draw pause screen
    void checkLanding();
    void saveGame();
    void loadGame();

};

#endif // GAME_H