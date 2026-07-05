#pragma once

#include <SFML/System.hpp>
#include "enemy/EnemyManager.h"

class WaveManager
{
public:
    WaveManager(EnemyManager& enemyManager);

    void update(sf::Time deltaTime);

    int getCurrentWave() const;

private:
    void startWave(int waveNumber);
    bool isWaveComplete() const;

private:
    // WaveManager doesn't own enemies — it just tells EnemyManager
    // "spawn one now" and asks "how many are alive." All the
    // actual spawning mechanics stay in EnemyManager.
    EnemyManager& m_enemyManager;

    int m_currentWave;
    int m_enemiesRemainingToSpawn;

    sf::Time m_spawnTimer;
    sf::Time m_spawnInterval;
};