#pragma once

#include <SFML/Graphics.hpp>

class Enemy
{
public:
    Enemy(sf::Vector2f spawnPosition);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_shape;
};