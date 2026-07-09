#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f position, sf::Vector2f size)
{
    bounds.position = position;
    bounds.size = size;
}

sf::FloatRect Obstacle::getBounds() const
{
    return bounds;
}