#include "Pickup.h"
#include <cmath>

namespace
{
    // The "how much is this worth" data lives here, not in PickupManager or
    // Game - a pickup should know its own value, the same way an Enemy
    // knows its own health/speed once it's given a type.
    int getDefaultValue(PickupType type)
    {
        switch (type)
        {
            case PickupType::Health: return 50; // HP restored
            case PickupType::Ammo:   return 30; // rounds added
            case PickupType::Coin:   return 10; // score points
        }
        return 0;
    }
}

Pickup::Pickup(PickupType type, sf::Vector2f position, sf::Texture& texture)
    : m_type(type)
    , m_value(getDefaultValue(type))
    , m_sprite(texture)
    , m_bobTimer(0.f)
    , m_baseVisualPosition(position)
{
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    m_sprite.setPosition(position);
}

void Pickup::update(sf::Time deltaTime)
{
    // Same elapsed-time-up pattern used everywhere else in this project
    // (invincibility, spawn timers, bullet lifetime).
    m_bobTimer += deltaTime.asSeconds() * BOB_SPEED;
    float offsetY = std::sin(m_bobTimer) * BOB_HEIGHT;

    m_sprite.setPosition({ m_baseVisualPosition.x, m_baseVisualPosition.y + offsetY });
}

void Pickup::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::FloatRect Pickup::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

PickupType Pickup::getType() const
{
    return m_type;
}

int Pickup::getValue() const
{
    return m_value;
}