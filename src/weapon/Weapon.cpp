#include "weapon/Weapon.h"
#include <algorithm>

Weapon::Weapon()
    : m_sprite(m_texture)
{
    // NOTE: replace with your actual downloaded gun/bullet sprites.
    m_texture.loadFromFile("assets/sprites/weapon/gun.png");
    m_bulletTexture.loadFromFile("assets/sprites/bullet/bullet.png");

    sf::Vector2u size = m_texture.getSize();

    // Origin at the left-center edge, not the middle — makes the
    // weapon pivot like it's held in a hand.
    m_sprite.setOrigin({0.f, size.y / 2.f});
}

void Weapon::update(sf::Time deltaTime, sf::Vector2f ownerPosition, sf::Angle ownerRotation)
{
    m_sprite.setPosition(ownerPosition);
    m_sprite.setRotation(ownerRotation);

    for (auto& bullet : m_bullets)
        bullet.update(deltaTime);

    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& bullet) { return !bullet.isAlive(); }),
        m_bullets.end());
}

void Weapon::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);

    for (auto& bullet : m_bullets)
        bullet.draw(window);
}

void Weapon::shoot()
{
    sf::Vector2u size = m_texture.getSize();

    // Spawn at the tip of the weapon sprite (its right edge),
    // transformed into world space.
    sf::Vector2f spawnPosition = m_sprite.getTransform().transformPoint(
        { static_cast<float>(size.x), size.y / 2.f });

    m_bullets.emplace_back(m_bulletTexture, spawnPosition, m_sprite.getRotation());
}

bool Weapon::checkHit(const sf::FloatRect& targetBounds)
{
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ++it)
    {
        if (it->getBounds().findIntersection(targetBounds))
        {
            m_bullets.erase(it);
            return true;
        }
    }

    return false;
}