#include "enemy/Enemy.h"

Enemy::Enemy(sf::Vector2f spawnPosition)
    : m_shape({30.f, 30.f})
{
    m_shape.setOrigin({15.f, 15.f});
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition(spawnPosition);
}

void Enemy::update(sf::Time deltaTime)
{
    // No behavior yet — the enemy just exists and holds its position.
    // Chasing/AI logic comes in a later milestone.
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}