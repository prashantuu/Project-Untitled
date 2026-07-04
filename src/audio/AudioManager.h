#pragma once

#include <SFML/Audio.hpp>
#include <vector>

class AudioManager
{
public:
    AudioManager();

    // Game calls these directly — it never touches an sf::Sound
    // or sf::SoundBuffer itself. Audio stays fully centralized here.
    void playGunshot();
    void playEnemyDeath();
    void playPlayerHit();
    void playGameOver();

    void playMusic();
    void stopMusic();

    // Cleans up finished one-shot sounds. Call once per frame.
    void update();

private:
    void playSound(sf::SoundBuffer& buffer);

private:
    sf::SoundBuffer m_gunshotBuffer;
    sf::SoundBuffer m_enemyDeathBuffer;
    sf::SoundBuffer m_playerHitBuffer;
    sf::SoundBuffer m_gameOverBuffer;

    // Multiple one-shot sounds can overlap (e.g. rapid gunfire),
    // so we keep a growing/shrinking pool rather than one sf::Sound
    // per effect.
    std::vector<sf::Sound> m_activeSounds;

    // Music is different from SFX: it's one long-running stream,
    // not something you fire-and-forget, so it gets its own object.
    sf::Music m_music;
};