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

    // Every 5th wave is a boss wave: no regular enemies, just one
    // tough boss instead.
    bool isBossWave = (waveNumber % 5 == 0);

    if (isBossWave)
    {
        m_enemiesRemainingToSpawn = 0;
        m_enemyManager.spawnBoss({400.f, 300.f}); // fixed arena-center-ish spot
        m_spawnTimer = sf::Time::Zero;
        return;
    }

    const int BASE_ENEMY_COUNT = 3;
    const int ENEMIES_PER_WAVE_INCREASE = 2;
    m_enemiesRemainingToSpawn = BASE_ENEMY_COUNT + (waveNumber - 1) * ENEMIES_PER_WAVE_INCREASE;

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
    if (m_enemyManager.hasBoss())
    {
        // Boss waves don't use the regular spawn timer at all —
        // just wait for the boss to die.
        if (isWaveComplete())
        {
            startWave(m_currentWave + 1);
        }
        return;
    }

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
        startWave(m_currentWave + 1);
    }
}

bool WaveManager::isWaveComplete() const
{
    return m_enemiesRemainingToSpawn == 0
        && m_enemyManager.getEnemyCount() == 0
        && !m_enemyManager.hasBoss();
}

int WaveManager::getCurrentWave() const
{
    return m_currentWave;
}

void WaveManager::reset()
{
    startWave(1);
}

void WaveManager::setWave(int waveNumber)
{
    startWave(waveNumber);
}