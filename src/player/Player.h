#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::CircleShape m_shape;

    static const float SPEED;
};