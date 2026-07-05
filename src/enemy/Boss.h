#pragma once

#include <SFML/Graphics.hpp>

// Two phases: Aggressive (melee chase, like a Heavy enemy) and
// Ranged (holds distance and fires more often) once health drops
// below half. This is a separate class from Enemy rather than a
// 4th EnemyType because a boss has genuinely different lifecycle
// concerns (phases, one-at-a-time, tied to specific waves) — not
// just different tuning numbers.
enum class BossPhase
{
    Aggressive,
    Ranged
};

class Boss
{
public:
    Boss(sf::Texture& texture, sf::Vector2f spawnPosition);

    void update(sf::Time deltaTime, sf::Vector2f playerPosition);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    bool takeDamage(int amount); // true = boss destroyed
    bool tryShoot(sf::Vector2f targetPosition);

    int getHealth() const;
    int getMaxHealth() const;

private:
    void updatePhase();
    void handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition);

private:
    sf::Sprite m_sprite;

    int m_health;
    int m_maxHealth;
    float m_speed;
    BossPhase m_phase;

    float m_attackRange;
    sf::Time m_shootCooldown;
    sf::Time m_shootTimer;
};