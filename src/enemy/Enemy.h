#pragma once

#include <SFML/Graphics.hpp>

class Enemy
{
public:
    Enemy(sf::Vector2f spawnPosition);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape m_shape;
};