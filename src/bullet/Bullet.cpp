#include "bullet/Bullet.h"
#include <cmath>

const float Bullet::SPEED = 600.f;
const sf::Time Bullet::MAX_LIFETIME = sf::seconds(2.f);

Bullet::Bullet(sf::Texture& texture, sf::Vector2f position, sf::Angle direction)
    : m_sprite(texture)
    , m_lifetime(sf::Time::Zero)
{
    sf::Vector2u size = texture.getSize();
    m_sprite.setOrigin({size.x / 2.f, size.y / 2.f});

    m_sprite.setPosition(position);
    m_sprite.setRotation(direction);

    float radians = direction.asRadians();
    m_velocity = { std::cos(radians) * SPEED, std::sin(radians) * SPEED };
}

void Bullet::update(sf::Time deltaTime)
{
    m_sprite.move(m_velocity * deltaTime.asSeconds());
    m_lifetime += deltaTime;
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

bool Bullet::isAlive() const
{
    return m_lifetime < MAX_LIFETIME;
}

sf::FloatRect Bullet::getBounds() const
{
    return m_sprite.getGlobalBounds();
}