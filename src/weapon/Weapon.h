#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "bullet/Bullet.h"

class Weapon
{
public:
    Weapon();

    void update(sf::Time deltaTime, sf::Vector2f ownerPosition, sf::Angle ownerRotation);
    void draw(sf::RenderWindow& window);

    void shoot();

    // Checks if any of this weapon's bullets hit the given bounds.
    // If so, that bullet is destroyed and true is returned.
    bool checkHit(const sf::FloatRect& targetBounds);

private:
    sf::RectangleShape m_shape;
    std::vector<Bullet> m_bullets;
};