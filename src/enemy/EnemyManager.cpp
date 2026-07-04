#include "enemy/EnemyManager.h"
#include <algorithm>
#include <ctime>

const sf::Time EnemyManager::SPAWN_INTERVAL = sf::seconds(3.f);
const std::size_t EnemyManager::MAX_ENEMIES = 10;

EnemyManager::EnemyManager(sf::Vector2f worldSize)
    : m_worldSize(worldSize)
    , m_spawnTimer(sf::Time::Zero)
    , m_randomEngine(static_cast<unsigned int>(std::time(nullptr)))
{
    // Start with a couple already in the world so it's not empty
    // the instant you press play.
    spawnEnemy(getRandomSpawnPosition());
    spawnEnemy(getRandomSpawnPosition());
}

void EnemyManager::update(sf::Time deltaTime, sf::Vector2f playerPosition)
{
    for (auto& enemy : m_enemies)
        enemy.update(deltaTime, playerPosition);

    updateSpawning(deltaTime);
}

void EnemyManager::updateSpawning(sf::Time deltaTime)
{
    m_spawnTimer += deltaTime;

    if (m_spawnTimer >= SPAWN_INTERVAL)
    {
        m_spawnTimer = sf::Time::Zero;

        // Cap total enemies so spawning forever doesn't tank
        // performance or overwhelm the player instantly.
        if (m_enemies.size() < MAX_ENEMIES)
        {
            spawnEnemy(getRandomSpawnPosition());
        }
    }
}

sf::Vector2f EnemyManager::getRandomSpawnPosition()
{
    // Keep a small margin from the world edges so enemies
    // don't spawn half-inside a boundary wall.
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
    m_enemies.emplace_back(position);
}

void EnemyManager::checkCollisions(Player& player)
{
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [&player](Enemy& enemy) { return player.checkHit(enemy.getBounds()); }),
        m_enemies.end());
}

void EnemyManager::checkPlayerCollision(Player& player)
{
    for (auto& enemy : m_enemies)
    {
        if (enemy.getBounds().findIntersection(player.getBounds()))
        {
            player.takeDamage(20);
        }
    }
}

std::size_t EnemyManager::getEnemyCount() const
{
    return m_enemies.size();
}