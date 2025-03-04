#include "../include/Game.h"
#include "../include/Rocket.h"
#include "../include/SoundManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>


// game constructor
Game::Game(sf::RenderWindow& window, const std::string& rocketName) 
    : window(window), inOptions(false), currentLevel(1), hasLanded(false), 
hasCrashed(false) {
    std::cout << "Rocket passed to Game: " << rocketName << std::endl;
    if (!font.loadFromFile("../assets/fonts/HibernalSentrySuperItalic-ov3Y4.otf")) {
        std::cerr << "Failed to load font for game!" << std::endl;
        exit(1);
    }

    levelText.setFont(font);
    levelText.setCharacterSize(30);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(10, 70);

    backgroundImages = {
        "../assets/images/background_game_day.jpg",
        "../assets/images/background_game_night.jpg",
        "../assets/images/background_game_sunset.jpg"
    };

    //std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::srand(time(NULL));

     //background
    //  if (!gameTexture.loadFromFile("../assets/images/background_game_sunset.jpg")) {
    //     std::cerr << "Failed to background image!" << std::endl;
    // }
    // gameSprite.setTexture(gameTexture);

    //pause image
    if (!pauseTexture.loadFromFile("../assets/images/pause.png")) {
        std::cerr << "Failed to load pause button image!" << std::endl;
        exit(1);
    }
    pauseButton.setTexture(pauseTexture);
    pauseButton.setPosition(10, 10); // set position of pause button

    // inicialise levels
    levels = {
        // values of vector: levelNumber, startingPosition, initialAngle, initialDownwardVelocity, initialSidewardVelocity, fuel, thrust
        {1, sf::Vector2f(800, 50), 0.0f, 200.0f, 0.0f, 80.0f, 37.0f},
        {2, sf::Vector2f(1000, 50), 0.0f, 100.0f, 20.0f, 150.0f, 20.0f},
        {3, sf::Vector2f(400, 50), -0.7f, 150.0f, 150.0f, 60.0f, 35.0f},
        {4, sf::Vector2f(800, 50), 0.1f, 250.0f, 40.0f, 50.0f, 50.0f},
        {5, sf::Vector2f(800, 50), -0.1f, 300.0f, -40.0f, 40.0f, 35.0f},
    };

    // inicialise rocket
    rocket = new Rocket("../assets/images/" + rocketName + ".png", rocketName);
    rocket->setPosition(800, 100); // show rocket

    // initialise droneship
    droneship = new Droneship("../assets/images/starship.png");
    droneship->setPosition(800, 900);

    outcomeText.setFont(font);
    outcomeText.setCharacterSize(50);
    outcomeText.setFillColor(sf::Color::White);
    outcomeText.setPosition(500, 500);

    // game text
    gameText.setFont(font);
    gameText.setString("Kod Gry");
    gameText.setCharacterSize(50);
    gameText.setFillColor(sf::Color::White);
    gameText.setPosition(250, 250);

    // pause text
    pausedText.setFont(font);
    pausedText.setString("GAME PAUSED\nClick to return");
    pausedText.setCharacterSize(30);
    pausedText.setFillColor(sf::Color::White);
    
    // screen size
    sf::FloatRect textRect = pausedText.getLocalBounds();
    sf::Vector2u windowSize = window.getSize();

    // paused text align center
    pausedText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    pausedText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    // load game with actual level
    loadLevel(currentLevel);

}

void Game::loadLevel(int levelNumber) {
    if (!backgroundImages.empty()) {
        int backgroundIndex = std::rand() % backgroundImages.size();
        std::string selectedBackground = backgroundImages[backgroundIndex];

        // Load the selected background image
        if (!gameTexture.loadFromFile(selectedBackground)) {
            std::cerr << "Failed to load background image: " << selectedBackground << std::endl;
        } else {
            gameSprite.setTexture(gameTexture);
        }
    }

    for (const auto& level : levels) {
        if (level.levelNumber == levelNumber) {
            rocket->reset(
                level.startingPosition,
                level.initialAngle,
                level.initialDownwardVelocity,
                level.initialSidewardVelocity,
                level.fuel,
                level.thrust
            );
            hasLanded = false;
            hasCrashed = false;
            outcomeText.setString("");
            return;
        }
    }
    outcomeText.setString("Congratulations! You have completed all level.\nPress ENTER to restart");
    hasLanded = true;
    levelText.setString("Level: " + std::to_string(currentLevel));
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();        
        handleInput();
        if (inOptions) {
            drawPausedScreen();
        }
        else {
            if (!hasLanded && !hasCrashed) {
                rocket -> update(deltaTime, levels[currentLevel - 1].windStrength);
                checkLanding();
            }
            draw();
        }
    }
}

void Game::draw() {
    window.clear();
    window.draw(gameSprite);

    // window.draw(gameText);
    window.draw(pauseButton);

    window.draw(levelText);
    
    droneship->draw(window);
    rocket->draw(window);
    
    if (hasLanded || hasCrashed)
    {
        window.draw(outcomeText);
    }

    window.display();
}

// function for pause screen
void Game::drawPausedScreen() {
    window.clear(sf::Color::Black);
    window.draw(pausedText);
    window.display();
}

void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (!inOptions && !hasLanded && !hasCrashed) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    inOptions = !inOptions; // Pause
                }
                if (event.key.code == sf::Keyboard::Left) {
                    rocket->rotateLeft();
                }
                if (event.key.code == sf::Keyboard::Right) {
                    rocket->rotateRight();
                }
            }

            // Pause screen
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (pauseButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        inOptions = true;
                    }
                }
            }
        } else if (inOptions) {
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (pausedText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    pausedText.setFillColor(sf::Color::Red); // Highlight in red
                } else {
                    pausedText.setFillColor(sf::Color::White); // Default color
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (pausedText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        inOptions = false; // Return to game
                    }
                }
            }
        } else if (hasLanded || hasCrashed) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return) {
                    if (hasLanded && outcomeText.getString() != "Congratulations! You have completed all level.\nPress ENTER to restart") {
                        currentLevel++;
                        loadLevel(currentLevel);
                    } else {
                        resetGame();
                    }
                }
            }
        }
    }
}

void Game::checkLanding() {

    sf::Vector2u windowSize = window.getSize();
    float waterLevel = windowSize.y;

    if (rocket->getPosition().y >= waterLevel)  {
        hasCrashed = true;
        outcomeText.setString("You missed the droneship\nPress ENTER to restart from level 1");
        
        sf::FloatRect textRect = outcomeText.getLocalBounds();
        outcomeText.setOrigin(textRect.left + textRect.width / 2.0f,
                              textRect.top + textRect.height / 2.0f);
        outcomeText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

        return;

        }



    if (rocket->getBounds().intersects(droneship->getBounds())) {
        sf::Vector2f velocity = rocket->getVelocity();
        float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        float angle = rocket->getAngle();

        const float MaxLandingSpeed = 150.0f;
        const float MaxLandingAngle = 0.1f;

        if (speed <= MaxLandingSpeed && std::abs(angle) <= MaxLandingAngle) {
            hasLanded = true;
             if (currentLevel < levels.size()) {
                outcomeText.setString("Successful Landing!\nPress ENTER to proceed to level " + std::to_string(currentLevel + 1));
            } else {
                outcomeText.setString("Successful Landing!\nPress ENTER to finish the game");
            }
        } else {
            hasCrashed = true;
          outcomeText.setString("Crash Landing!\nPress ENTER to restart from level 1");
        }
        // Center outcome text
        sf::FloatRect textRect = outcomeText.getLocalBounds();
        outcomeText.setOrigin(textRect.left + textRect.width / 2.0f,
                              textRect.top + textRect.height / 2.0f);
        outcomeText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    }
}

void Game::resetGame() {
    currentLevel = 1;
    hasLanded = false;
    hasCrashed = false;
    loadLevel(currentLevel);
}

Game::~Game() {
    delete rocket;
    delete droneship;
}

void Game::saveGame() {
    std::ofstream saveFile("savegame.sav");
    if (saveFile.is_open()) {
        saveFile << currentLevel << "\n";
        saveFile.close();
    }
}


void Game::loadGame() {
    std::ifstream loadFile("savegame.sav");
    if (loadFile.is_open()) {
        loadFile >> currentLevel;
        loadFile.close();
        loadLevel(currentLevel);
    } else {      
        currentLevel = 1;
        loadLevel(currentLevel);
    }
    }
