#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

    void update(sf::Time deltaTime, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    void handleMovement(sf::Time deltaTime);
    void handleRotation(const sf::RenderWindow& window);

private:
    sf::RectangleShape m_shape;

    static const float SPEED;
};