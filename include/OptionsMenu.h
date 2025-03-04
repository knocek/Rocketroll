#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../include/SoundManager.h"

class OptionsMenu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    SoundManager& soundManager;
    std::vector<sf::Text> menuOptions;
    int selectedOption;

    std::string& selectedRocket; // selected rocket
    void setupMenu(); // main options setup
    void drawSetRocket(); // function for select rocket
    void drawMusicMenager(); // function for mute/unmute music

    std::vector<std::string> rockets; // rockets list
    std::vector<sf::Texture> rocketTextures; // rockets textures
    std::vector<sf::Sprite> rocketSprites;   // rockets sprites

    void initializeRockets(); // new function for initialize rockets
    bool adjustingVolume;
    bool musicMenuActive;

    sf::Texture backgroundTexture; 
    sf::Sprite backgroundSprite;
    sf::RectangleShape darkOverlay; // Ensure this is declared
    std::vector<sf::RectangleShape> menuBackgrounds;

public:
    OptionsMenu(sf::RenderWindow& window, SoundManager& soundManager, std::string& selectedRocket);
    void draw();
    bool handleInput();
    std::string getSelectedRocket() const;
};

#endif // OPTIONSMENU_H