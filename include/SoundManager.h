#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Audio.hpp>

class SoundManager {
private:
    sf::Music backgroundMusic;
    bool isMuted;
    float volume;

public:
    SoundManager();
    void playMusic();
    void stopMusic();
    void toggleMute();
    void setVolume(float volume);
    bool isMusicPlaying() const;
    float getVolume() const;
};

#endif // SOUNDMANAGER_H