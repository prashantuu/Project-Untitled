#include "enemy/EnemyManager.h"
#include <algorithm>
#include <ctime>

EnemyManager::EnemyManager(sf::Vector2f worldSize, ResourceManager& resources)
    : m_enemyTexture(resources.getTexture("assets/sprites/enemy/enemy_spritesheet.png"))
    , m_worldSize(worldSize)
    , m_randomEngine(static_cast<unsigned int>(std::time(nullptr)))
{
    // No longer spawns anything on its own — WaveManager decides
    // when the first enemies appear.
}

void EnemyManager::update(sf::Time deltaTime, sf::Vector2f playerPosition)
{
    for (auto& enemy : m_enemies)
        enemy.update(deltaTime, playerPosition);
}

sf::Vector2f EnemyManager::getRandomSpawnPosition()
{
    std::uniform_real_distribution<float> xDist(50.f, m_worldSize.x - 50.f);
    std::uniform_real_distribution<float> yDist(50.f, m_worldSize.y - 50.f);

    return { xDist(m_randomEngine), yDist(m_randomEngine) };
}

void EnemyManager::draw(sf::RenderWindow& window)
{
    for (auto& enemy : m_enemies)
        enemy.draw(window);
}

void EnemyManager::spawnEnemy(sf::Vector2f position)
{
    m_enemies.emplace_back(m_enemyTexture, position);
}

void EnemyManager::spawnRandomEnemy()
{
    spawnEnemy(getRandomSpawnPosition());
}

std::size_t EnemyManager::checkCollisions(Player& player)
{
    std::size_t countBefore = m_enemies.size();

    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [&player](Enemy& enemy) { return player.checkHit(enemy.getBounds()); }),
        m_enemies.end());

    return countBefore - m_enemies.size();
}

bool EnemyManager::checkPlayerCollision(Player& player)
{
    bool playerWasHit = false;

    for (auto& enemy : m_enemies)
    {
        if (enemy.getBounds().findIntersection(player.getBounds()))
        {
            if (player.takeDamage(20))
            {
                playerWasHit = true;
            }
        }
    }

    return playerWasHit;
}

std::size_t EnemyManager::getEnemyCount() const
{
    return m_enemies.size();
}