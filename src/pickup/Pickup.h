#pragma once

#include <SFML/Graphics.hpp>

enum class PickupType
{
    Health,
    Ammo,
    Coin
};

class Pickup
{
public:
    Pickup(sf::Texture& texture, sf::Vector2f position, PickupType type, int value);

    // Small bobbing motion so pickups don't look like static, dead
    // objects sitting in the world — purely cosmetic, no gameplay
    // effect.
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    PickupType getType() const;
    int getValue() const;

private:
    sf::Sprite m_sprite;
    PickupType m_type;
    int m_value;

    sf::Vector2f m_basePosition;
    sf::Time m_elapsedTime;
};