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

    // Remove bullets that have expired. Weapon owns its bullets,
    // so it's responsible for cleaning them up too.
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
    // Spawn the bullet at the tip of the weapon (local point {24, 3}),
    // transformed into world space so it accounts for the weapon's
    // current position and rotation.
    sf::Vector2f spawnPosition = m_shape.getTransform().transformPoint({24.f, 3.f});

    m_bullets.emplace_back(spawnPosition, m_shape.getRotation());
}