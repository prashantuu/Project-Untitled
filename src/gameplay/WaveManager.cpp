#include "gameplay/WaveManager.h"
#include <algorithm>

WaveManager::WaveManager(EnemyManager& enemyManager)
    : m_enemyManager(enemyManager)
    , m_currentWave(0)
    , m_enemiesRemainingToSpawn(0)
    , m_spawnTimer(sf::Time::Zero)
    , m_spawnInterval(sf::Time::Zero)
{
    startWave(1);
}

void WaveManager::startWave(int waveNumber)
{
    m_currentWave = waveNumber;

    // Increasing difficulty: more enemies each wave...
    const int BASE_ENEMY_COUNT = 3;
    const int ENEMIES_PER_WAVE_INCREASE = 2;
    m_enemiesRemainingToSpawn = BASE_ENEMY_COUNT + (waveNumber - 1) * ENEMIES_PER_WAVE_INCREASE;

    // ...spawned faster each wave, down to a floor so it never
    // becomes an unplayable firehose.
    const float BASE_INTERVAL_SECONDS = 2.f;
    const float INTERVAL_DECREASE_PER_WAVE = 0.15f;
    const float MIN_INTERVAL_SECONDS = 0.5f;

    float intervalSeconds = std::max(
        MIN_INTERVAL_SECONDS,
        BASE_INTERVAL_SECONDS - (waveNumber - 1) * INTERVAL_DECREASE_PER_WAVE);

    m_spawnInterval = sf::seconds(intervalSeconds);
    m_spawnTimer = sf::Time::Zero;
}

void WaveManager::update(sf::Time deltaTime)
{
    if (m_enemiesRemainingToSpawn > 0)
    {
        m_spawnTimer += deltaTime;

        if (m_spawnTimer >= m_spawnInterval)
        {
            m_spawnTimer = sf::Time::Zero;

            m_enemyManager.spawnRandomEnemy();
            m_enemiesRemainingToSpawn--;
        }
    }
    else if (isWaveComplete())
    {
        // All enemies for this wave have been spawned AND defeated —
        // move on to the next, harder wave.
        startWave(m_currentWave + 1);
    }
}

bool WaveManager::isWaveComplete() const
{
    return m_enemiesRemainingToSpawn == 0 && m_enemyManager.getEnemyCount() == 0;
}

int WaveManager::getCurrentWave() const
{
    return m_currentWave;
}