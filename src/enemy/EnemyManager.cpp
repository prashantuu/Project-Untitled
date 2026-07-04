#include "enemy/EnemyManager.h"
#include <algorithm>

EnemyManager::EnemyManager()
{
    // Spread across a much larger world now — you'll have to
    // actually walk to find these instead of seeing them immediately.
    spawnEnemy({1500.f, 800.f});
    spawnEnemy({1800.f, 1200.f});
    spawnEnemy({300.f, 1700.f});
}

void EnemyManager::update(sf::Time deltaTime, sf::Vector2f playerPosition)
{
    for (auto& enemy : m_enemies)
        enemy.update(deltaTime, playerPosition);
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
    // EnemyManager owns the enemies, so it owns the responsibility
    // of removing whichever ones got hit. It asks the player
    // "did your bullets hit this enemy?" without knowing how
    // that check works internally.
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [&player](Enemy& enemy) { return player.checkHit(enemy.getBounds()); }),
        m_enemies.end());
}

void EnemyManager::checkPlayerCollision(Player& player)
{
    // EnemyManager only detects and reports — it never decides how
    // much damage is dealt or touches the player's health directly.
    // That decision belongs entirely to Player::takeDamage().
    for (auto& enemy : m_enemies)
    {
        if (enemy.getBounds().findIntersection(player.getBounds()))
        {
            player.takeDamage(20);
        }
    }
}