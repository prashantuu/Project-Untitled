#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet(sf::Vector2f position, sf::Angle direction);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    bool isAlive() const;
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
    sf::Time m_lifetime;

    static const float SPEED;
    static const sf::Time MAX_LIFETIME;
};