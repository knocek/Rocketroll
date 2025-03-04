#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../include/Menu.h"
#include "../include/Game.h"
#include "../include/SoundManager.h"
#include "../include/OptionsMenu.h"
#include <iostream>

int main() {
    // Create the main application window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Rocketroll");

    // Initialize the sound manager
    SoundManager soundManager;
    soundManager.playMusic();
    // soundManager.stopMusic(); // Music is turned off for now

    //Background
    sf::Texture textureMenu;
    textureMenu.loadFromFile("../assets/images/tlo_menu_gry_2.jpg");
    sf::Sprite spriteBG;

    //Scale background
    sf::Vector2u textureSize = textureMenu.getSize(); // background size
    sf::Vector2u windowSize = window.getSize();       // window size
    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
    spriteBG.setTexture(textureMenu);
    spriteBG.setScale(1.0f, 1.0f);
    spriteBG.setPosition(0, 0);

    // default rocket
    std::string selectedRocket = "rocket_grey";

    // Initialize the main menu 
    Menu menu(window, soundManager, selectedRocket);
    //OptionsMenu optionsMenu(window, soundManager, selectedRocket);

    while (window.isOpen()) {

        // Handle user input in the main menu
        if (menu.handleInput()) {
            int selected = menu.getSelectedOption();

            if (selected == 0) { // "Play" selected
                std::cout << "Play selected!" << std::endl;
                std::cout << "Rocket before Game: " << selectedRocket << std::endl;

                // Create and run the game instance
                Game game(window, selectedRocket);
                game.run();

                       // After game ends, return to the main menu
                menu.updateSelectedRocket(selectedRocket);


            }
            else if (selected == 1) { // "Options" selected
                std::cout << "Options selected!" << std::endl;
                std::cout << "Options selected! Current Rocket: " << selectedRocket << std::endl;
                OptionsMenu optionsMenu(window, soundManager, selectedRocket);
                if (optionsMenu.handleInput()) {
                    std::cout << "Returning to main menu..." << std::endl;
                    selectedRocket = optionsMenu.getSelectedRocket();
                    menu.updateSelectedRocket(selectedRocket);
                    std::cout << "Rocket after OptionsMenu in main.cpp: " << selectedRocket << std::endl;
                    //selected = -1;
                }
                
            }
            else if (selected == 2) { // "Quit" selected
                std::cout << "Quit selected!" << std::endl;
                window.close();
            }

        }

        // Draw the main menu
        window.clear();
        window.draw(spriteBG);
        menu.draw();

    }

    return 0;
}