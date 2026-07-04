#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "bullet/Bullet.h"
#include "resource/ResourceManager.h"

class Weapon
{
public:
    Weapon(ResourceManager& resources);

    void update(sf::Time deltaTime, sf::Vector2f ownerPosition, sf::Angle ownerRotation);
    void draw(sf::RenderWindow& window);

    void shoot();
    bool checkHit(const sf::FloatRect& targetBounds);

private:
    sf::Sprite m_sprite;

    // Reference, not owned — ResourceManager owns the actual texture
    // and guarantees it never moves, so every Bullet can safely
    // hold a reference to it too.
    sf::Texture& m_bulletTexture;

    std::vector<Bullet> m_bullets;
};