#pragma once

#include <SFML/Graphics.hpp>

enum class PickupType
{
    Health,
    Ammo
};

class Pickup
{
private:
    sf::RectangleShape body;
    PickupType type;

public:
    Pickup(sf::Vector2f position, PickupType type);

    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

    PickupType getType() const;
};