#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include "graphics/Animation.h"

enum class EnemyType
{
    Fast,
    Heavy,
    Shooter
};

enum class EnemyBehaviorState
{
    Patrol,  // wandering near spawn point, player not detected
    Chase,   // player detected, closing in
    Attack,  // (Shooter only) in range, holding position and firing
    Search,  // lost the player, moving to their last known position
    Return   // gave up searching, heading back to spawn
};

class Enemy
{
public:
    Enemy(sf::Texture& texture, sf::Vector2f spawnPosition, EnemyType type);

    void update(sf::Time deltaTime, sf::Vector2f targetPosition);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    bool takeDamage(int amount);
    bool tryShoot(sf::Vector2f targetPosition);

private:
    void applyStatsForType();
    void updateBehaviorState(sf::Time deltaTime, sf::Vector2f targetPosition);
    void handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition);
    void moveToward(sf::Vector2f destination, sf::Time deltaTime);
    float distanceTo(sf::Vector2f point) const;
    sf::Vector2f pickNewPatrolTarget();

private:
    sf::Sprite m_sprite;
    Animation m_walkAnimation;

    EnemyType m_type;
    EnemyBehaviorState m_behaviorState;

    float m_speed;
    int m_health;
    int m_maxHealth;

    float m_attackRange;
    sf::Time m_shootCooldown;
    sf::Time m_shootTimer;

    sf::Vector2f m_spawnPosition;
    sf::Vector2f m_patrolTarget;
    sf::Vector2f m_lastKnownPlayerPosition;

    float m_detectionRadius;
    sf::Time m_searchTimer;
    sf::Time m_searchDuration;

    std::mt19937 m_randomEngine;
};