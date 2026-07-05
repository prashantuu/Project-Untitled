#include "enemy/EnemyManager.h"
#include <algorithm>
#include <ctime>
#include <cmath>

const int EnemyManager::ENEMY_BULLET_DAMAGE = 10;
const int EnemyManager::MELEE_DAMAGE = 20;

EnemyManager::EnemyManager(sf::Vector2f worldSize, ResourceManager& resources)
    : m_fastTexture(resources.getTexture("assets/sprites/enemy/fast_spritesheet.png"))
    , m_heavyTexture(resources.getTexture("assets/sprites/enemy/heavy_spritesheet.png"))
    , m_shooterTexture(resources.getTexture("assets/sprites/enemy/shooter_spritesheet.png"))
    , m_bossTexture(resources.getTexture("assets/sprites/enemy/boss_spritesheet.png"))
    , m_enemyBulletTexture(resources.getTexture("assets/sprites/bullet/bullet.png"))
    , m_worldSize(worldSize)
    , m_randomEngine(static_cast<unsigned int>(std::time(nullptr)))
{
}

sf::Texture& EnemyManager::getTextureForType(EnemyType type)
{
    switch (type)
    {
        case EnemyType::Fast:    return m_fastTexture;
        case EnemyType::Heavy:   return m_heavyTexture;
        case EnemyType::Shooter: return m_shooterTexture;
    }

    return m_fastTexture; // unreachable, keeps compiler happy
}

void EnemyManager::update(sf::Time deltaTime, sf::Vector2f playerPosition)
{
    for (auto& enemy : m_enemies)
    {
        enemy.update(deltaTime, playerPosition);

        if (enemy.tryShoot(playerPosition))
        {
            sf::Vector2f direction = playerPosition - enemy.getPosition();
            float angle = std::atan2(direction.y, direction.x);

            m_enemyBullets.emplace_back(m_enemyBulletTexture, enemy.getPosition(), sf::radians(angle), ENEMY_BULLET_DAMAGE);
        }
    }

    if (m_boss)
    {
        m_boss->update(deltaTime, playerPosition);

        if (m_boss->tryShoot(playerPosition))
        {
            sf::Vector2f direction = playerPosition - m_boss->getPosition();
            float angle = std::atan2(direction.y, direction.x);
            m_enemyBullets.emplace_back(m_enemyBulletTexture, m_boss->getPosition(), sf::radians(angle), ENEMY_BULLET_DAMAGE * 2);
        }
    }

    for (auto& bullet : m_enemyBullets)
        bullet.update(deltaTime);

    m_enemyBullets.erase(
        std::remove_if(m_enemyBullets.begin(), m_enemyBullets.end(),
            [](const Bullet& bullet) { return !bullet.isAlive(); }),
        m_enemyBullets.end());
}

sf::Vector2f EnemyManager::getRandomSpawnPosition()
{
    std::uniform_real_distribution<float> xDist(50.f, m_worldSize.x - 50.f);
    std::uniform_real_distribution<float> yDist(50.f, m_worldSize.y - 50.f);

    return { xDist(m_randomEngine), yDist(m_randomEngine) };
}

EnemyType EnemyManager::getRandomEnemyType()
{
    std::uniform_int_distribution<int> typeDist(0, 2);

    switch (typeDist(m_randomEngine))
    {
        case 0:  return EnemyType::Fast;
        case 1:  return EnemyType::Heavy;
        default: return EnemyType::Shooter;
    }
}

void EnemyManager::draw(sf::RenderWindow& window)
{
    for (auto& enemy : m_enemies)
        enemy.draw(window);

    if (m_boss)
        m_boss->draw(window);

    for (auto& bullet : m_enemyBullets)
        bullet.draw(window);
}

void EnemyManager::spawnEnemy(sf::Vector2f position, EnemyType type)
{
    m_enemies.emplace_back(getTextureForType(type), position, type);
}

void EnemyManager::spawnRandomEnemy()
{
    spawnEnemy(getRandomSpawnPosition(), getRandomEnemyType());
}

std::size_t EnemyManager::checkCollisions(Player& player)
{
    std::size_t destroyedCount = 0;

    if (m_boss)
    {
        int damage = player.checkHit(m_boss->getBounds());
        if (damage > 0 && m_boss->takeDamage(damage))
        {
            m_boss.reset();
            destroyedCount++;
        }
    }

    for (auto it = m_enemies.begin(); it != m_enemies.end();)
    {
        // Damage now comes from whichever bullet actually hit —
        // different weapons deal different damage, so a flat
        // constant no longer makes sense here.
        int damage = player.checkHit(it->getBounds());

        if (damage > 0)
        {
            bool destroyed = it->takeDamage(damage);
            if (destroyed)
            {
                it = m_enemies.erase(it);
                destroyedCount++;
                continue;
            }
        }

        ++it;
    }

    return destroyedCount;
}

bool EnemyManager::checkPlayerCollision(Player& player)
{
    bool playerWasHit = false;

    if (m_boss && m_boss->getBounds().findIntersection(player.getBounds()))
    {
        if (player.takeDamage(MELEE_DAMAGE * 2)) // boss hits harder
            playerWasHit = true;
    }

    for (auto& enemy : m_enemies)
    {
        if (enemy.getBounds().findIntersection(player.getBounds()))
        {
            if (player.takeDamage(MELEE_DAMAGE))
            {
                playerWasHit = true;
            }
        }
    }

    return playerWasHit;
}

bool EnemyManager::checkEnemyBulletHits(Player& player)
{
    bool playerWasHit = false;

    m_enemyBullets.erase(
        std::remove_if(m_enemyBullets.begin(), m_enemyBullets.end(),
            [&player, &playerWasHit](Bullet& bullet)
            {
                if (bullet.getBounds().findIntersection(player.getBounds()))
                {
                    if (player.takeDamage(bullet.getDamage()))
                    {
                        playerWasHit = true;
                    }
                    return true; // bullet is consumed on hit either way
                }
                return false;
            }),
        m_enemyBullets.end());

    return playerWasHit;
}

std::size_t EnemyManager::getEnemyCount() const
{
    return m_enemies.size();
}

void EnemyManager::reset()
{
    m_enemies.clear();
    m_enemyBullets.clear();
    m_boss.reset();
}

void EnemyManager::spawnBoss(sf::Vector2f position)
{
    m_boss.emplace(m_bossTexture, position);
}

bool EnemyManager::hasBoss() const
{
    return m_boss.has_value();
}