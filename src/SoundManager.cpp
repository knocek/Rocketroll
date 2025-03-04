#include "../include/SoundManager.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <algorithm>

SoundManager::SoundManager() : isMuted(false), volume(100.0f) {
    if (!backgroundMusic.openFromFile("../assets/sounds/background-music.ogg")) {
        std::cerr << "Error loading background music!" << std::endl;
        exit(1);
    }
    backgroundMusic.setLoop(true); // music loop
    backgroundMusic.setVolume(volume);
}

void SoundManager::playMusic() {
    if (!isMuted) {
        backgroundMusic.play();
    }
}

void SoundManager::stopMusic() {
    backgroundMusic.stop();
}

void SoundManager::toggleMute() {
    isMuted = !isMuted;
    if (isMuted) {
        backgroundMusic.pause();
    }
    else {
        backgroundMusic.play();
    }
}

void SoundManager::setVolume(float volume) {
    backgroundMusic.setVolume(volume * 100); // SFML expects a range of 0-100
}

float SoundManager::getVolume() const {
    return backgroundMusic.getVolume() / 100.0f; // Return volume in range 0-1
}

bool SoundManager::isMusicPlaying() const {
    return !isMuted && backgroundMusic.getStatus() == sf::Music::Playing;
}