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
    // Still no behavior — chasing/AI comes later.
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

sf::FloatRect Enemy::getBounds() const
{
    return m_shape.getGlobalBounds();
}