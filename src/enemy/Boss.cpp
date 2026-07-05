#include "enemy/Boss.h"
#include <cmath>
#include <algorithm>

Boss::Boss(sf::Texture& texture, sf::Vector2f spawnPosition)
    : m_sprite(texture)
    , m_maxHealth(500)
    , m_speed(70.f)
    , m_phase(BossPhase::Aggressive)
    , m_attackRange(250.f)
    , m_shootCooldown(sf::seconds(1.f))
    , m_shootTimer(sf::Time::Zero)
{
    m_health = m_maxHealth;

    sf::Vector2u size = texture.getSize();
    m_sprite.setOrigin({size.x / 2.f, size.y / 2.f});
    m_sprite.setPosition(spawnPosition);
    m_sprite.setScale({2.f, 2.f}); // visually bigger than regular enemies
}

void Boss::updatePhase()
{
    // Switches to Ranged once below half health — the fight
    // noticeably changes character partway through.
    if (m_phase == BossPhase::Aggressive && m_health <= m_maxHealth / 2)
    {
        m_phase = BossPhase::Ranged;
        m_shootCooldown = sf::seconds(0.6f); // fires faster once ranged
    }
}

void Boss::update(sf::Time deltaTime, sf::Vector2f playerPosition)
{
    updatePhase();
    handleMovement(deltaTime, playerPosition);

    if (m_phase == BossPhase::Ranged)
    {
        m_shootTimer += deltaTime;
    }
}

void Boss::handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition)
{
    sf::Vector2f direction = targetPosition - m_sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (m_phase == BossPhase::Ranged && length <= m_attackRange)
    {
        return; // holds ground once in range, like Shooter enemies
    }

    if (length != 0.f)
    {
        direction /= length;
    }

    m_sprite.move(direction * m_speed * deltaTime.asSeconds());
}

bool Boss::tryShoot(sf::Vector2f targetPosition)
{
    if (m_phase != BossPhase::Ranged)
        return false;

    if (m_shootTimer < m_shootCooldown)
        return false;

    m_shootTimer = sf::Time::Zero;
    return true;
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::FloatRect Boss::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

sf::Vector2f Boss::getPosition() const
{
    return m_sprite.getPosition();
}

bool Boss::takeDamage(int amount)
{
    m_health -= amount;
    return m_health <= 0;
}

int Boss::getHealth() const
{
    return m_health;
}

int Boss::getMaxHealth() const
{
    return m_maxHealth;
}