#include "../include/Menu.h"
#include "../include/OptionsMenu.h"
#include <iostream>

// Constructor
Menu::Menu(sf::RenderWindow& window, SoundManager& soundManager, std::string& rocketName)
    : window(window), soundManager(soundManager), selectedOption(0), selectedRocket(rocketName) {
    if (!font.loadFromFile("../assets/fonts/HibernalSentrySuperItalic-ov3Y4.otf")) {
        std::cerr << "Error loading font!" << std::endl;
        exit(1);
    }

    // Title
    title.setFont(font);
    title.setString("Rocketroll");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleBounds = title.getLocalBounds();
    sf::Vector2u windowSize = window.getSize();
    title.setPosition((windowSize.x - titleBounds.width) / 2.0f, 50);

    //background
    if (!textureMenu.loadFromFile("../assets/images/tlo_menu_gry_2.jpg")) {
        std::cerr << "Error loading background image!" << std::endl;
        exit(1);
    }

    spriteBG.setTexture(textureMenu);
    spriteBG.setPosition(0, 0);

    // Main menu options
    std::vector<std::string> options = { "Play", "Options", "Quit" };
    const float buttonWidth = 300.0f; // const button width

    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(40);

        sf::FloatRect textBounds = text.getLocalBounds();
        float textX = (1920 / 4.0f) - (buttonWidth / 2.0f); // background position
        float textY = (1080 / 2.0f) - (options.size() * 60 / 2.0f) + i * 60;
        
        // Create background rectangle for the text
        sf::RectangleShape background(sf::Vector2f(buttonWidth, textBounds.height + 20)); // use buttonWidth
        background.setFillColor(sf::Color(50, 50, 50, 200)); // semi-transparent gray
        background.setPosition(textX, textY - 10);
        // Center the text within the background
        text.setPosition(
            textX + (buttonWidth - textBounds.width) / 2.0f, 
            textY - 10 + (background.getSize().y - textBounds.height) / 2.0f - textBounds.top
        );
        
        text.setFillColor(i == selectedOption ? sf::Color::Blue : sf::Color::White);

        menuBackgrounds.push_back(background);
        menuOptions.push_back(text);
    }
}

void Menu::updateSelectedRocket(const std::string& newRocket) {
    selectedRocket = newRocket; // update selected rocket
    std::cout << "Updated Rocket in Menu: " << selectedRocket << std::endl;
}

// Draw main menu
void Menu::draw() {
    window.clear();
    window.draw(spriteBG);
    window.draw(title);
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuOptions[i]);     
    }
    window.display();
}

int Menu::getSelectedOption() const {
    return selectedOption;
}

// Handle main menu input
bool Menu::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return false;
        }

        if (event.type == sf::Event::MouseMoved) {
            // mouse position
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            for (size_t i = 0; i < menuOptions.size(); ++i) {
                if (menuOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    selectedOption = i; 
                }
            }

            for (size_t i = 0; i < menuOptions.size(); ++i) {
                menuOptions[i].setFillColor(i == selectedOption ? sf::Color::Blue : sf::Color::White);
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            // left mouse button
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // check if mouse is over any option
                for (size_t i = 0; i < menuOptions.size(); ++i) {
                    if (menuOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        selectedOption = i; // set selected option

                        // do code based on selected option
                        if (selectedOption == 0) { // Play
                            return true;
                        } else if (selectedOption == 1) { // Open Options Menu
                            OptionsMenu optionsMenu(window, soundManager, selectedRocket);
                            if (optionsMenu.handleInput()) {
                                selectedRocket = optionsMenu.getSelectedRocket();
                                updateSelectedRocket(selectedRocket);
                                selectedOption = -1;
                                return true;
                            }
                        } else if (selectedOption == 2) { // Quit
                            window.close();
                        }
                        return true;
                    }
                }
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
            }
            else if (event.key.code == sf::Keyboard::Down) {
                selectedOption = (selectedOption + 1) % menuOptions.size();
            }
            else if (event.key.code == sf::Keyboard::Return) {
                if (selectedOption == 1) { // Open Options Menu
                    OptionsMenu optionsMenu(window, soundManager, selectedRocket);
                    
                    if (optionsMenu.handleInput()) {
                        selectedRocket = optionsMenu.getSelectedRocket();
                        updateSelectedRocket(selectedRocket);
                    }
                    
                    selectedOption = -1;
                    return true;
                }
                else if (selectedOption == 2) { // Quit
                    window.close();
                }
                return true;
            }
        }

        // Update colors
        for (size_t i = 0; i < menuOptions.size(); ++i) {
            menuOptions[i].setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
        }
    }

    return false;
}