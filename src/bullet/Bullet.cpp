#include "bullet/Bullet.h"
#include <cmath>

const float Bullet::SPEED = 600.f;
const sf::Time Bullet::MAX_LIFETIME = sf::seconds(2.f);

Bullet::Bullet(sf::Vector2f position, sf::Angle direction)
    : m_shape(4.f)
    , m_lifetime(sf::Time::Zero)
{
    m_shape.setOrigin({4.f, 4.f});
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setPosition(position);

    float radians = direction.asRadians();
    m_velocity = { std::cos(radians) * SPEED, std::sin(radians) * SPEED };
}

void Bullet::update(sf::Time deltaTime)
{
    m_shape.move(m_velocity * deltaTime.asSeconds());
    m_lifetime += deltaTime;
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

bool Bullet::isAlive() const
{
    return m_lifetime < MAX_LIFETIME;
}

sf::FloatRect Bullet::getBounds() const
{
    return m_shape.getGlobalBounds();
}