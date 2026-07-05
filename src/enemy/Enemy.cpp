#include "enemy/Enemy.h"
#include <cmath>

Enemy::Enemy(sf::Texture& texture, sf::Vector2f spawnPosition, EnemyType type)
    : m_sprite(texture)
    , m_walkAnimation({32, 32}, 4, 0, sf::seconds(0.15f), true)
    , m_type(type)
    , m_speed(0.f)
    , m_health(0)
    , m_maxHealth(0)
    , m_attackRange(0.f)
    , m_shootCooldown(sf::Time::Zero)
    , m_shootTimer(sf::Time::Zero)
{
    applyStatsForType();

    m_sprite.setTextureRect(m_walkAnimation.getCurrentFrameRect());
    m_sprite.setOrigin({16.f, 16.f});
    m_sprite.setPosition(spawnPosition);
}

void Enemy::applyStatsForType()
{
    // All tuning lives here, in one place, keyed off the enum —
    // adding a new type later means adding one more case, not a
    // new class.
    switch (m_type)
    {
        case EnemyType::Fast:
            m_speed = 160.f;
            m_maxHealth = 25;   // dies in a single player hit
            break;

        case EnemyType::Heavy:
            m_speed = 60.f;
            m_maxHealth = 100;  // takes several hits to kill
            break;

        case EnemyType::Shooter:
            m_speed = 90.f;
            m_maxHealth = 50;
            m_attackRange = 300.f;
            m_shootCooldown = sf::seconds(1.5f);
            break;
    }

    m_health = m_maxHealth;
}

void Enemy::update(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    handleMovement(deltaTime, targetPosition);

    if (m_type == EnemyType::Shooter)
    {
        m_shootTimer += deltaTime;
    }

    m_walkAnimation.update(deltaTime);
    m_sprite.setTextureRect(m_walkAnimation.getCurrentFrameRect());
}

void Enemy::handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    sf::Vector2f direction = targetPosition - m_sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (m_type == EnemyType::Shooter && length <= m_attackRange)
    {
        // Shooter holds its distance once in range instead of
        // closing all the way in — that's what makes it feel like
        // a ranged attacker instead of a re-skinned melee enemy.
        return;
    }

    if (length != 0.f)
    {
        direction /= length;
    }

    m_sprite.move(direction * m_speed * deltaTime.asSeconds());
}

bool Enemy::tryShoot(sf::Vector2f targetPosition)
{
    if (m_type != EnemyType::Shooter)
        return false;

    sf::Vector2f direction = targetPosition - m_sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > m_attackRange)
        return false;

    if (m_shootTimer < m_shootCooldown)
        return false;

    m_shootTimer = sf::Time::Zero;
    return true;
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::FloatRect Enemy::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const
{
    return m_sprite.getPosition();
}

bool Enemy::takeDamage(int amount)
{
    m_health -= amount;
    return m_health <= 0; // true = this hit destroyed the enemy
}