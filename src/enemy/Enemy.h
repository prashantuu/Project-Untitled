#pragma once

#include <SFML/Graphics.hpp>
#include "graphics/Animation.h"

enum class EnemyType
{
    Fast,
    Heavy,
    Shooter
};

class Enemy
{
public:
    Enemy(sf::Texture& texture, sf::Vector2f spawnPosition, EnemyType type);

    void update(sf::Time deltaTime, sf::Vector2f targetPosition);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    // Returns true if this hit destroyed the enemy.
    bool takeDamage(int amount);

    // Shooter-only: returns true (and resets its cooldown) if this
    // enemy wants to fire right now. Fast/Heavy always return false.
    // EnemyManager owns actually creating the bullet — Enemy only
    // decides WHEN it wants to attack, same split as WaveManager
    // deciding WHEN to spawn while EnemyManager decides HOW.
    bool tryShoot(sf::Vector2f targetPosition);

private:
    void handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition);
    void applyStatsForType();

private:
    sf::Sprite m_sprite;
    Animation m_walkAnimation;

    EnemyType m_type;

    float m_speed;
    int m_health;
    int m_maxHealth;

    // Only meaningful for Shooter enemies.
    float m_attackRange;
    sf::Time m_shootCooldown;
    sf::Time m_shootTimer;
};