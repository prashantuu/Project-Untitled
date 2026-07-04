#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "bullet/Bullet.h"

class Weapon
{
public:
    Weapon();

    void update(sf::Time deltaTime, sf::Vector2f ownerPosition, sf::Angle ownerRotation);
    void draw(sf::RenderWindow& window);

    void shoot();
    bool checkHit(const sf::FloatRect& targetBounds);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    // Owned once here, referenced by every Bullet — see Bullet.h
    // for why bullets don't each own their own texture.
    sf::Texture m_bulletTexture;
    std::vector<Bullet> m_bullets;
};