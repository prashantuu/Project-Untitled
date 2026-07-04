#pragma once

#include <SFML/Graphics.hpp>
#include "graphics/Animation.h"

class Enemy
{
public:
    Enemy(sf::Texture& texture, sf::Vector2f spawnPosition);

    void update(sf::Time deltaTime, sf::Vector2f targetPosition);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;

private:
    void handleMovement(sf::Time deltaTime, sf::Vector2f targetPosition);

private:
    sf::Sprite m_sprite;

    // Enemies are always chasing in this design — no idle state —
    // so just one continuously looping animation, unlike Player
    // which switches between idle/walk.
    Animation m_walkAnimation;

    static const float SPEED;
};