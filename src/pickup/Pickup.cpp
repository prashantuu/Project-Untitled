#include "pickup/Pickup.h"
#include <cmath>

Pickup::Pickup(sf::Texture& texture, sf::Vector2f position, PickupType type, int value)
    : m_sprite(texture)
    , m_type(type)
    , m_value(value)
    , m_basePosition(position)
    , m_elapsedTime(sf::Time::Zero)
{
    sf::Vector2u size = texture.getSize();
    m_sprite.setOrigin({size.x / 2.f, size.y / 2.f});
    m_sprite.setPosition(position);
}

void Pickup::update(sf::Time deltaTime)
{
    m_elapsedTime += deltaTime;

    float bobOffset = std::sin(m_elapsedTime.asSeconds() * 4.f) * 5.f;
    m_sprite.setPosition({m_basePosition.x, m_basePosition.y + bobOffset});
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