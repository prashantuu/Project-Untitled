#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    // Damage now travels with the bullet itself, since different
    // weapons deal different damage — the thing that gets hit
    // shouldn't need to know which weapon fired it.
    Bullet(sf::Texture& texture, sf::Vector2f position, sf::Angle direction, int damage);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    bool isAlive() const;
    sf::FloatRect getBounds() const;
    int getDamage() const;

private:
    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;
    sf::Time m_lifetime;
    int m_damage;

    static const float SPEED;
    static const sf::Time MAX_LIFETIME;
};