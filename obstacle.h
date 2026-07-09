#pragma once

#include <SFML/Graphics.hpp>

class Obstacle
{
private:
    sf::FloatRect bounds;

public:
    Obstacle(sf::Vector2f position, sf::Vector2f size);

    sf::FloatRect getBounds() const;
};