#include "audio/AudioManager.h"
#include <algorithm>

AudioManager::AudioManager(ResourceManager& resources)
    : m_gunshotBuffer(resources.getSoundBuffer("assets/audio/sfx/gunshot.wav"))
    , m_enemyDeathBuffer(resources.getSoundBuffer("assets/audio/sfx/enemy_death.wav"))
    , m_playerHitBuffer(resources.getSoundBuffer("assets/audio/sfx/player_hit.wav"))
    , m_gameOverBuffer(resources.getSoundBuffer("assets/audio/sfx/game_over.wav"))
    , m_music(resources.getMusic("assets/audio/music/background.ogg"))
{
    m_music.setLooping(true);
}

void AudioManager::playSound(sf::SoundBuffer& buffer)
{
    m_activeSounds.emplace_back(buffer);
    m_activeSounds.back().play();
}

void AudioManager::playGunshot()
{
    playSound(m_gunshotBuffer);
}

void AudioManager::playEnemyDeath()
{
    playSound(m_enemyDeathBuffer);
}

void AudioManager::playPlayerHit()
{
    playSound(m_playerHitBuffer);
}

void AudioManager::playGameOver()
{
    playSound(m_gameOverBuffer);
}

void AudioManager::playMusic()
{
    m_music.play();
}

void AudioManager::stopMusic()
{
    m_music.stop();
}

void AudioManager::update()
{
    m_activeSounds.erase(
        std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
            [](const sf::Sound& sound) { return sound.getStatus() == sf::Sound::Status::Stopped; }),
        m_activeSounds.end());
}