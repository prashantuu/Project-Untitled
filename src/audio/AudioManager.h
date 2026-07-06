#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include "resources/ResourceManager.h"

class AudioManager
{
public:
    AudioManager(ResourceManager& resources);

    void playGunshot();
    void playEnemyDeath();
    void playPlayerHit();
    void playGameOver();

    void playMusic();
    void stopMusic();

    void update();

private:
    void playSound(sf::SoundBuffer& buffer);

private:
    sf::SoundBuffer& m_gunshotBuffer;
    sf::SoundBuffer& m_enemyDeathBuffer;
    sf::SoundBuffer& m_playerHitBuffer;
    sf::SoundBuffer& m_gameOverBuffer;

    std::vector<sf::Sound> m_activeSounds;

    sf::Music& m_music;
};