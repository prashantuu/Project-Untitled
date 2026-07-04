#include "weapon/Weapon.h"
#include <algorithm>

Weapon::Weapon()
    : m_shape({24.f, 6.f})
{
    m_shape.setOrigin({0.f, 3.f});
    m_shape.setFillColor(sf::Color::White);
}

void Weapon::update(sf::Time deltaTime, sf::Vector2f ownerPosition, sf::Angle ownerRotation)
{
    m_shape.setPosition(ownerPosition);
    m_shape.setRotation(ownerRotation);

    for (auto& bullet : m_bullets)
        bullet.update(deltaTime);

    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& bullet) { return !bullet.isAlive(); }),
        m_bullets.end());
}

void Weapon::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);

    for (auto& bullet : m_bullets)
        bullet.draw(window);
}

void Weapon::shoot()
{
    sf::Vector2f spawnPosition = m_shape.getTransform().transformPoint({24.f, 3.f});
    m_bullets.emplace_back(spawnPosition, m_shape.getRotation());
}

bool Weapon::checkHit(const sf::FloatRect& targetBounds)
{
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ++it)
    {
        // findIntersection returns std::optional<FloatRect> in SFML 3 —
        // it has a value only if the two rectangles actually overlap.
        if (it->getBounds().findIntersection(targetBounds))
        {
            m_bullets.erase(it);
            return true;
        }
    }

    return false;
}