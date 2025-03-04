#include "../include/OptionsMenu.h"
#include <iostream>
#include <cmath>

// Constructor
OptionsMenu::OptionsMenu(sf::RenderWindow& window, SoundManager& soundManager, std::string& selectedRocket)
    : window(window), soundManager(soundManager), selectedOption(0), selectedRocket(selectedRocket), adjustingVolume(false) {
    if (!font.loadFromFile("../assets/fonts/HibernalSentrySuperItalic-ov3Y4.otf")) {
        std::cerr << "Error loading font!" << std::endl;
        exit(1);
    }

    // Load and set background texture
    if (!backgroundTexture.loadFromFile("../assets/images/tlo_menu_gry_2.jpg")) {
        std::cerr << "Error loading background image!" << std::endl;
        exit(1);
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Create a darker semi-transparent overlay
    darkOverlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    darkOverlay.setFillColor(sf::Color(50, 50, 50, 200)); // Darker overlay
    darkOverlay.setPosition(0, 0); // Ensure it covers the whole window

    setupMenu();
    initializeRockets();
}

// Draw the menu
void OptionsMenu::draw() {
    window.clear();

    // Draw the background and dark overlay
    window.draw(backgroundSprite);
    window.draw(darkOverlay);

    for (size_t i = 0; i < menuOptions.size(); ++i) {
        window.draw(menuBackgrounds[i]);
        window.draw(menuOptions[i]);
    }
    window.display();
}

// initialize rockets functions
void OptionsMenu::initializeRockets() {
    rocketTextures.clear();
    rocketSprites.clear();

    rockets = { "rocket_grey", "rocket_red", "rocket_blue" };

    for (const auto& rocket : rockets) {
        sf::Texture texture;
        if (!texture.loadFromFile("../assets/images/" + rocket + ".png")) {
            std::cerr << "Failed to load rocket texture: " << rocket << std::endl;
            exit(1);
        }
        texture.setSmooth(true);
        rocketTextures.push_back(std::move(texture)); // move texture to vector

        sf::Sprite sprite;
        sprite.setTexture(rocketTextures.back());

        // Set scale for the sprite
        sf::Vector2u textureSize = rocketTextures.back().getSize();
        sf::Vector2u windowSize = window.getSize();
        float maxImageHeight = windowSize.y * 0.5f;
        float scale = maxImageHeight / static_cast<float>(textureSize.y);
        sprite.setScale(scale, scale);

        rocketSprites.push_back(sprite);
    }

    // set sprites positions
    sf::Vector2u windowSize = window.getSize();
    float totalWidth = rocketSprites.size() * (rocketSprites[0].getGlobalBounds().width + 50.0f);
    float startX = (windowSize.x - totalWidth) / 2.0f;
    float centerY = (windowSize.y - rocketSprites[0].getGlobalBounds().height) / 2.0f;

    for (size_t i = 0; i < rocketSprites.size(); ++i) {
        rocketSprites[i].setPosition(startX + i * (rocketSprites[0].getGlobalBounds().width + 50.0f), centerY);
    }
}

// Setup menu options
void OptionsMenu::setupMenu() {
    menuOptions.clear();
    std::vector<std::string> options = { "Set Rocket", "Music", "Back to menu" };
    const float buttonWidth = 320.0f; 

    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(40);

        sf::FloatRect textBounds = text.getLocalBounds();
        
        // align text center
        sf::Vector2u windowSize = window.getSize();
        float textX = (windowSize.x / 2.0f) - (textBounds.width / 2.0f);
        float textY = (windowSize.y / 2.0f) - (options.size() * 60 / 2.0f) + i * 60;
        text.setPosition(textX, textY);

        // Create background rectangle for the text
        sf::RectangleShape background(sf::Vector2f(buttonWidth, textBounds.height + 20)); // use buttonWidth
        background.setFillColor(sf::Color(50, 50, 50, 200)); // semi-transparent gray
        background.setPosition((windowSize.x / 2.0f) - (buttonWidth / 2.0f), textY - 10);
        
        // Center the text within the background
        text.setPosition(
            background.getPosition().x + (background.getSize().x - textBounds.width) / 2.0f,
            background.getPosition().y + (background.getSize().y - textBounds.height) / 2.0f - textBounds.top
        );

        text.setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
        menuBackgrounds.push_back(background);
        menuOptions.push_back(text);
    }
}

// Handle input for the menu
bool OptionsMenu::handleInput() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }

            // Handle window resize event
            if (event.type == sf::Event::Resized) {
                // Update the size of the dark overlay to match the new window size
                darkOverlay.setSize(sf::Vector2f(event.size.width, event.size.height));

                // Update the view to match the new window size
                sf::View view = window.getDefaultView();
                view.setSize(event.size.width, event.size.height);
                window.setView(view);
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (size_t i = 0; i < menuOptions.size(); ++i) {
                    if (menuOptions[i].getGlobalBounds().contains(mousePos)) {
                        selectedOption = i;
                        for (size_t j = 0; j < menuOptions.size(); ++j) {
                            menuOptions[j].setFillColor(j == selectedOption ? sf::Color::Red : sf::Color::White);
                        }
                    }
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
                            if (selectedOption == 0) { // Set Rocket
                                std::cout << "Set Rocket!" << std::endl;
                                drawSetRocket();
                                //selectedOption = 2;
                                //return true;
                            }
                            else if (selectedOption == 1) { // Toggle Music
                                //soundManager.toggleMute();
                                // Display mute/unmute text
                                drawMusicMenager();
        
                            }
                            else if (selectedOption == 2) { // Back
                                std::cout << "Option 2-Returning to main menu..." << std::endl;
                                return true;
                            }
                            // return true; // with return it will close the window and return to back menu
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
                    if (selectedOption == 0) { // Set Rocket
                        std::cout << "Set Rocket!" << std::endl;
                        drawSetRocket();
                        //selectedOption = 2;
                        //return true;
                    }
                    else if (selectedOption == 1) { // Toggle Music
                        drawMusicMenager();
                        //soundManager.toggleMute();
                        //return true;

                    }
                    else if (selectedOption == 2) { // Back
                        std::cout << "Option 2-Returning to main menu..." << std::endl;
                        return true;
                    }
                }

                // Update option colors
                for (size_t i = 0; i < menuOptions.size(); ++i) {
                    menuOptions[i].setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
                }
            }
        }

        draw(); // Redraw menu
    }
    return false;
}

void OptionsMenu::drawMusicMenager() {

    window.clear(); // Clear window before drawing options

    while(window.isOpen()){

        window.clear();
        window.draw(backgroundSprite);
        window.draw(darkOverlay);
        // Music options: Music on/off and Adjust volume
        std::vector<std::string> musicOptions = {
            soundManager.isMusicPlaying() ? "Turn Music Off" : "Turn Music On", // Toggle music
            "Adjust Volume (Left/Right key)", // Adjust volume option
            "Back to options" // Exit to options
        };

        std::vector<sf::Text> musicMenuTexts;
        const float buttonWidth = 750.0f; 
        for (size_t i = 0; i < musicOptions.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(musicOptions[i]);
            text.setCharacterSize(40);

            sf::FloatRect textBounds = text.getLocalBounds();
            sf::Vector2u windowSize = window.getSize();
            float textX = (windowSize.x / 2.0f) - (textBounds.width / 2.0f);
            float textY = (windowSize.y / 2.0f) - (musicOptions.size() * 60 / 2.0f) + i * 60;
            text.setPosition(textX, textY);

            // Create background rectangle for the text
            sf::RectangleShape background(sf::Vector2f(buttonWidth, textBounds.height + 20)); // use buttonWidth
            background.setFillColor(sf::Color(50, 50, 50, 200)); // semi-transparent gray
            background.setPosition((windowSize.x / 2.0f) - (buttonWidth / 2.0f), textY - 10);
            
            // Center the text within the background
            text.setPosition(
                background.getPosition().x + (background.getSize().x - textBounds.width) / 2.0f,
                background.getPosition().y + (background.getSize().y - textBounds.height) / 2.0f - textBounds.top
            );

            text.setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
            menuBackgrounds.push_back(background);
            musicMenuTexts.push_back(text);
            window.draw(background);
            window.draw(text);
        }


        // If the second option (volume) is selected, draw the volume % text
        if (selectedOption == 1) {
            sf::Text volumeText;
            volumeText.setFont(font);
            volumeText.setString("Volume: " + std::to_string(static_cast<int>(round(soundManager.getVolume() * 100))) + "%");
            volumeText.setCharacterSize(40);
            sf::FloatRect textBounds = volumeText.getLocalBounds();
            sf::Vector2u windowSize = window.getSize();
            float textX = (windowSize.x / 2.0f) - (textBounds.width / 2.0f);
            float textY = (windowSize.y / 2.0f) - (musicOptions.size() * 60 / 2.0f) + 300;
            volumeText.setPosition(textX, textY);
            window.draw(volumeText);
        }

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (size_t i = 0; i < musicMenuTexts.size(); ++i) {
                    if (musicMenuTexts[i].getGlobalBounds().contains(mousePos)) {
                        selectedOption = i;
                        for (size_t j = 0; j < musicMenuTexts.size(); ++j) {
                            musicMenuTexts[j].setFillColor(j == selectedOption ? sf::Color::Red : sf::Color::White);
                        }
                    }
                }
            }
            

            if (event.type == sf::Event::MouseButtonPressed) { // Mouse click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (size_t i = 0; i < musicMenuTexts.size(); ++i) {
                        if (musicMenuTexts[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            selectedOption = i;
                            if (selectedOption == 0) { // Toggle music
                                soundManager.toggleMute();
                            }
                            else if (selectedOption == 1) { // Adjust volume
                                adjustingVolume = true;
                            }
                            else if (selectedOption == 2) { // Exit
                                return;
                            }
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedOption = (selectedOption - 1 + musicOptions.size()) % musicOptions.size();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedOption = (selectedOption + 1) % musicOptions.size();
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (selectedOption == 0) { // Toggle music
                        soundManager.toggleMute();
                    }
                    else if (selectedOption == 1) { // Adjust volume
                        adjustingVolume = true;
                        }
                    else if (selectedOption == 2) { // Exit
                        return;
                    }
                }
                
                if (event.key.code == sf::Keyboard::Left  && selectedOption == 1) {
                    // Decrease volume
                    float volume = soundManager.getVolume();
                    volume = std::max(0.0f, volume - 0.1f); // Decrease volume with min bound at 0
                    soundManager.setVolume(volume);          
                    std::cout << "Volume: " << volume * 100 << std::endl;          
                }
                else if (event.key.code == sf::Keyboard::Right &&  selectedOption == 1) {
                    // Increase volume
                    float volume = soundManager.getVolume();
                    volume = std::min(1.0f, volume + 0.1f); // Increase volume with max bound at 100
                    soundManager.setVolume(volume);
                    std::cout << "Volume: " << volume * 100 << std::endl;
                }

                // Update option colors
                for (size_t i = 0; i < musicMenuTexts.size(); ++i) {
                    musicMenuTexts[i].setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
                }
            }

  
    }

}
}

// change rocket function
void OptionsMenu::drawSetRocket() {
    window.setView(window.getDefaultView());
    
    int selectedRocketOption = -1;
    initializeRockets();

    while (window.isOpen()) {
   
        window.clear();

        window.draw(backgroundSprite);
        window.draw(darkOverlay);

        for (size_t i = 0; i < rocketSprites.size(); ++i) {
            // background and frame for selected rocket
            if (i == selectedRocketOption) {
                sf::RectangleShape frame;
                frame.setSize(sf::Vector2f(
                    rocketSprites[i].getGlobalBounds().width + 10.0f,
                    rocketSprites[i].getGlobalBounds().height + 10.0f
                ));
                frame.setOutlineThickness(5.0f);
                frame.setOutlineColor(sf::Color::Yellow);
                frame.setFillColor(sf::Color(50, 50, 50)); // gray background color for better visibility
                frame.setPosition(rocketSprites[i].getPosition().x - 5.0f, rocketSprites[i].getPosition().y - 5.0f);
                window.draw(frame);
            }

            window.draw(rocketSprites[i]);
        }

        window.display();


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseMoved) {
                // Update the selected rocket option based on mouse hover
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < rocketSprites.size(); ++i) {
                    if (rocketSprites[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        selectedRocketOption = i;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (size_t i = 0; i < rocketSprites.size(); ++i) {
                        if (rocketSprites[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            selectedRocketOption = i;
                            selectedRocket = rockets[i];
                            std::cout << "Selected Rocket: " << selectedRocket << std::endl;
                            return;
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    return; // Go back to options menu
                } else if (event.key.code == sf::Keyboard::Left) {
                    selectedRocketOption = (selectedRocketOption - 1 + rockets.size()) % rockets.size();
                } else if (event.key.code == sf::Keyboard::Right) {
                    selectedRocketOption = (selectedRocketOption + 1) % rockets.size();
                } else if (event.key.code == sf::Keyboard::Return) {
                    if (selectedRocketOption >=0 && selectedRocketOption < rockets.size()) {
                        selectedRocket = rockets[selectedRocketOption];
                        std::cout << "Selected Rocket: " << selectedRocket << std::endl;
                        return;
                    }
                }
            }
        }
    }
}

std::string OptionsMenu::getSelectedRocket() const {
    std::cout << "Returning Rocket from OptionsMenu: " << selectedRocket << std::endl;
    return selectedRocket; // return actual selected rocket
}