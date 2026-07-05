#pragma once

#include <SFML/Graphics.hpp>

// One Pickup class covering Health / Ammo / Coin via composition,
// same reasoning as Enemy (Fast/Heavy/Shooter) and Weapon (Pistol/Shotgun/...).
// A pickup doesn't need a different interface per type - just different
// data (which texture, how much it's worth, what it does when collected).
enum class PickupType
{
    Health,
    Ammo,
    Coin
};

class Pickup
{
public:
    Pickup(PickupType type, sf::Vector2f position, sf::Texture& texture);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    PickupType getType() const;
    int getValue() const;

private:
    PickupType   m_type;
    int          m_value;
    sf::Sprite   m_sprite;

    // Purely cosmetic idle "bob" so pickups read clearly against the floor
    // tiles instead of sitting as a flat, static icon on the ground.
    float        m_bobTimer;
    sf::Vector2f m_baseVisualPosition;

    static constexpr float BOB_SPEED  = 3.f;
    static constexpr float BOB_HEIGHT = 4.f;
};