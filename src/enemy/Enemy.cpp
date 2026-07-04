#include "enemy/Enemy.h"
#include <cmath>

const float Enemy::SPEED = 100.f;

Enemy::Enemy(sf::Vector2f spawnPosition)
    : m_shape({30.f, 30.f})
{
    m_shape.setOrigin({15.f, 15.f});
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition(spawnPosition);
}

void Enemy::update(sf::Time deltaTime, sf::Vector2f playerPosition)
{
    // Direction = Player Position - Enemy Position
    sf::Vector2f direction = playerPosition - m_shape.getPosition();

    // Magnitude (length) of the direction vector.
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize so the enemy always moves at a constant SPEED,
    // regardless of how far away the player is. Guard against
    // dividing by zero if the enemy is exactly on the player.
    if (length != 0.f)
    {
        direction /= length;
    }

    m_shape.move(direction * SPEED * deltaTime.asSeconds());
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

sf::FloatRect Enemy::getBounds() const
{
    return m_shape.getGlobalBounds();
}