#include "enemy/Enemy.h"
#include <cmath>

const float Enemy::SPEED = 100.f;

Enemy::Enemy(sf::Texture& texture, sf::Vector2f spawnPosition)
    : m_sprite(texture)
    // NOTE: adjust to match your actual downloaded enemy sheet.
    // Assumes 32x32 frames, 4-frame walk cycle on row 0.
    , m_walkAnimation({32, 32}, 4, 0, sf::seconds(0.15f), true)
{
    m_sprite.setTextureRect(m_walkAnimation.getCurrentFrameRect());
    m_sprite.setOrigin({16.f, 16.f});
    m_sprite.setPosition(spawnPosition);
}

void Enemy::update(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    handleMovement(deltaTime, targetPosition);

    m_walkAnimation.update(deltaTime);
    m_sprite.setTextureRect(m_walkAnimation.getCurrentFrameRect());
}

void Enemy::handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    sf::Vector2f direction = targetPosition - m_sprite.getPosition();

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0.f)
    {
        direction /= length;
    }

    m_sprite.move(direction * SPEED * deltaTime.asSeconds());
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::FloatRect Enemy::getBounds() const
{
    return m_sprite.getGlobalBounds();
}