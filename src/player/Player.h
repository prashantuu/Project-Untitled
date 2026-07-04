#pragma once

#include <SFML/Graphics.hpp>
#include "weapon/Weapon.h"

class Player
{
public:
    Player();

    void update(sf::Time deltaTime, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void shoot();
    bool checkHit(const sf::FloatRect& targetBounds);

private:
    void handleMovement(sf::Time deltaTime);
    void handleRotation(const sf::RenderWindow& window);

private:
    sf::RectangleShape m_shape;
    Weapon m_weapon;

    static const float SPEED;
};