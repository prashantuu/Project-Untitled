#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    // Takes a reference to a texture owned elsewhere (by Weapon) —
    // Bullet does NOT own its own texture. This matters because
    // Bullets live in a std::vector that can reallocate; if each
    // Bullet owned its texture, the sprite's internal reference
    // could go stale when the vector moves elements around.
    Bullet(sf::Texture& texture, sf::Vector2f position, sf::Angle direction);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    bool isAlive() const;
    sf::FloatRect getBounds() const;

private:
    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;
    sf::Time m_lifetime;

    static const float SPEED;
    static const sf::Time MAX_LIFETIME;
};